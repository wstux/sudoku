#include <cassert>
#include <algorithm>
#include <functional>

#include "engine/board.h"
#include "engine/details/utils.h"

namespace engine {
namespace {

struct step_change_t final
{
    bool is_valid() const { return (pos != board::BOARD_SIZE);}

    size_t pos = board::BOARD_SIZE;
    board::value_t val;
};

using changed_fn_t = std::function<board::step_t(size_t)>;
using possible_fn_t = std::function<board::step_t(size_t,board::value_t)>;

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

step_change_t find_change(const changed_fn_t& ch_step_fn, const possible_fn_t& poss_step_fn, const board::step_t step)
{
    step_change_t ch;
    for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        if (ch_step_fn(p) == step) {
            ch.pos = p;
            break;
        }
    }
    for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
        if (poss_step_fn(ch.pos, v) == step) {
            ch.val = v;
            break;
        }
    }
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
        const size_t c = to_col(p);
        const size_t r = to_row(p);

        m_possible[r][c].fill(data_t(-1, DEFAULT_TAG));
        m_ch_grid[r][c] = -1;
    }

    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            if (m_grid[r][c] != 0) {
                set_impossible(r, c, m_grid[r][c], m_step, DEFAULT_TAG);
            }
        }
    }
}

bool board::is_possible(const size_t p, const value_t v) const
{
    return (m_ch_grid[to_row(p)][to_col(p)] != 0) && (m_possible[to_row(p)][to_col(p)][v - 1].d.step == -1);
}

void board::mark_impossible(const size_t p, const value_t v, const tag_t t)
{
    mark_impossible(to_row(p), to_col(p), v, m_step, t);
}

void board::mark_impossible(const size_t r, const size_t c, value_t v, const step_t s, const tag_t t)
{
    v -= 1;
    const data_t d(s, t);
    // Set columns.
    for_each_n(m_possible.begin(), ROW_SIZE,
               [c, v, d](poss_row_t& pos_r) {
                   set_if(pos_r[c][v], d, [](data_t var) -> bool { return var.d.step == -1; });
               });
    // Set rows.
    for_each_n(m_possible[r].begin(), ROW_SIZE,
               [v, d](poss_value_t& pos_cell) {
                   set_if(pos_cell[v], d, [](data_t var) -> bool { return var.d.step == -1; });
               });
    // Set grid.
    const size_t start_col = details::grid_start_col(c);
    const size_t start_row = details::grid_start_row(r);
    for (size_t row = start_row; row < start_row + GRID_SIZE; ++row) {
        poss_row_t& pos_row = m_possible[row];
        for (size_t col = start_col; col < start_col + GRID_SIZE; ++col) {
            poss_value_t& pos_cell = pos_row[col];
            set_if(pos_cell[v], d, [](data_t var) -> bool { return var.d.step == -1; });
        }
    }
}

void board::reset(grid_t b)
{
    m_grid = std::move(b);
    init();
}

void board::reset_possible(const size_t p, value_t v, const step_t s, const tag_t t)
{
    m_ch_grid[to_row(p)][to_col(p)] = -1;

    const data_t d(s, t);
    for (poss_row_t& pos_row : m_possible) {
        for (poss_value_t& pos_cell : pos_row) {
            set_if(pos_cell[v - 1], data_t(-1, DEFAULT_TAG), [d](data_t var) -> bool { return var.d.step == d.d.step; });
        }
    }
}

void board::rollback(const step_t step)
{
    rollback(step, DEFAULT_TAG);
}

void board::rollback(const step_t step, const tag_t t)
{
    if (step < 0) {
        return;
    }

    const changed_fn_t ch_step_fn = [&](size_t p) -> step_t {
        return m_ch_grid[to_row(p)][to_col(p)];
    };
    const possible_fn_t poss_step_fn = [&](size_t p, value_t v) -> step_t {
        return m_possible[to_row(p)][to_col(p)][v - 1].d.step;
    };
    while (m_step != step) {
        const step_change_t f_ch = find_change(ch_step_fn, poss_step_fn, m_step);
        if (f_ch.is_valid()) {
            reset_possible(f_ch.pos, f_ch.val, m_step, t);
            m_grid[to_row(f_ch.pos)][to_col(f_ch.pos)] = 0;
        }
        --m_step;
    }
}

void board::set_impossible(const size_t r, const size_t c, value_t v, const step_t s, const tag_t t)
{
    m_ch_grid[r][c] = s;
    mark_impossible(r, c, v, s, t);
}

bool board::set_value(const size_t p, const value_t v)
{
    return set_value(p, v, DEFAULT_TAG);
}

bool board::set_value(const size_t p, const value_t v, const tag_t t)
{
    const size_t c = to_col(p);
    const size_t r = to_row(p);

    if (m_ch_grid[r][c] == 0) {
        return false;
    }

    ++m_step;
    m_grid[r][c] = v;
    set_impossible(r, c, v, m_step, t);
    return true;
}

size_t board::to_col(const size_t p)
{
    return details::col_by_position(p);
}

size_t board::to_row(const size_t p)
{
    return details::row_by_position(p);
}

} // namespace engine

