#pragma once

#include <array>

#include "engine/board.h"

namespace engine {

class solver final
{
private:
    using random_indices_t = std::array<size_t, board::BOARD_SIZE>;

public:
    using grid_t = board::grid_t;
    using value_t = board::value_t;

    solver();
    explicit solver(grid_t board);

    grid_t get_grid() const { return m_solver_board.grid(); }
    board get_board() const { return m_solver_board; }

    bool solve();
    bool solve(grid_t grid);

    static bool is_impossible(const board& b);

    static bool is_solve_single_tag(const board::tag_t t) { return (t % 2 == 1); }

    static bool can_solve(const grid_t& g);

    static bool is_solved(const grid_t& g);
    static bool is_solved(const board& brd);

private:
    bool solve(const board::tag_t tag);

    static bool solve_single(board& b, const board::tag_t t);

private:
    board m_solver_board;
    mutable random_indices_t m_rand_board_idx;
};

} // namespace engine

