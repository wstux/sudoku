#include <cassert>
#include <algorithm>
#include <functional>

#include "engine/board.h"
#include "engine/details/utils.h"

namespace engine {
namespace {

struct step_change_t final
{
    board::step_t step = -1;
    board::cell_t val;
    size_t row = board::ROW_SIZE;
    size_t col = board::COL_SIZE;
};

using changed_fn = std::function<board::step_t(size_t,size_t)>;
using possible_fn = std::function<board::step_t(size_t,size_t,board::cell_t)>;

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

step_change_t find_change(const changed_fn& ch_step_fn, const possible_fn& poss_step_fn, const board::step_t step)
{
    step_change_t ch;
    for (size_t r = 0; r < board::ROW_SIZE && (ch.row == board::ROW_SIZE); ++r) {
        for (size_t c = 0; c < board::COL_SIZE; ++c) {
            if (ch_step_fn(r, c) == step) {
                ch.row = r;
                ch.col = c;
                break;
            }
        }
    }
    for (board::cell_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
        if (poss_step_fn(ch.row, ch.col, v) == step) {
            ch.val = v;
            break;
        }
    }

    ch.step = step;
    return ch;
}

} // <anonymous> namespace

board::board()
{
    for (row_t& row : m_grid) {
        row.fill(0);
    }

    init();
}

board::board(grid_t b)
    : m_grid(std::move(b))
{
    init();
}

void board::init()
{
    m_step = 0;

    for (size_t p = 0; p < BOARD_SIZE; ++p) {
        const size_t c = details::col_by_position(p);
        const size_t r = details::row_by_position(p);

        m_possible[r][c].fill(-1);
        m_ch_grid[r][c] = -1;
    }
    for (possibility_row_t& pos_row : m_possible) {
        for (possibility_cell_t& pos_cell : pos_row) {
            pos_cell.fill(-1);
        }
    }

    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            if (m_grid[r][c] != 0) {
                set_impossible(r, c, m_grid[r][c], m_step);
            }
        }
    }
}

bool board::is_possible(const size_t r, const size_t c, const cell_t v) const
{
    return (m_ch_grid[r][c] != 0) && (m_possible[r][c][v - 1] == -1);
}

void board::mark_impossible(const size_t r, const size_t c, const cell_t v)
{
    mark_impossible(r, c, v, m_step);
}

void board::mark_impossible(const size_t r, const size_t c, cell_t v, const int s)
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

void board::reset(grid_t b)
{
    m_grid = std::move(b);
    init();
}

void board::reset_possible(const size_t r, const size_t c, cell_t v, const int s)
{
    m_ch_grid[r][c] = -1;

    for (possibility_row_t& pos_row : m_possible) {
        for (possibility_cell_t& pos_cell : pos_row) {
            set_if(pos_cell[v - 1], -1, [s](int var) -> bool { return var == s; });
        }
    }
}

void board::rollback(const int step)
{
    if ((m_step < step) || (step < 0)) {
        return;
    }

    const changed_fn ch_step_fn = [&](size_t r, size_t c) -> step_t { return m_ch_grid[r][c]; };
    const possible_fn poss_step_fn = [&](size_t r, size_t c, cell_t v) -> step_t { return m_possible[r][c][v - 1]; };
    while (m_step != step) {
        const step_change_t f_ch = find_change(ch_step_fn, poss_step_fn, m_step);
        if (f_ch.step != -1) {
            reset_possible(f_ch.row, f_ch.col, f_ch.val, m_step);
            m_grid[f_ch.row][f_ch.col] = 0;
        }
        --m_step;
    }
}

void board::set_impossible(const size_t r, const size_t c, cell_t v, const int s)
{
    m_ch_grid[r][c] = s;
    mark_impossible(r, c, v, s);
}

bool board::set_value(const size_t r, const size_t c, const cell_t v)
{
    if (m_ch_grid[r][c] == 0) {
        return false;
    }

    ++m_step;
    m_grid[r][c] = v;
    set_impossible(r, c, v, m_step);
    return true;
}

} // namespace engine

