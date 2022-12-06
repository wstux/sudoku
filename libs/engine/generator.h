#pragma once

#include <array>
#include <string>

#include "engine/board.h"

namespace engine {

class generator final
{
private:
    using random_indices_t = std::array<size_t, board::BOARD_SIZE>;

public:
    enum class difficult
    {
        EASY,
        MEDIUM,
        HARD,
        VERY_HARD,
        INVALID
    };

    generator();

    board::grid_t generate(const difficult d);

    static std::string difficult_to_str(const difficult d);

    static board::grid_t generate_grid();

private:
    void init();

    size_t random_pos(size_t p) const { return m_rand_board_idx[p]; }

private:
    random_indices_t m_rand_board_idx;
};

} // namespace engine

