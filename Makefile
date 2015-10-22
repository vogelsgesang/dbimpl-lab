# SYNOPSIS:
#
#   make [all]  - makes everything.
#   make [all] BUILD_TYPE=debug  - makes everything; uses debug flags
#   make TARGET - makes the given target.
#   make clean  - removes all files generated by make.
#   make test   - runs the test cases

CPPFLAGS = -I . -isystem lib/gtest-1.7.0/include
CXXFLAGS =-std=c++14 -Wall -Werror -pedantic -Wno-unused-function -fno-rtti
LDFLAGS  =-Wall -Werror -pthread
LDLIBS   =-lm

# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = lib/gtest-1.7.0

# possible values: "build", "release"
BUILD_TYPE=release
ifeq ($(BUILD_TYPE), debug)
  BIN_SUFFIX=_debug
  CPPFLAGS+= -DDEBUG
  CXXFLAGS+= -g3 -O0
else
  ifeq ($(BUILD_TYPE), release)
    BIN_POSTFIX=
    CXXFLAGS+=-O3
    LDFLAGS+=-O3
  else
    $(error Invalid build type: "$(BUILD_TYPE)")
  endif
endif
OBJ_DIR=build/$(BUILD_TYPE)

.PHONY: all
all: $(addsuffix $(BIN_SUFFIX), bin/runTests bin/tpcc bin/generate_schema)

.PHONY: test
test: all
	@echo "=== Executing GoogleTest tests ==="
	@./bin/runTests$(BIN_SUFFIX)

.PHONY: clean
clean:
	rm -rf build/
	rm -rf bin/

#####################
# the actual binaries
#####################

GENERATE_SCHEMA_OBJS=generate_schema.o schema/Types.o schema/Schema.o schema/Parser.o
bin/generate_schema$(BIN_SUFFIX): $(addprefix $(OBJ_DIR)/, $(GENERATE_SCHEMA_OBJS))
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $(filter-out tests, $^) $(LDLIBS) -o $@

TPCC_OBJS=tpcc_main.o schema/Types.o schema/Parser.o queries/neworderrandom.o queries/neworder.o
bin/tpcc$(BIN_SUFFIX): $(addprefix $(OBJ_DIR)/, $(TPCC_OBJS))
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $(filter-out tests, $^) $(LDLIBS) -o $@

RUNTESTS_OBJS=gtest_main.a $(patsubst %.cpp, %.o, $(shell find tests/ -iname *Test.cpp -type f)) \
							schema/Types.o schema/Parser.o
bin/runTests$(BIN_SUFFIX): CPPFLAGS+= -isystem $(GTEST_DIR)/include
#the dependency on the _directory_ containing the test specifications is neccessary in
#order to handle deleted files correctly
bin/runTests$(BIN_SUFFIX): tests $(addprefix $(OBJ_DIR)/, $(RUNTESTS_OBJS))
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $(filter-out tests, $^) $(LDLIBS) -o $@

######################
# general rules
######################

#general rule in order to build the object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#######################
#rules for GTest
######################
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

$(OBJ_DIR)/gtest-all.o: $(GTEST_SRCS_)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(OBJ_DIR)/gtest_main.o : $(GTEST_SRCS_)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(OBJ_DIR)/gtest_main.a: $(OBJ_DIR)/gtest-all.o $(OBJ_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

############################
#automatically generate Make dependency files for the included header files
############################
build/deps/$(BUILD_TYPE)/%.cpp.d: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MF $@ -MM -MP -MT $@ -MT $(OBJ_DIR)/$(basename $<).o $<

build/deps/$(BUILD_TYPE)/%.h.d: %.h
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MF $@ -MM -MP -MT $@ -MT $(OBJ_DIR)/$(basename $<).o $<

#include these make rules
DEPFILES=$(patsubst %, build/deps/$(BUILD_TYPE)/%.d, $(filter-out unused/%, $(filter-out lib/%, $(wildcard **/*.cpp **/*.h))))

-include $(DEPFILES)
