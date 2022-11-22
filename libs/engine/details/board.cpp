#include <cassert>
#include <algorithm>
#include <functional>
#include <iostream>

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

int reset_possibility(std::array<std::array<std::array<int, LINE_SIZE>, COL_SIZE>, ROW_SIZE>& pos, const size_t r, const size_t c, cell_t v)
{
    const int step = pos[r][c][v - 1];
    assert(step != 0);
    reset_possibility(pos, v, step);
    return step;
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
    for (size_t r = 0; r < m_possible.size() && (ch.row == ROW_SIZE); ++r) {
        for (size_t c = 0; c < m_possible[r].size() && (ch.col == COL_SIZE); ++c) {
            for (size_t i = 0; i < m_possible[r][c].size(); ++i) {
                if (step == 4 && c == 7 && i == 5) {
                    std::cout << "m_possible[" << r << "][7][5] = " << m_possible[r][7][5] << std::endl;
                }
                if (m_possible[r][c][i] == step) {
                    ch.val = i + 1;
                    ch.row = r;
                    ch.col = c;
                    break;
                }
            }
        }
    }
    if (ch.row == ROW_SIZE) {
        return ch;
    }
    //for (size_t c = 0; c < m_possible[ch.row].size(); ++c) {
    //    if (m_possible[ch.row][c][ch.val - 1] == step) {
    //        ch.col = c;
    //        break;
    //    }
    //}
    if (ch.col != COL_SIZE) {
        ch.step = step;
    }
    return ch;
}

/*cell_t board::find_value(const step_t step) const
{
    for (const possibility_row_t& pos_row : m_possible) {
        for (const possibility_cell_t& pos_cell : pos_row) {
            for (size_t i = 0; i < pos_cell.size(); ++i) {
                if (pos_cell[i] == step) {
                    return (i + 1);
                }
            }
        }
    }
    return 0;
}*/

void board::init()
{
    m_step = 0;

    for (possibility_row_t& pos_row : m_possible) {
        for (possibility_cell_t& pos_cell : pos_row) {
            pos_cell.fill(-1);
        }
    }

    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            if (m_base_board[r][c] != 0) {
                m_solution[r][c] = m_base_board[r][c];
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

void board::reset_possible(const size_t r, const size_t c, cell_t v)
{
    const step_t step = reset_possibility(m_possible, r, c, v);
    m_changes.emplace(m_step, step_change_t(step, v, r, c));
}

void board::rollback(const int step)
{
    if ((m_step < step) || (step < 0)) {
        return;
    }
    while (m_step != step) {
        std::cout << "rollback step " << m_step << std::endl;
        if (m_step == 4) {
            std::cout << "m_possible[1][7][5] = " << m_possible[1][7][5] << std::endl;
            //std::cout << "m_step = " << m_step << "; step = " << step << std::endl;
            //std::cout << "f_ch.row = " << f_ch.row << "; f_ch.col = " << f_ch.col << "; f_ch.val = " << ((size_t)f_ch.val) << std::endl;
        }
        const step_change_t f_ch = find_change(m_step);
        if (f_ch.step == 4) {
            std::cout << "m_possible[1][7][5] = " << m_possible[1][7][5] << std::endl;
            std::cout << "m_step = " << m_step << "; step = " << step << std::endl;
            std::cout << "f_ch.row = " << f_ch.row << "; f_ch.col = " << f_ch.col << "; f_ch.val = " << ((size_t)f_ch.val) << std::endl;
        }
        assert(f_ch.step != -1);
        assert(f_ch.step == m_step);

        reset_possibility(m_possible, f_ch.val, m_step);
        changes_t::iterator it = m_changes.find(m_step);
        if (it != m_changes.cend()) {
            const step_change_t& ch = it->second;
            m_solution[ch.row][ch.col] = ch.val;
            set_possible(ch.row, ch.col, ch.val, ch.step);
            m_changes.erase(it);
        } else {
            m_solution[f_ch.row][f_ch.col] = 0;
        }
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
    if (r == 1 && c == 7) {
        std::cout << "step = " << m_step << "; v = " << ((size_t)v) << "; m_solution[r][c] = " << ((size_t)m_solution[r][c]) << std::endl;
    }
    if (m_solution[r][c] != 0) {
        reset_possible(r, c, m_solution[r][c]);
    }
    m_solution[r][c] = v;
    set_possible(r, c, v, m_step);
    return true;
}

} // namespace engine

