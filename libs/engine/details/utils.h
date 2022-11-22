#pragma once

#include <algorithm>
#include <functional>

#include "engine/board.h"

namespace engine {
namespace details {

inline size_t grid_start_col(const size_t c) { return c - (c % board::GRID_SIZE); }
inline size_t grid_start_row(const size_t r) { return r - (r % board::GRID_SIZE); }

inline bool is_unique_in_col(const board::grid_t& b, const size_t c, const board::cell_t v)
{
    const std::function<bool(const board::row_t&)> is_in_col_fn =
        [c, v](const board::row_t& row) -> bool { return (row[c] == v); };
    return (std::count_if(b.cbegin(), b.cend(), is_in_col_fn) == 1);
}

inline bool is_unique_in_row(const board::grid_t& b, const size_t r, const board::cell_t v)
{
    return (std::count(b[r].cbegin(), b[r].cend(), v) == 1);
}

inline bool is_unique_in_grid(const board::grid_t& b, const size_t row, const size_t col, const board::cell_t v)
{
    const size_t start_col = grid_start_col(col);
    const size_t start_row = grid_start_row(row);
    size_t count = 0;
    for (size_t r = start_row; r < start_row + board::GRID_SIZE; ++r) {
        for (size_t c = start_col; c < start_col + board::GRID_SIZE; ++c) {
            if (b[r][c] == v) {
                ++count;
            }
        }
    }
    return (count == 1);
}

inline size_t col_by_position(const size_t p) { return (p % board::COL_SIZE); }
inline size_t row_by_position(const size_t p) { return (p / board::ROW_SIZE); }

bool solve_single_cell(engine::board& b);
bool solve_single_value_col(engine::board& b);
bool solve_single_value_row(engine::board& b);
bool solve_single_value_section(engine::board& b);

} // namespace details
} // namespace engine

