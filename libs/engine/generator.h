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
    static constexpr size_t ATTEMPTS_COUNT = 255;

    enum class difficult
    {
        EASY,
        MEDIUM,
        HARD,
        VERY_HARD,
        INVALID
    };

    generator();

    difficult difficulty() const { return m_dif; }

    std::string difficulty_str() const { return difficult_to_str(difficulty()); }

    board::grid_t generate();

    size_t solutions_count() const { return m_solutions_count; }

    static std::string difficult_to_str(const difficult d);

    static board::grid_t generate_grid();

private:
    void init();

    size_t random_pos(size_t p) const { return m_rand_board_idx[p]; }

private:
    random_indices_t m_rand_board_idx;

    difficult m_dif = difficult::INVALID;
    size_t m_solutions_count = 0;
};

} // namespace engine

