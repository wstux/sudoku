#include "engine/details/utils.h"

namespace engine {
namespace details {

bool solve_single_cell(engine::board& b)
{
    bool is_found = false;
    for (size_t r = 0; r < board::ROW_SIZE; ++r) {
        for (size_t c = 0; c < board::COL_SIZE; ++c) {
            if (b.is_set_value(r, c)) {
                continue;
            }
            size_t count = 0;
            board::cell_t possible_val = 0;
            for (board::cell_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
                if (b.is_possible(r, c, v)) {
                    ++count;
                    possible_val = v;
                }
            }
            if (count == 1) {
                b.set_value(r, c, possible_val);
                is_found = true;
            }
        }
    }
    return is_found;
}

bool solve_single_value_col(engine::board& b)
{
    bool is_found = false;
    for (size_t c = 0; c < board::COL_SIZE; ++c) {
        for (board::cell_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
            size_t count = 0;
            size_t possible_row = 0;
            board::cell_t possible_val = 0;
            for (size_t r = 0; r < board::ROW_SIZE; ++r) {
                if (! b.is_set_value(r, c) && b.is_possible(r, c, v)) {
                    ++count;
                    possible_row = r;
                    possible_val = v;
                }
            }
            if (count == 1) {
                b.set_value(possible_row, c, possible_val);
                is_found = true;
            }
        }
    }
    return is_found;
}

bool solve_single_value_row(engine::board& b)
{
    bool is_found = false;
    for (size_t r = 0; r < board::ROW_SIZE; ++r) {
        for (board::cell_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
            size_t count = 0;
            size_t possible_col = 0;
            board::cell_t possible_val = 0;
            for (size_t c = 0; c < board::COL_SIZE; ++c) {
                if (! b.is_set_value(r, c) && b.is_possible(r, c, v)) {
                    ++count;
                    possible_col = c;
                    possible_val = v;
                }
            }
            if (count == 1) {
                b.set_value(r, possible_col, possible_val);
                is_found = true;
            }
        }
    }
    return is_found;
}

bool solve_single_value_section(engine::board& b)
{
    for (size_t s = 0; s < board::ROW_SIZE; ++s) {
        const size_t start_row = (s / board::GRID_SIZE) * board::GRID_SIZE;
        const size_t start_col = (s % board::GRID_SIZE) * board::GRID_SIZE;
        for (size_t r = start_row; r < start_row + board::GRID_SIZE; ++r) {
            for (size_t c = start_col; c < start_col + board::GRID_SIZE; ++c) {
                if (b.is_set_value(r, c)) {
                    continue;
                }
                size_t count = 0;
                board::cell_t possible_val = 0;
                for (board::cell_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
                    if (b.is_possible(r, c, v)) {
                        ++count;
                        possible_val = v;
                    }
                }
                if (count == 1) {
                    b.set_value(r, c, possible_val);
                    return true;
                }
            }
        }
    }
    return false;
}

} // namespace details
} // namespace engine

