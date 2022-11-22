#include "engine/types.h"
#include "engine/details/utils.h"

namespace engine {
namespace details {
namespace {

enum single_value_type
{
    CELL,
    COL,
    ROW
};

template<single_value_type T>
bool solve_single_value(engine::board& b)
{
    const cell_t begin_j = (T == CELL) ? 0 : BEGIN_VALUE;
    const cell_t begin_k = (T == CELL) ? BEGIN_VALUE : 0;

    const cell_t end_i = (T == COL) ? COL_SIZE : ROW_SIZE;
    const cell_t end_j = (T == CELL) ? COL_SIZE : END_VALUE;
    const cell_t end_k = (T == CELL) ? END_VALUE : ((T == COL) ? ROW_SIZE : COL_SIZE);

    bool is_found = false;
    for (cell_t i = 0; i < end_i; ++i) {
        for (cell_t j = begin_j; j < end_j; ++j) {
            size_t count = 0;
            cell_t possible_val = 0;
            size_t row = 0;
            size_t col = 0;
            for (cell_t k = begin_k; k < end_k; ++k) {
                const cell_t& r = (T == COL) ? k : i;
                const cell_t& c = (T == CELL) ? j : ((T == COL) ? i : k);
                const cell_t& v = (T == CELL) ? k : j;
                if (! b.is_set_value(r, c) && b.is_possible(r, c, v)) {
                    ++count;
                    possible_val = v;
                    row = r;
                    col = c;
                }
            }
            if (count == 1) {
                b.set_value(row, col, possible_val);
                is_found = true;
            }
        }
    }
    return is_found;
}

} // <anonumous> namespace

bool solve_single_cell(engine::board& b)// { return solve_single_value<CELL>(b); }
{
    bool is_found = false;
    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            if (b.is_set_value(r, c)) {
                continue;
            }
            size_t count = 0;
            cell_t possible_val = 0;
            for (cell_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
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

bool solve_single_value_col(engine::board& b)// { return solve_single_value<COL>(b); }
{
    bool is_found = false;
    for (size_t c = 0; c < COL_SIZE; ++c) {
        for (cell_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
            size_t count = 0;
            size_t possible_row = 0;
            cell_t possible_val = 0;
            for (size_t r = 0; r < ROW_SIZE; ++r) {
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

bool solve_single_value_row(engine::board& b)// { return solve_single_value<ROW>(b); }
{
    bool is_found = false;
    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (cell_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
            size_t count = 0;
            size_t possible_col = 0;
            cell_t possible_val = 0;
            for (size_t c = 0; c < COL_SIZE; ++c) {
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
    for (size_t s = 0; s < ROW_SIZE; ++s) {
        const size_t start_row = (s / GRID_SIZE) * GRID_SIZE;
        const size_t start_col = (s % GRID_SIZE) * GRID_SIZE;
        for (size_t r = start_row; r < start_row + GRID_SIZE; ++r) {
            for (size_t c = start_col; c < start_col + GRID_SIZE; ++c) {
                if (b.is_set_value(r, c)) {
                    continue;
                }
                size_t count = 0;
                cell_t possible_val = 0;
                for (cell_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
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

