#include <cassert>
#include <cstdlib>

#include "engine/generator.h"
#include "engine/solver.h"
#include "engine/details/utils.h"

namespace engine {
namespace {

enum rotate
{
    rotate_90 = 0,
    rotate_180,
    rotate_270,
    row_mirror,
    col_mirror,
    size
};

rotate randomizer()
{
    details::init_random();
    return static_cast<rotate>(std::rand() % rotate::size);
}

size_t rotate_position(size_t pos, const rotate r)
{
    switch (r) {
    case rotate::rotate_90:
        pos = (board::ROW_SIZE - details::col_by_position(pos) - 1) * board::ROW_SIZE + details::row_by_position(pos);
        break;
    case rotate::rotate_180:
        pos = (board::ROW_SIZE - details::row_by_position(pos) - 1) * board::ROW_SIZE +
              (board::COL_SIZE - details::col_by_position(pos) - 1);
        break;
    case rotate::rotate_270:
        pos = details::col_by_position(pos) * board::ROW_SIZE + (board::COL_SIZE - details::row_by_position(pos) - 1);
        break;
    case rotate::row_mirror:
        pos = (board::ROW_SIZE - details::row_by_position(pos) - 1) * board::ROW_SIZE + details::col_by_position(pos);
        break;
    case rotate::col_mirror:
        pos = details::row_by_position(pos) * board::ROW_SIZE + (board::COL_SIZE - details::col_by_position(pos) - 1);
        break;
    default:
        break;
    }
    return pos;
}

} // <anonymous> namespace

generator::generator()
{
    init();
}

board::grid_t generator::generate()
{
    generate_board();
    details::shaffle_array(m_rand_board_idx);

    const rotate rand_rotate = randomizer();
    for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        details::shaffle_array(m_rand_board_idx);
        const size_t rand_pos = rotate_position(p, rand_rotate);
        const size_t c = details::col_by_position(random_pos(rand_pos));
        const size_t r = details::row_by_position(random_pos(rand_pos));

        if (! m_board.is_set_value(r, c)) {
            continue;
        }
        [[maybe_unused]] const board::cell_t orig_val = m_board.value(r, c);
        m_board.set_value(r, c, 0);
    }

    return m_board.grid();
}

void generator::generate_board()
{
    solver sl;
    [[maybe_unused]] const bool is_solved = sl.solve();
    assert(is_solved);

    m_board = sl.get_board();
    assert(solver::is_solved(m_board));
}

void generator::init()
{
    for (size_t i = 0; i < m_rand_board_idx.size(); ++i) {
        m_rand_board_idx[i] = i;
    }

    details::shaffle_array(m_rand_board_idx);
}

} // namespace engine

