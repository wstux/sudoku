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

bool mark_naked_pairs(board& b, const board::tag_t t)
{
    using has_two_possibles_fn_t = std::function<bool(size_t)>;
    using mark_pair_fn_t = std::function<bool(size_t,size_t,size_t)>;
    using pos_to_grid_start_fn_t = std::function<size_t(size_t)>;

    const std::function<bool(size_t,size_t)> are_same_fn =
        [&b](size_t p1, size_t p2) -> bool {
            for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
                if (b.is_possible(p1, v) != b.is_possible(p2, v)) {
                    return false;
                }
            }
            return true;
        };
    const has_two_possibles_fn_t has_two_possibles_fn = [&b](size_t p) -> bool {
        size_t count = 0;
        for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
            if (b.is_possible(p, v)) {
                ++count;
            }
        }
        return (count == 2);
    };
    const mark_pair_fn_t mark_pair_fn = [&b, t](size_t p1, size_t p2, size_t p3) -> bool {
        if (p3 == p1) { return false; }
        if (p3 == p2) { return false; }

        bool is_found = false;
        for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
            if (b.is_possible(p1, v) && b.is_possible(p3, v)) {
                is_found = b.set_impossible(p3, v, t);
            }
        }
        return is_found;
    };
    const pos_to_grid_start_fn_t start_col_fn = [](size_t p) -> size_t {
        return grid_start_col(col_by_position(p));
    };
    const pos_to_grid_start_fn_t start_row_fn = [](size_t p) -> size_t {
        return grid_start_row(row_by_position(p));
    };

    bool is_found = false;
    for (size_t p1 = 0; p1 < board::BOARD_SIZE; ++p1) {
        if (! has_two_possibles_fn(p1)) {
            continue;
        }

        const size_t c1 = col_by_position(p1);
        const size_t r1 = row_by_position(p1);
        const size_t st_c1 = grid_start_col(c1);
        const size_t st_r1 = grid_start_row(r1);
        assert(st_c1 == start_col_fn(p1));
        assert(st_r1 == start_row_fn(p1));
        for (size_t p2 = p1; p2 < board::BOARD_SIZE; ++p2) {
            if (p1 == p2) {
                continue;
            }
            if (! has_two_possibles_fn(p2)) {
                continue;
            }
            if (! are_same_fn(p1, p2)) {
                continue;
            }

            // Check rows.
            if (r1 == row_by_position(p2)) {
                for (size_t c2 = 0; c2 < board::COL_SIZE; ++c2) {
                    if (mark_pair_fn(p1, p2, to_position(r1, c2))) {
                        is_found = true;
                    }
                }
            }
            // Check cols.
            if (c1 == col_by_position(p2)) {
                for (size_t r2 = 0; r2 < board::ROW_SIZE; ++r2) {
                    is_found = mark_pair_fn(p1, p2, to_position(r2, c1));
                }
            }
            // Check grid.
            if ((st_c1 == start_col_fn(p2)) && (st_r1 == start_row_fn(p2))) {
                for (size_t r3 = st_r1; r3 < st_r1 + board::GRID_SIZE; ++r3) {
                    for (size_t c3 = st_c1; c3 < st_c1 + board::GRID_SIZE; ++c3) {
                        is_found = mark_pair_fn(p1, p2, to_position(r3, c3));
                    }
                }
            }
        }
    }
    return is_found;
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

