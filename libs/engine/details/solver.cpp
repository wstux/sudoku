#include <cassert>
#include <cstddef>
#include <algorithm>

#include "engine/solver.h"
#include "engine/details/utils.h"

namespace engine {
namespace {

struct guess_cell_t final
{
    using available_t = std::bitset<board::GRID_SIZE * board::GRID_SIZE>;

    bool is_valid = false;
    size_t row = board::ROW_SIZE;
    size_t col = board::COL_SIZE;
    available_t available;
};

guess_cell_t find_next_cell(const board& b)
{
	guess_cell_t guess;
	guess.available.set();
	assert(guess.available.count() == guess.available.size());
	for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        const size_t c = details::col_by_position(p);
        const size_t r = details::row_by_position(p);

	    if (! b.is_set_value(r, c)) {
		    guess_cell_t::available_t available;
		    for (board::cell_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
			    if (b.is_possible(r, c, v)) {
			        available[v - 1] = true;
			    }
		    }
		    if (available.count() < guess.available.count()) {
		        guess.is_valid = true;
			    guess.row = r;
			    guess.col = c;
			    guess.available = available;
			    assert(guess.available.count() != guess.available.size());
		    }
	    }
	}
	return guess;
}

} // <anonymous> namespace

solver::solver()
{}

solver::solver(grid_t board)
    : m_solver_board(std::move(board))
{}

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
    while (solve_single()) {}
    if (is_solved(m_solver_board)) { return true; }
    if (is_impossible(m_solver_board)) { return false; }

    guess_cell_t guess = find_next_cell(m_solver_board);
    if (! guess.is_valid) {
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

        m_solver_board.set_value(guess.row, guess.col, value);
        if (is_impossible(m_solver_board) || ! solve(m_solver_board.current_step())) {
            m_solver_board.rollback(step);
        } else {
            return true;
        }
    }
    return false;
}

bool solver::solve_single()
{
    if (details::solve_single_cell(m_solver_board))          { return true; }
    if (details::solve_single_value_col(m_solver_board))     { return true; }
    if (details::solve_single_value_row(m_solver_board))     { return true; }
    if (details::solve_single_value_section(m_solver_board)) { return true; }
    return false;
}

} // namespace engine

