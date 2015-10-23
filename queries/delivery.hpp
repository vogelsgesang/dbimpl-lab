#ifndef H_delivery
#define H_delivery

#include <cstdint>
#include "schema/Types.hpp"
#include "schema/GeneratedSchema.hpp"

void delivery(table_data* tables, int32_t w_id, int32_t o_carrier_id, Timestamp datetime);

#endif
