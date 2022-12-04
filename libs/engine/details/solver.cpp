#include <cassert>
#include <cstddef>
#include <algorithm>

#include "engine/solver.h"
#include "engine/details/utils.h"

namespace engine {

solver::solver()
{
    for (size_t i = 0; i < m_rand_board_idx.size(); ++i) {
        m_rand_board_idx[i] = i;
    }
    details::shaffle_array(m_rand_board_idx);
}

solver::solver(grid_t board)
    : m_solver_board(std::move(board))
{
    for (size_t i = 0; i < m_rand_board_idx.size(); ++i) {
        m_rand_board_idx[i] = i;
    }
    details::shaffle_array(m_rand_board_idx);
}

bool solver::is_impossible(const board& b)
{
    for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        if (! b.is_set_value(p)) {
            size_t count = 0;
            for (value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
                if (b.is_possible(p, v)) {
                    ++count;
                }
            }
            if (count == 0) {
                return true;
            }
        }
    }
    return false;
}

bool solver::is_solved(const grid_t& b)
{
    for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        const size_t c = details::col_by_position(p);
        const size_t r = details::row_by_position(p);

        if (b[r][c] == 0) { return false; }
        if (! details::is_unique_in_row(b, r, b[r][c]))     { return false; }
        if (! details::is_unique_in_col(b, r, b[r][c]))     { return false; }
        if (! details::is_unique_in_grid(b, r, c, b[r][c])) { return false; }
    }
    return true;
}

bool solver::is_solved(const board& brd)
{
    const grid_t& b = brd.grid();
    return is_solved(b);
}

bool solver::solve()
{
    return solve(board::BEGIN_TAG);
}

bool solver::solve(grid_t board)
{
    m_solver_board.reset(std::move(board));
    return solve();
}

bool solver::solve(const board::tag_t tag)
{
    while (solve_single(m_solver_board, tag)) {}
    if (is_solved(m_solver_board)) { return true; }
    if (is_impossible(m_solver_board)) { return false; }

    const details::is_set_fn_t is_set_fn =
        [this](size_t p) -> bool { return m_solver_board.is_set_value(p); };
    const details::is_poss_fn_t is_poss_fn =
        [this](size_t p, board::value_t v) -> bool { return m_solver_board.is_possible(p, v); };

    details::guess_t guess = details::find_guess_cell(is_set_fn, is_poss_fn, m_rand_board_idx);
    if (! guess.is_valid()) {
        return false;
    }

    const board::tag_t guess_tag = tag + 1;
    assert(guess_tag % 2 == 0);
    const board::tag_t next_tag = tag + 2;
    assert(next_tag % 2 == 1);

    assert(guess.available.count() > 0);
    for (size_t i = 0; i < guess.available.size(); ++i) {
        if (! guess.available[i]) {
            continue;
        }
        assert(guess.available[i]);

        const value_t value = i + 1;
        assert(value > 0 && value < 10);

        m_solver_board.set_value(guess.pos, value, guess_tag);
        if (is_impossible(m_solver_board) || ! solve(next_tag)) {
            m_solver_board.rollback_to_tag(tag);
        } else {
            return true;
        }
    }
    return false;
}

bool solver::solve_single(board& b, const board::tag_t t)
{
    if (details::solve_single_cell(b, t))          { return true; }
    if (details::solve_single_value_col(b, t))     { return true; }
    if (details::solve_single_value_row(b, t))     { return true; }
    if (details::solve_single_value_section(b, t)) { return true; }
    return false;
}

} // namespace engine

