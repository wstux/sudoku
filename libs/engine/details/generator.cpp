#include <cassert>

#include "engine/generator.h"
#include "engine/solver.h"
#include "engine/details/utils.h"

namespace engine {

generator::generator()
{
    init();
}

board::grid_t generator::generate()
{
    generate_board();

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

