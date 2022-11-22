#pragma once

#include <array>
#include <bitset>

#include "engine/types.h"

namespace engine {

class board final
{
private:
    using step_t = int;
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
    using possibility_cell_t = std::array<step_t, LINE_SIZE>;
    template<typename T>
    using p_row_t = std::array<T, COL_SIZE>;
    template<typename T>
    using p_board_t = std::array<p_row_t<T>, ROW_SIZE>;

    using possibility_row_t = p_row_t<possibility_cell_t>;
    using possibility_board_t = p_board_t<possibility_cell_t>;

    using change_row_t = p_row_t<step_t>;
    using change_board_t = p_board_t<step_t>;

public:
    board();
    explicit board(board_t b);

    int current_step() const { return m_step; }

    bool is_possible(const size_t r, const size_t c, const cell_t v) const;

    bool is_set_value(const size_t r, const size_t c) const { return (m_solution[r][c] != 0); }

    void reset(board_t b);

    void rollback(const int step);

    void rollback_last() { rollback(m_step - 1); }

    bool set_value(const size_t r, const size_t c, const cell_t v);

    const board_t& solution() const { return m_solution; }

    cell_t value(const size_t r, const size_t c) const { return m_solution[r][c]; }

private:
    //cell_t find_value(const step_t step) const;

    step_change_t find_change(const step_t step) const;

    void init();

    void set_possible(const size_t r, const size_t c, cell_t v, const int s);

private:
    board_t m_base_board; // Begin state.

    step_t m_step = 0;
    board_t m_solution;
    possibility_board_t m_possible;
    change_board_t m_ch_board;
};

} // namespace engine

