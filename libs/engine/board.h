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
    using change_row_t = std::array<step_t, COL_SIZE>;
    using change_grid_t = std::array<change_row_t, ROW_SIZE>;

    using possibility_cell_t = std::array<step_t, GRID_SIZE * GRID_SIZE>;
    using possibility_row_t = std::array<possibility_cell_t, COL_SIZE>;
    using possibility_grid_t = std::array<possibility_row_t, ROW_SIZE>;

public:
    board();
    explicit board(grid_t b);

    int current_step() const { return m_step; }

    const grid_t& grid() const { return m_grid; }

    bool is_possible(const size_t r, const size_t c, const cell_t v) const;

    bool is_set_value(const size_t r, const size_t c) const { return (m_grid[r][c] != 0); }

    void mark_impossible(const size_t r, const size_t c, const cell_t v);

    void reset(grid_t b);

    void rollback(const int step);

    void rollback_last() { rollback(m_step - 1); }

    bool set_value(const size_t r, const size_t c, const cell_t v);

    cell_t value(const size_t r, const size_t c) const { return m_grid[r][c]; }

private:
    void init();

    void mark_impossible(const size_t r, const size_t c, cell_t v, const int s);

    void reset_possible(const size_t r, const size_t c, cell_t v, const int s);

    void set_impossible(const size_t r, const size_t c, cell_t v, const int s);

private:
    grid_t m_grid; // Begin state.

    step_t m_step = 0;
    possibility_grid_t m_possible;
    change_grid_t m_ch_grid;
};

} // namespace engine

