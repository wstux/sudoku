#include <cassert>
#include <algorithm>
#include <functional>

#include "engine/board.h"
#include "engine/details/utils.h"

namespace engine {
namespace {

using ch_row_t = std::array<board::tag_t, board::COL_SIZE>;
using ch_grid_t = std::array<ch_row_t, board::ROW_SIZE>;
using possible_value_t = std::array<board::tag_t, board::GRID_SIZE * board::GRID_SIZE>;
using possible_row_t = std::array<possible_value_t, board::COL_SIZE>;
using possible_grid_t = std::array<possible_row_t, board::ROW_SIZE>;

using changed_fn_t = std::function<board::tag_t(size_t)>;
using is_correct_tag_fn_t = std::function<bool(board::tag_t)>;

struct changed_pos_t final
{
    bool is_valid() const { return (pos != board::BOARD_SIZE);}

    size_t pos = board::BOARD_SIZE;
};

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

changed_pos_t find_change(const changed_fn_t& ch_fn, const board::tag_t t)
{
    changed_pos_t ch;
    for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        if (ch_fn(p) == t) {
            ch.pos = p;
            break;
        }
    }
    return ch;
}

inline bool is_valid_tag(const board::tag_t t) { return t == board::INVALID_TAG; }

void mark(possible_grid_t& possible_grid, const size_t r, const size_t c,
          board::value_t v, const board::tag_t t, const is_correct_tag_fn_t& is_valid_fn)
{
    v -= 1;

    // Set rows.
    for (possible_row_t& row : possible_grid) {
        set_if(row[c][v], t, is_valid_fn);
    }
    // Set columns.
    for (possible_value_t& cell : possible_grid[r]) {
        set_if(cell[v], t, is_valid_fn);
    }
    // Set grid.
    const size_t start_col = details::grid_start_col(c);
    const size_t start_row = details::grid_start_row(r);
    for (size_t row = start_row; row < start_row + board::GRID_SIZE; ++row) {
        for (size_t col = start_col; col < start_col + board::GRID_SIZE; ++col) {
            set_if(possible_grid[row][col][v], t, is_valid_fn);
        }
    }
}

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
    mark(m_possible, to_row(p), to_col(p), v, t, is_valid_tag);
}

board::tag_t board::max_tag() const
{
    tag_t max_tag = DEFAULT_TAG;
    for (size_t r = 0; r < ROW_SIZE; ++r) {
        for (size_t c = 0; c < COL_SIZE; ++c) {
            max_tag = std::max(m_ch_grid[r][c], max_tag);
        }
    }

    return max_tag;
}

void board::reset(grid_t g)
{
    m_grid = std::move(g);
    init();
}

void board::rollback(const tag_t t)
{
    if (t < BEGIN_TAG) {
        return;
    }

    for (poss_row_t& pos_row : m_possible) {
        for (poss_value_t& pos_cell : pos_row) {
            for (value_t v = BEGIN_VALUE; v < END_VALUE; ++v) {
                set_if(pos_cell[v - 1], INVALID_TAG, [t](tag_t tag) -> bool { return (tag == t); });
            }
        }
    }

    const changed_fn_t ch_fn = [&](size_t p) -> tag_t { return m_ch_grid[to_row(p)][to_col(p)]; };
    changed_pos_t f_ch = find_change(ch_fn, t);
    while (f_ch.is_valid()) {
        m_ch_grid[to_row(f_ch.pos)][to_col(f_ch.pos)] = INVALID_TAG;
        m_grid[to_row(f_ch.pos)][to_col(f_ch.pos)] = 0;

        f_ch = find_change(ch_fn, t);
    }
}

void board::rollback_to_tag(const tag_t t)
{
    tag_t tag = max_tag();
    while (tag > t) {
        rollback(tag);
        --tag;
    }
}

void board::set_impossible(const size_t r, const size_t c, value_t v, const tag_t t)
{
    m_ch_grid[r][c] = t;
    mark(m_possible, r, c, v, t, is_valid_tag);
}

bool board::set_value(const size_t p, const value_t v, const tag_t t)
{
    const size_t c = to_col(p);
    const size_t r = to_row(p);

    if (m_ch_grid[r][c] == 0) {
        return false;
    } else if (m_ch_grid[r][c] != INVALID_TAG) {
        const tag_t old_tag = m_ch_grid[r][c];
        mark(m_possible, r, c, m_grid[r][c], INVALID_TAG,
             [&old_tag](tag_t t) -> bool { return (t == old_tag); });

        m_grid[r][c] = 0;
        m_ch_grid[r][c] = INVALID_TAG;
    }

    m_grid[r][c] = v;
    set_impossible(r, c, v, t);
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

