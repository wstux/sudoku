#pragma once

#include <bitset>

#include "engine/board.h"

namespace engine {

class solver final
{
public:
    using cell_t = board::cell_t;
    using grid_t = board::grid_t;

    solver();
    explicit solver(grid_t board);

    grid_t solved_board() const { return m_solver_board.grid(); }

    bool solve();
    bool solve(grid_t board);

    static bool is_impossible(const board& b);
    static bool is_solved(const grid_t& b);
    static bool is_solved(const board& brd);

private:
    bool solve(const int step);

    bool solve_single();

private:
    board m_solver_board;
};

} // namespace engine

