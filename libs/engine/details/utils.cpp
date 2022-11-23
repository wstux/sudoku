#include <cstdlib>
#include <ctime>
#include "engine/details/utils.h"

namespace engine {
namespace details {

guess_cell_t find_guess_cell(const board& b, random_indices_t& rand_idx)
{
    const std::function<size_t(size_t)> pos_fn =
        [&rand_idx](size_t p) -> size_t { return rand_idx[p]; };

	guess_cell_t guess;
	shaffle_array(rand_idx);
	for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        const size_t c = col_by_position(pos_fn(p));
        const size_t r = row_by_position(pos_fn(p));

	    if (! b.is_set_value(r, c)) {
		    for (board::cell_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
			    if (b.is_possible(r, c, v)) {
			        guess.available[v - 1] = true;
			    }
		    }
		    if (guess.available.count() <= guess.available.count()) {
			    guess.pos = to_position(r, c);
			    return guess;
		    } else {
		        guess.available.reset();
		    }
	    }
	}
	return guess;
}

void init_random()
{
    static bool is_once = true;
    if (is_once) {
        is_once = false;
        std::srand(std::time(nullptr));
    }
}

bool solve_single_cell(board& b)
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

bool solve_single_value_col(board& b)
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

bool solve_single_value_row(board& b)
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

bool solve_single_value_section(board& b)
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

