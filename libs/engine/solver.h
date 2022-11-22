#pragma once

#include <bitset>

#include "engine/board.h"
#include "engine/types.h"

namespace engine {

class solver final
{
public:
    solver();
    explicit solver(board_t board);

    board_t solved_board() const { return m_solver_board.solution(); }

    bool solve();
    bool solve(board_t board);

    static bool is_impossible(const board& b);
    static bool is_solved(const board_t& b);
    static bool is_solved(const board& brd);

private:
    bool solve(const int step);

    bool solve_single();

    //bool solve_single_cell(board& b);
    //bool solve_single_value_col(board& b);
    //bool solve_single_value_row(board& b);
    //bool solve_single_value_section(board& b);

private:
    board m_solver_board;
};

} // namespace engine

