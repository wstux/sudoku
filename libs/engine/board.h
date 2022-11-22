#pragma once

#include <cstddef>
#include <array>

namespace engine {

class board final
{
public:
    static constexpr size_t GRID_SIZE = 3;
    static constexpr size_t COL_SIZE = 9;
    static constexpr size_t ROW_SIZE = 9;
    static constexpr size_t BOARD_SIZE = COL_SIZE * ROW_SIZE;

    using cell_t = char;
    using step_t = int;
    using row_t = std::array<cell_t, COL_SIZE>;
    using grid_t = std::array<row_t, ROW_SIZE>;

    static constexpr cell_t BEGIN_VALUE = 1;
    static constexpr cell_t END_VALUE = 10;

private:
    struct step_change_t final
    {
        step_change_t() {}

        step_change_t(step_t s, cell_t v, size_t r, size_t c)
            : step(s)
            , val(v)
            , row(r)
            , col(c)
        {}

        step_t step = -1;
        cell_t val;
        size_t row = ROW_SIZE;
        size_t col = COL_SIZE;
    };

    using possibility_cell_t = std::array<step_t, GRID_SIZE * GRID_SIZE>;
    using possibility_row_t = std::array<possibility_cell_t, COL_SIZE>;
    using possibility_grid_t = std::array<possibility_row_t, ROW_SIZE>;

    using change_row_t = std::array<step_t, COL_SIZE>;
    using change_grid_t = std::array<change_row_t, ROW_SIZE>;

public:
    board();
    explicit board(grid_t b);

    int current_step() const { return m_step; }

    bool is_possible(const size_t r, const size_t c, const cell_t v) const;

    bool is_set_value(const size_t r, const size_t c) const { return (m_solution[r][c] != 0); }

    void reset(grid_t b);

    void rollback(const int step);

    void rollback_last() { rollback(m_step - 1); }

    bool set_value(const size_t r, const size_t c, const cell_t v);

    const grid_t& solution() const { return m_solution; }

    cell_t value(const size_t r, const size_t c) const { return m_solution[r][c]; }

private:
    //cell_t find_value(const step_t step) const;

    step_change_t find_change(const step_t step) const;

    void init();

    void set_possible(const size_t r, const size_t c, cell_t v, const int s);

private:
    grid_t m_base_board; // Begin state.

    step_t m_step = 0;
    grid_t m_solution;
    possibility_grid_t m_possible;
    change_grid_t m_ch_board;
};

} // namespace engine

