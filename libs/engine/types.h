#pragma once

#include <cstddef>
#include <array>

namespace engine {

enum difficulty_t
{
	none,
	easy,
	medium,
	hard,
	very_hard
};

constexpr size_t GRID_SIZE = 3;
constexpr size_t LINE_SIZE = 9;
constexpr size_t COL_SIZE = LINE_SIZE;
constexpr size_t ROW_SIZE = LINE_SIZE;
constexpr size_t BOARD_SIZE = COL_SIZE * ROW_SIZE;

using cell_t = char;
using row_t = std::array<cell_t, COL_SIZE>;
using board_t = std::array<row_t, ROW_SIZE>;

constexpr cell_t BEGIN_VALUE = 1;
constexpr cell_t END_VALUE = 10;

} // namespace engine

