#ifndef H_neworder
#define H_neworder

#include <cstdint>
#include "schema/Types.hpp"
#include "schema/GeneratedSchema.hpp"

void newOrder(table_data* tables, int32_t w_id, int32_t d_id, int32_t c_id, int32_t items, int32_t supware[15], int32_t itemid[15], int32_t qty[15], Timestamp datetime);

#endif
