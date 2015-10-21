#!/usr/bin/python3
import re
import jinja2
import inspect

def parse_sql_schema(schema_definition):
    schema = []
    for match in re.finditer(r"create table (?P<name>\S+) \((?P<body>(?:[^\)]|\)[^;])*)\);", schema_definition, re.MULTILINE | re.IGNORECASE):
        table_name = match.group('name');
        if table_name[0] == '"' and table_name[-1] == '"':
            table_name = table_name[1:-1]
        columns = []
        body = match.group('body')
        column_ddl_re = r"^\s*(?P<name>\S+)\s+(?P<type>\S+)\s+not\s+null\s*$"
        for definition in body.split(",\n"):
            match = re.match(column_ddl_re, definition, re.IGNORECASE)
            if match:
                columns.append(match.groupdict())
        schema.append({"name": table_name, "columns": columns})
    return schema


def translate_type(typedef):
    type_re = r"^(?P<type>[a-z]+)(?:\((?P<args>[^)]*)\))?$"
    match = re.match(type_re, typedef)
    if not match:
        raise RuntimeError("unknown type "+typedef)
    type_name = match.group("type").lower()
    type_args = match.group("args")
    if type_args:
        type_args = [arg.strip() for arg in type_args.split(",")]
    if type_name == "integer" and not type_args:
        return "Integer"
    if type_name == "timestamp" and not type_args:
        return "Timestamp"
    elif type_name == "varchar" and len(type_args) == 1:
        return "Varchar<{0}>".format(*type_args)
    elif type_name == "char" and len(type_args) == 1:
        return "Char<{0}>".format(*type_args)
    elif type_name == "numeric" and len(type_args) == 2:
        return "Numeric<{0},{1}>".format(*type_args)
    else:
        raise RuntimeError("unknown type "+typedef)

env = jinja2.Environment()
env.filters["translate_type"] = translate_type

with open("schema.sql") as schema_file:
    schema = parse_sql_schema(schema_file.read())

with open("schema.cpp.jinja2") as template_file:
    template = env.from_string(template_file.read())

print(template.render(schema=schema))
