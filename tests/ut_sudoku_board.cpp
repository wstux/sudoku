#include <bitset>
#include <sstream>
#include <string>

#include "engine/board.h"
#include "engine/details/utils.h"

#include "testdefs.h"

namespace {

const engine::board::grid_t td = {
        {{3, 0, 6, 5, 0, 8, 4, 0, 0},
         {5, 2, 0, 0, 0, 0, 0, 0, 0},
         {0, 8, 7, 0, 0, 0, 0, 3, 1},
         {0, 0, 3, 0, 1, 0, 0, 8, 0},
         {9, 0, 0, 8, 6, 3, 0, 0, 5},
         {0, 5, 0, 0, 9, 0, 6, 0, 0}, 
         {1, 3, 0, 0, 0, 0, 2, 5, 0},
         {0, 0, 0, 0, 0, 0, 0, 7, 4},
         {0, 0, 5, 2, 0, 6, 3, 0, 0}}
    };

bool is_possible(const engine::board& b, const size_t r, const size_t c, const engine::board::value_t& v)
{
    return b.is_possible(engine::details::to_position(r, c), v);
}

bool set_value(engine::board& b, const size_t r, const size_t c, const engine::board::value_t& v)
{
    return b.set_value(engine::details::to_position(r, c), v);
}

std::string print(const engine::board::grid_t& board)
{
    std::stringstream ss;
    for (size_t i = 0; i < board.size(); ++i) {
        for (size_t j = 0; j < board[i].size(); ++j) {
            ss << (size_t)board[i][j] << " ";
        }
        ss << std::endl;
    }
    return ss.str();
}

} // <anonymous> namespace

TEST(sudoku_board, is_possible_case_1)
{
    engine::board sb(td);

    for (size_t r = 0; r < engine::board::ROW_SIZE; ++r) {
        EXPECTED(! is_possible(sb, r, 0, 3))
            << "Rows: Invalid cell [" << r << "; 0] with value '3'" << std::endl;
    }
    for (size_t c = 0; c < engine::board::COL_SIZE; ++c) {
        EXPECTED(! is_possible(sb, 0, c, 3))
            << "Cols: Invalid cell [0; " << c << "] with value '3'" << std::endl;
    }
    for (size_t r = 0; r < engine::board::GRID_SIZE; ++r) {
        for (size_t c = 0; c < engine::board::GRID_SIZE; ++c) {
            EXPECTED(! is_possible(sb, r, c, 3))
                << "Grid: Invalid cell [" << r << "; " << c << "] with value '3'" << std::endl;
        }
    }
}

TEST(sudoku_board, is_possible_case_2)
{
    std::bitset<engine::board::COL_SIZE> possible_cols("100010001");
    std::bitset<engine::board::ROW_SIZE> possible_rows("110100000");
    std::array<std::bitset<engine::board::GRID_SIZE>, engine::board::GRID_SIZE> possible_grid = {
        {std::bitset<engine::board::GRID_SIZE>("100"),
         std::bitset<engine::board::GRID_SIZE>("101"),
         std::bitset<engine::board::GRID_SIZE>("001")}
    };

    engine::board sb(td);

    for (size_t r = 0; r < engine::board::ROW_SIZE; ++r) {
        EXPECTED(is_possible(sb, r, 0, 8) == possible_rows[r])
            << "Rows: cell [" << r << "; 0] with value '8' should be "
            << (possible_rows[r] ? "poissible" : "impoissible") << std::endl;
    }
    for (size_t c = 0; c < engine::board::COL_SIZE; ++c) {
        EXPECTED(is_possible(sb, 8, c, 8) == possible_cols[c])
            << "Cols: cell [" << c << "; 8] with value '8' should be "
            << (possible_cols[c] ? "poissible" : "impoissible") << std::endl;
    }
    for (size_t r = 6; r < 6 + engine::board::GRID_SIZE; ++r) {
        for (size_t c = 0; c < engine::board::GRID_SIZE; ++c) {
            EXPECTED(is_possible(sb, r, c, 8) == possible_grid[r - 6][c])
                << "Grid: cell [" << r << "; " << c << "] with value '8' should be "
                << (possible_grid[r - 6][c] ? "poissible" : "impoissible") << std::endl;
        }
    }
}

TEST(sudoku_board, set_value_case_1)
{
    engine::board::grid_t expected_board = td;
    expected_board[8][0] = 8;

    engine::board sb(td);

    EXPECTED(is_possible(sb, 8, 0, 8));
    EXPECTED(set_value(sb, 8, 0, 8));

    for (size_t r = 0; r < engine::board::ROW_SIZE; ++r) {
        EXPECTED(! is_possible(sb, r, 0, 8))
            << "Rows: Invalid cell [" << r << "; 0] with value '8'" << std::endl;
    }
    for (size_t c = 0; c < engine::board::COL_SIZE; ++c) {
        EXPECTED(! is_possible(sb, 8, c, 8))
            << "Cols: Invalid cell [8; " << c << "] with value '8'" << std::endl;
    }
    for (size_t r = 6; r < 6 + engine::board::GRID_SIZE; ++r) {
        for (size_t c = 0; c < engine::board::GRID_SIZE; ++c) {
            EXPECTED(! is_possible(sb, r, c, 8))
                << "Grid: Invalid cell [" << r << "; " << c << "] with value '8'" << std::endl;
        }
    }
    EXPECTED(sb.grid() == expected_board);
}

TEST(sudoku_board, set_value_case_2)
{
    engine::board sb(td);

    EXPECTED(! is_possible(sb, 2, 2, 9));
    EXPECTED(! set_value(sb, 2, 2, 9));
    EXPECTED(sb.grid() == td);
}

TEST(sudoku_board, rollback)
{
    engine::board sb(td);
    engine::details::solve_single_value_col(sb);

    sb.rollback_to_tag(engine::board::DEFAULT_TAG);
    EXPECTED(sb.current_tag() == 0);
    EXPECTED(sb.grid() == td)
        << "Etalon: " << std::endl << print(td) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

int main()
{
    return RUN_TESTS();
}

