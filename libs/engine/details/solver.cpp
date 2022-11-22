#include <cassert>
#include <cstddef>
#include <algorithm>

#include "engine/solver.h"
#include "engine/details/utils.h"

namespace engine {
namespace {

struct guess_cell_t final
{
    using available_t = std::bitset<LINE_SIZE>;

    bool is_valid = false;
    size_t row = ROW_SIZE;
    size_t col = COL_SIZE;
    available_t available;
};

guess_cell_t find_next_cell(const board& b)
{
	guess_cell_t guess;
	guess.available.set();
	assert(guess.available.count() == guess.available.size());
	for (size_t r = 0; r < ROW_SIZE; ++r) {
	    for (size_t c = 0; c < COL_SIZE; ++c) {
		    if (! b.is_set_value(r, c)) {
			    guess_cell_t::available_t available;
			    for (cell_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
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
	}
	return guess;
}

inline bool is_unique_in_col(const board_t& b, const size_t c, const cell_t v)
{
    const std::function<bool(const row_t&)> is_in_col_fn =
        [c, v](const row_t& row) -> bool { return (row[c] == v); };
    return (std::count_if(b.cbegin(), b.cend(), is_in_col_fn) == 1);
}

inline bool is_unique_in_row(const board_t& b, const size_t r, const cell_t v)
{
    return (std::count(b[r].cbegin(), b[r].cend(), v) == 1);
}

inline bool is_unique_in_grid(const board_t& b, const size_t row, const size_t col, const cell_t v)
{
    const size_t start_col = details::grid_start_col(col);
    const size_t start_row = details::grid_start_row(row);
    size_t count = 0;
    for (size_t r = start_row; r < start_row + GRID_SIZE; ++r) {
        for (size_t c = start_col; c < start_col + GRID_SIZE; ++c) {
            if (b[r][c] == v) {
                ++count;
            }
        }
    }
    return (count == 1);
}

} // <anonymous> namespace

solver::solver()
{}

solver::solver(board_t board)
    : m_solver_board(std::move(board))
{}

bool solver::is_impossible(const board& b)
{
    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            if (! b.is_set_value(r, c)) {
                size_t count = 0;
                for (cell_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
                    if (b.is_possible(r, c, v)) {
                        ++count;
                    }
                }
                if (count == 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool solver::is_solved(const board_t& b)
{
    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            if (b[r][c] == 0) { return false; }
            if (! is_unique_in_row(b, r, b[r][c]))     { return false; }
            if (! is_unique_in_col(b, r, b[r][c]))     { return false; }
            if (! is_unique_in_grid(b, r, c, b[r][c])) { return false; }
        }
    }
    return true;
}

bool solver::is_solved(const board& brd)
{
    const board_t& b = brd.solution();
    return is_solved(b);
}

bool solver::solve()
{
    return solve(m_solver_board.current_step());
}

bool solver::solve(board_t board)
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

/*bool solver::solve(int step)
{
    while (solve_single(step)) {}
    if (is_solved(m_solver_board)) { return true; }
	if (is_impossible(m_solver_board)) { return false; }

    ++step;
    return false;
}*/

bool solver::solve_single()
{
    if (details::solve_single_cell(m_solver_board))          { return true; }
    if (details::solve_single_value_col(m_solver_board))     { return true; }
    if (details::solve_single_value_row(m_solver_board))     { return true; }
    if (details::solve_single_value_section(m_solver_board)) { return true; }
    return false;
}
/*
bool solver::solve_single_cell(board& b)
{
    bool is_found = false;
    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            size_t count = 0;
            cell_t possible_val = 0;
            for (cell_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
                if (! b.is_set_value(r, c) && b.is_possible(r, c, v)) {
                    ++count;
                    possible_val = v;
                }
            }
            if (count == 1) {
                if (r == 0 && c == 1) {
                    std::cout << "CELL: v = " << ((size_t)possible_val) << "; step = " << b.current_step() << "; " << b.is_set_value(0, 1) << std::endl;
                }
                b.set_value(r, c, possible_val);
                is_found = true;
            }
        }
    }
    return is_found;
}

bool solver::solve_single_value_col(board& b)
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
                if (possible_row == 0 && c == 1) {
                    std::cout << "COL: v = " << ((size_t)possible_val) << "; step = " << b.current_step() << std::endl;
                }
                b.set_value(possible_row, c, possible_val);
                is_found = true;
            }
        }
    }
    return is_found;
}

bool solver::solve_single_value_row(board& b)
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
                if (r == 0 && possible_col == 1) {
                    std::cout << "ROW: v = " << ((size_t)possible_val) << "; step = " << b.current_step() << std::endl;
                }
                b.set_value(r, possible_col, possible_val);
                is_found = true;
            }
        }
    }
    return is_found;
}

bool solver::solve_single_value_section(board& b)
{
    bool is_found = false;
    for (size_t s = 0; s < ROW_SIZE; ++s) {
        const size_t start_row = (s / GRID_SIZE) * GRID_SIZE;
        const size_t start_col = (s % GRID_SIZE) * GRID_SIZE;
        for (size_t r = start_row; r < start_row + GRID_SIZE; ++r) {
            for (size_t c = start_col; c < start_col + GRID_SIZE; ++c) {
                size_t count = 0;
                cell_t possible_val = 0;
                for (cell_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
                    if (! b.is_set_value(r, c) && b.is_possible(r, c, v)) {
                        ++count;
                        possible_val = v;
                    }
                }
                if (count == 1) {
                    if (r == 0 && c == 1) {
                        std::cout << "GRIG: v = " << ((size_t)possible_val) << "; step = " << b.current_step() << std::endl;
                    }
                    b.set_value(r, c, possible_val);
                    is_found = true;
                }
            }
        }
    }
    return is_found;
}*/

} // namespace engine

