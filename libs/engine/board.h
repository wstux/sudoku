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
    using step_t = int32_t;
    using tag_t = char;
    using row_t = std::array<cell_t, COL_SIZE>;
    using grid_t = std::array<row_t, ROW_SIZE>;

    static constexpr cell_t BEGIN_VALUE = 1;
    static constexpr cell_t END_VALUE = 10;

public:
    board();
    explicit board(grid_t b);

    step_t current_step() const { return m_step; }

    const grid_t& grid() const { return m_grid; }

    bool is_possible(const size_t r, const size_t c, const cell_t v) const;

    bool is_set_value(const size_t r, const size_t c) const { return (m_grid[r][c] != 0); }

    void mark_impossible(const size_t r, const size_t c, const cell_t v);

    void reset(grid_t b);

    void rollback(const step_t step);

    void rollback(const step_t step, const tag_t t);

    void rollback_last() { rollback(m_step - 1); }

    bool set_value(const size_t r, const size_t c, const cell_t v);

    bool set_value(const size_t r, const size_t c, const cell_t v, const tag_t t);

    cell_t value(const size_t r, const size_t c) const { return m_grid[r][c]; }

private:
    void init();

    void mark_impossible(const size_t r, const size_t c, cell_t v, const step_t s, const tag_t t);

    void reset_possible(const size_t r, const size_t c, cell_t v, const step_t s, const tag_t t);

    void set_impossible(const size_t r, const size_t c, cell_t v, const step_t s, const tag_t t);

private:
    static constexpr tag_t DEFAULT_TAG = 0;

    union data_t
    {
        data_t()
            : d{-1, DEFAULT_TAG}
        {}

        data_t(step_t s)
            : d{s, DEFAULT_TAG}
        {}

        data_t(step_t s, tag_t t)
            : d{s, t}
        {}

        bool operator==(const step_t s) const { return (d.step == s); }

        struct
        {
            step_t step;
            tag_t tag;
        } d;
        int64_t data;
    };
    static_assert(sizeof(data_t) == sizeof(int64_t), "Invalid 'data_t' size");

    using change_row_t = std::array<step_t, COL_SIZE>;
    using change_grid_t = std::array<change_row_t, ROW_SIZE>;

    using poss_cell_t = std::array<data_t, GRID_SIZE * GRID_SIZE>;
    using poss_row_t = std::array<poss_cell_t, COL_SIZE>;
    using poss_grid_t = std::array<poss_row_t, ROW_SIZE>;

private:
    grid_t m_grid; // Begin state.

    step_t m_step = 0;
    poss_grid_t m_possible;
    change_grid_t m_ch_grid;
};

} // namespace engine

