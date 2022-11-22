#pragma once

#include "engine/board.h"

namespace engine {
namespace details {

inline size_t grid_start_col(const size_t c) { return c - (c % board::GRID_SIZE); }
inline size_t grid_start_row(const size_t r) { return r - (r % board::GRID_SIZE); }

bool solve_single_cell(engine::board& b);
bool solve_single_value_col(engine::board& b);
bool solve_single_value_row(engine::board& b);
bool solve_single_value_section(engine::board& b);

} // namespace details
} // namespace engine

