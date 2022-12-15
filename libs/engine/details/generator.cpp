#include <cassert>
#include <cstdlib>

#include "engine/board_view.h"
#include "engine/generator.h"
#include "engine/solver.h"
#include "engine/details/checker.h"
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

std::string generator::difficult_to_str(const difficult d)
{
    if (d == difficult::EASY) {
        return "EASY";
    } else if (d == difficult::MEDIUM) {
        return "MEDIUM";
    } else if (d == difficult::HARD) {
        return "HARD";
    } else if (d == difficult::VERY_HARD) {
        return "VERY_HARD";
    }
    return "INVALID";
}

board::grid_t generator::generate(const difficult /*d*/)
{
    m_dif = difficult::INVALID;
    m_solutions_count = 0;

    board::grid_t grid = generate_grid();
    board_view brd(grid);
    details::shaffle_array(m_rand_board_idx);

    const rotate rand_rotate = randomizer();
    details::shaffle_array(m_rand_board_idx);

    details::checker checker;
    for (size_t p = 0; p < board::BOARD_SIZE; ++p) {
        const size_t pos = random_pos(rotate_position(p, rand_rotate));

        if (! brd.is_set_value(pos)) {
            continue;
        }
        const board::value_t orig_val = brd.value(pos);
        brd.set_value(pos, 0);
        const size_t sol_count = checker.calc_solutions(brd, 2);
        //const difficult dif = checker.difficulty();
        if ((sol_count != 1) /*&& (dif > d)*/) {
            brd.set_value(pos, orig_val);
        } else {
            //m_dif = dif;
            m_solutions_count = sol_count;
        }
    }
    m_dif = checker.calc_difficulty(brd);

    return brd.grid();
}

board::grid_t generator::generate_grid()
{
    solver sl;
    [[maybe_unused]] const bool is_solved = sl.solve();
    assert(is_solved);

    const board brd = sl.get_board();
    assert(solver::is_solved(brd));
    return brd.grid();
}

void generator::init()
{
    for (size_t i = 0; i < m_rand_board_idx.size(); ++i) {
        m_rand_board_idx[i] = i;
    }

    details::shaffle_array(m_rand_board_idx);
}

} // namespace engine

