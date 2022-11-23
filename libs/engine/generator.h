#pragma once

#include <array>

#include "engine/board.h"

namespace engine {

class generator final
{
private:
    using random_indices_t = std::array<size_t, board::BOARD_SIZE>;

public:
    generator();

    board::grid_t generate();

private:
    void generate_board();

    void init();

    size_t random_pos(size_t p) const { return m_rand_board_idx[p]; }

private:
    board m_board;
    random_indices_t m_rand_board_idx;
};

} // namespace engine

