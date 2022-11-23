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
        const size_t c = details::col_by_position(p);
        const size_t r = details::row_by_position(p);

        if (! b.is_set_value(r, c)) {
            size_t count = 0;
            for (cell_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
                if (b.is_possible(r, c, v)) {
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
    return solve(m_solver_board.current_step());
}

bool solver::solve(grid_t board)
{
    m_solver_board.reset(std::move(board));
    return solve(m_solver_board.current_step());
}

bool solver::solve(const int step)
{
    while (solve_single(m_solver_board)) {}
    if (is_solved(m_solver_board)) { return true; }
    if (is_impossible(m_solver_board)) { return false; }

    details::guess_cell_t guess = details::find_guess_cell(m_solver_board, m_rand_board_idx);
    if (! guess.is_valid()) {
        return false;
    }

    assert(guess.available.count() > 0);
    for (size_t i = 0; i < guess.available.size(); ++i) {
        if (! guess.available[i]) {
            continue;
        }
        assert(guess.available[i]);

        const cell_t value = i + 1;
        assert(value > 0 && value < 10);

        const size_t c = details::col_by_position(guess.pos);
        const size_t r = details::row_by_position(guess.pos);
        m_solver_board.set_value(r, c, value);
        if (is_impossible(m_solver_board) || ! solve(m_solver_board.current_step())) {
            m_solver_board.rollback(step);
        } else {
            return true;
        }
    }
    return false;
}

bool solver::solve_single(board& b)
{
    if (solve_single_cell(b))          { return true; }
    if (solve_single_value_col(b))     { return true; }
    if (solve_single_value_row(b))     { return true; }
    if (solve_single_value_section(b)) { return true; }
    return false;
}

bool solver::solve_single_cell(board& b)
{
    return details::solve_single_cell(b);
}

bool solver::solve_single_value_col(board& b)
{
    return details::solve_single_value_col(b);
}

bool solver::solve_single_value_row(board& b)
{
    return details::solve_single_value_row(b);
}

bool solver::solve_single_value_section(board& b)
{
    return details::solve_single_value_section(b);
}

} // namespace engine

