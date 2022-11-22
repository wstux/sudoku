#include <cassert>
#include <algorithm>
#include <functional>

#include "engine/board.h"
#include "engine/details/utils.h"

namespace engine {
namespace {

template<class TInputIterator, class TUnaryFn>
inline void for_each_n(TInputIterator it, size_t n, TUnaryFn f)
{
    for (size_t i = 0; i < n; ++it, ++i) {
        f(*it);
    }
}

template<class TType, class TUnaryFn>
inline void set_if(TType& var, const TType& val, TUnaryFn f)
{
    if (f(var)) {
        var = val;
    }
}

void reset_possibility(std::array<std::array<std::array<int, LINE_SIZE>, COL_SIZE>, ROW_SIZE>& pos, cell_t v, const int step)
{
    v -= 1;
    for (std::array<std::array<int, LINE_SIZE>, COL_SIZE>& pos_row : pos) {
        for (std::array<int, LINE_SIZE>& pos_cell : pos_row) {
            set_if(pos_cell[v], -1, [step](int var) -> bool { return var == step; });
        }
    }
}

} // <anonymous> namespace

board::board()
{}

board::board(board_t b)
    : m_base_board(std::move(b))
{
    init();
}

board::step_change_t board::find_change(const step_t step) const
{
    step_change_t ch;
    for (size_t r = 0; r < m_ch_board.size() && (ch.row == ROW_SIZE); ++r) {
        for (size_t c = 0; c < m_ch_board[r].size() && (ch.col == COL_SIZE); ++c) {
            if (m_ch_board[r][c] == step) {
                ch.row = r;
                ch.col = c;
                break;
            }
        }
    }
    assert((ch.row != ROW_SIZE) && (ch.col != COL_SIZE));
    for (cell_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
        if (m_possible[ch.row][ch.col][v - 1] == step) {
            ch.val = v;
            break;
        }
    }

    ch.step = step;
    return ch;
}

void board::init()
{
    m_step = 0;

    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            m_possible[r][c].fill(-1);
            m_ch_board[r][c] = -1;
        }
    }
    for (possibility_row_t& pos_row : m_possible) {
        for (possibility_cell_t& pos_cell : pos_row) {
            pos_cell.fill(-1);
        }
    }

    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            if (m_base_board[r][c] != 0) {
                m_solution[r][c] = m_base_board[r][c];
                m_ch_board[r][c] = m_step;
                set_possible(r, c, m_base_board[r][c], m_step);
            } else {
                m_solution[r][c] = 0;
            }
        }
    }
}

bool board::is_possible(const size_t r, const size_t c, const cell_t v) const
{
    return (m_base_board[r][c] == 0) && (m_possible[r][c][v - 1] == -1);
}

void board::reset(board_t b)
{
    m_base_board = std::move(b);
    init();
}

void board::rollback(const int step)
{
    if ((m_step < step) || (step < 0)) {
        return;
    }
    while (m_step != step) {
        const step_change_t f_ch = find_change(m_step);
        assert(f_ch.step != -1);
        assert(f_ch.step == m_step);

        reset_possibility(m_possible, f_ch.val, m_step);
        m_ch_board[f_ch.row][f_ch.col] = -1;
        m_solution[f_ch.row][f_ch.col] = 0;
        --m_step;
    }
}

void board::set_possible(const size_t r, const size_t c, cell_t v, const int s)
{
    v -= 1;
    // Set columns.
    for_each_n(m_possible.begin(), ROW_SIZE,
               [c, v, s](possibility_row_t& pos_r) {
                   set_if(pos_r[c][v], s, [](int var) -> bool { return var == -1; });
               });
    // Set rows.
    for_each_n(m_possible[r].begin(), ROW_SIZE,
               [v, s](possibility_cell_t& pos_cell) {
                   set_if(pos_cell[v], s, [](int var) -> bool { return var == -1; });
               });
    // Set grid.
    const size_t start_col = details::grid_start_col(c);
    const size_t start_row = details::grid_start_row(r);
    for (size_t row = start_row; row < start_row + GRID_SIZE; ++row) {
        possibility_row_t& pos_row = m_possible[row];
        for (size_t col = start_col; col < start_col + GRID_SIZE; ++col) {
            possibility_cell_t& pos_cell = pos_row[col];
            set_if(pos_cell[v], s, [](int var) -> bool { return var == -1; });
        }
    }
}

bool board::set_value(const size_t r, const size_t c, const cell_t v)
{
    if (m_base_board[r][c] != 0) {
        return false;
    }

    ++m_step;
    m_solution[r][c] = v;
    m_ch_board[r][c] = m_step;
    set_possible(r, c, v, m_step);
    return true;
}

} // namespace engine

