#include <cassert>
#include <algorithm>
#include <functional>

#include "engine/board.h"
#include "engine/details/utils.h"

namespace engine {
namespace {

struct tag_change_t final
{
    bool is_valid() const { return (pos != board::BOARD_SIZE);}

    size_t pos = board::BOARD_SIZE;
    board::value_t val;
};

using changed_fn_t = std::function<board::tag_t(size_t)>;
using possible_fn_t = std::function<board::tag_t(size_t,board::value_t)>;

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

tag_change_t find_change(const changed_fn_t& is_ch_fn, const possible_fn_t& is_poss_fn, const board::tag_t t)
{
    tag_change_t ch;
    for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        if (is_ch_fn(p) == t) {
            ch.pos = p;
            break;
        }
    }
    for (board::value_t v = board::BEGIN_VALUE; v < board::END_VALUE; ++v) {
        if (is_poss_fn(ch.pos, v) == t) {
            ch.val = v;
            break;
        }
    }
    return ch;
}

inline bool is_valid_tag(const board::tag_t t) { return t == board::INVALID_TAG; }

} // <anonymous> namespace

board::board()
{
    for (row_t& row : m_grid) {
        row.fill(0);
    }

    init();
}

board::board(grid_t g)
    : m_grid(std::move(g))
{
    init();
}

void board::init()
{
    m_tag = 0;

    for (size_t p = 0; p < BOARD_SIZE; ++p) {
        const size_t c = to_col(p);
        const size_t r = to_row(p);

        m_possible[r][c].fill(INVALID_TAG);
        m_ch_grid[r][c] = INVALID_TAG;
    }

    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            if (m_grid[r][c] != 0) {
                set_impossible(r, c, m_grid[r][c], DEFAULT_TAG);
            }
        }
    }
}

bool board::is_possible(const size_t p, const value_t v) const
{
    return (m_ch_grid[to_row(p)][to_col(p)] != 0) && (m_possible[to_row(p)][to_col(p)][v - 1] == INVALID_TAG);
}

void board::mark_impossible(const size_t p, const value_t v, const tag_t t)
{
    mark_impossible(to_row(p), to_col(p), v, t);
}

void board::mark_impossible(const size_t r, const size_t c, value_t v, const tag_t t)
{
    v -= 1;

    // Set columns.
    for_each_n(m_possible.begin(), ROW_SIZE,
               [c, v, t](poss_row_t& pos_r) { set_if(pos_r[c][v], t, is_valid_tag); });

    // Set rows.
    for_each_n(m_possible[r].begin(), COL_SIZE,
               [v, t](poss_value_t& pos_cell) { set_if(pos_cell[v], t, is_valid_tag); });

    // Set grid.
    const size_t start_col = details::grid_start_col(c);
    const size_t start_row = details::grid_start_row(r);
    for (size_t row = start_row; row < start_row + GRID_SIZE; ++row) {
        poss_row_t& pos_row = m_possible[row];
        for (size_t col = start_col; col < start_col + GRID_SIZE; ++col) {
            poss_value_t& pos_cell = pos_row[col];
            set_if(pos_cell[v], t, is_valid_tag);
        }
    }
}

void board::reset(grid_t g)
{
    m_grid = std::move(g);
    init();
}

void board::reset_possible(const size_t p, value_t v, const tag_t t)
{
    m_ch_grid[to_row(p)][to_col(p)] = INVALID_TAG;

    for (poss_row_t& pos_row : m_possible) {
        for (poss_value_t& pos_cell : pos_row) {
            set_if(pos_cell[v - 1], INVALID_TAG, [t](tag_t tag) -> bool { return (tag == t); });
        }
    }
}

void board::rollback(const tag_t t)
{
    if (t < BEGIN_TAG) {
        return;
    }

    const changed_fn_t is_ch_fn = [&](size_t p) -> tag_t {
        return m_ch_grid[to_row(p)][to_col(p)];
    };
    const possible_fn_t is_poss_fn = [&](size_t p, value_t v) -> tag_t {
        return m_possible[to_row(p)][to_col(p)][v - 1];
    };

    tag_change_t f_ch = find_change(is_ch_fn, is_poss_fn, t);
    while (f_ch.is_valid()) {
        reset_possible(f_ch.pos, f_ch.val, t);
        m_grid[to_row(f_ch.pos)][to_col(f_ch.pos)] = 0;

        f_ch = find_change(is_ch_fn, is_poss_fn, t);
    }
}

void board::rollback_to_tag(const tag_t t)
{
    while (m_tag > t) {
        rollback(m_tag);
        --m_tag;
    }
}

void board::set_impossible(const size_t r, const size_t c, value_t v, const tag_t t)
{
    m_ch_grid[r][c] = t;
    mark_impossible(r, c, v, t);
}

bool board::set_value(const size_t p, const value_t v)
{
    return set_value(p, v, DEFAULT_TAG);
}

bool board::set_value(const size_t p, const value_t v, const tag_t /*t*/)
{
    const size_t c = to_col(p);
    const size_t r = to_row(p);

    if (m_ch_grid[r][c] == 0) {
        return false;
    }

    ++m_tag;
    m_grid[r][c] = v;
    set_impossible(r, c, v, m_tag);
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

