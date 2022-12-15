#include <cassert>
#include <cstdlib>
#include <ctime>
#include "engine/details/utils.h"

namespace engine {
namespace details {

guess_t find_guess_cell(const is_set_fn_t& is_set_fn, const is_poss_fn_t& is_poss_fn,
                        random_indices_t& rand_idx)
{
    guess_t guess;
    guess.available.set();
    assert(guess.available.size() == guess.available.count());

    shaffle_array(rand_idx);
    for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        guess_t::available_t available;
        const size_t pos = rand_idx[p];

        if (! is_set_fn(pos)) {
            for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
                if (is_poss_fn(pos, v)) {
                    available[v - 1] = true;
                }
            }
            if ((available.count() > 0) && (guess.available.count() >= available.count())) {
                guess.available = available;
                guess.pos = pos;
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

bool solve_single_cell(board& b, const board::tag_t t)
{
    bool is_found = false;
    for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        if (b.is_set_value(p)) {
            continue;
        }
        size_t count = 0;
        board::value_t possible_val = 0;
        for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
            if (b.is_possible(p, v)) {
                ++count;
                possible_val = v;
            }
        }
        if (count == 1) {
            b.set_value(p, possible_val, t);
            is_found = true;
        }
    }
    return is_found;
}

bool solve_single_value_col(board& b, const board::tag_t t)
{
    bool is_found = false;
    for (size_t c = 0; c < board::COL_SIZE; ++c) {
        for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
            size_t count = 0;
            size_t possible_pos = 0;
            board::value_t possible_val = 0;
            for (size_t r = 0; r < board::ROW_SIZE; ++r) {
                const size_t p = to_position(r, c);
                if (! b.is_set_value(p) && b.is_possible(p, v)) {
                    ++count;
                    possible_pos = p;
                    possible_val = v;
                }
            }
            if (count == 1) {
                b.set_value(possible_pos, possible_val, t);
                is_found = true;
            }
        }
    }
    return is_found;
}

bool solve_single_value_row(board& b, const board::tag_t t)
{
    bool is_found = false;
    for (size_t r = 0; r < board::ROW_SIZE; ++r) {
        for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
            size_t count = 0;
            size_t possible_pos = 0;
            board::value_t possible_val = 0;
            for (size_t c = 0; c < board::COL_SIZE; ++c) {
                const size_t p = to_position(r, c);
                if (! b.is_set_value(p) && b.is_possible(p, v)) {
                    ++count;
                    possible_pos = p;
                    possible_val = v;
                }
            }
            if (count == 1) {
                b.set_value(possible_pos, possible_val, t);
                is_found = true;
            }
        }
    }
    return is_found;
}

bool solve_single_value_section(board& b, const board::tag_t t)
{
    for (size_t s = 0; s < board::ROW_SIZE; ++s) {
        const size_t start_row = (s / board::GRID_SIZE) * board::GRID_SIZE;
        const size_t start_col = (s % board::GRID_SIZE) * board::GRID_SIZE;
        for (size_t r = start_row; r < start_row + board::GRID_SIZE; ++r) {
            for (size_t c = start_col; c < start_col + board::GRID_SIZE; ++c) {
                const size_t p = to_position(r, c);
                if (b.is_set_value(p)) {
                    continue;
                }
                size_t count = 0;
                board::value_t possible_val = 0;
                for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
                    if (b.is_possible(p, v)) {
                        ++count;
                        possible_val = v;
                    }
                }
                if (count == 1) {
                    b.set_value(p, possible_val, t);
                    return true;
                }
            }
        }
    }
    return false;
}

} // namespace details
} // namespace engine

