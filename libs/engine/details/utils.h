#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <functional>

#include "engine/board.h"

namespace engine {
namespace details {

using is_poss_fn_t = std::function<bool(size_t,board::value_t)>;
using is_set_fn_t = std::function<bool(size_t)>;
using random_indices_t = std::array<size_t, board::BOARD_SIZE>;

struct guess_t final
{
    using available_t = std::bitset<board::GRID_SIZE * board::GRID_SIZE>;

    bool is_valid() const { return (pos != board::BOARD_SIZE);}

    size_t pos = board::BOARD_SIZE;
    available_t available;
};

guess_t find_guess_cell(const is_set_fn_t& is_set_fn, const is_poss_fn_t& is_poss_fn,
                        random_indices_t& rand_idx);

inline size_t grid_start_col(const size_t c) { return c - (c % board::GRID_SIZE); }
inline size_t grid_start_row(const size_t r) { return r - (r % board::GRID_SIZE); }

void init_random();

inline bool is_unique_in_col(const board::grid_t& b, const size_t c, const board::value_t v)
{
    const std::function<bool(const board::row_t&)> is_in_col_fn =
        [c, v](const board::row_t& row) -> bool { return (row[c] == v); };
    return (std::count_if(b.cbegin(), b.cend(), is_in_col_fn) == 1);
}

inline bool is_unique_in_row(const board::grid_t& b, const size_t r, const board::value_t v)
{
    return (std::count(b[r].cbegin(), b[r].cend(), v) == 1);
}

inline bool is_unique_in_grid(const board::grid_t& b, const size_t row, const size_t col, const board::value_t v)
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
inline size_t to_position(const size_t r, const size_t c) { return (r * board::ROW_SIZE + c); }

template<typename TArray>
void shaffle_array(TArray& array)
{
    init_random();

    for (size_t i = 0; i < array.size(); ++i) {
        const size_t tail = array.size() - i;
        const size_t tail_idx = std::rand() % tail + i;

        std::swap(array[i], array[tail_idx]);
    }
}

bool mark_hidden_pairs_col(board& b, const board::tag_t t);
bool mark_hidden_pairs_row(board& b, const board::tag_t t);

bool mark_naked_pairs(board& b, const board::tag_t t);

bool solve_single_cell(board& b, const board::tag_t t);
bool solve_single_value_col(board& b, const board::tag_t t);
bool solve_single_value_row(board& b, const board::tag_t t);
bool solve_single_value_section(board& b, const board::tag_t t);

} // namespace details
} // namespace engine

