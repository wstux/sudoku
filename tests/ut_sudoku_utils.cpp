#include <sstream>
#include <string>

#include "engine/board.h"
#include "engine/solver.h"
#include "engine/details/utils.h"

#include "testdefs.h"

namespace {

const engine::board::grid_t td_1 = {
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
const engine::board::grid_t td_2 = {
        {{0, 0, 0, 2, 0, 0, 0, 5, 0},
         {5, 0, 0, 0, 9, 4, 0, 1, 0},
         {0, 0, 8, 3, 0, 0, 0, 0, 7},
         {8, 7, 0, 0, 4, 0, 0, 9, 0},
         {0, 0, 0, 9, 0, 7, 0, 0, 0},
         {0, 1, 0, 0, 6, 0, 0, 4, 2}, 
         {1, 0, 0, 0, 0, 9, 5, 0, 0},
         {0, 5, 0, 4, 2, 0, 0, 0, 8},
         {0, 2, 0, 0, 0, 8, 0, 0, 0}}
    };

bool is_set_value(const engine::board& b, const size_t r, const size_t c)
{
    return b.is_set_value(engine::details::to_position(r, c));
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

bool solve_single_cell(engine::board& b)
{
    return engine::details::solve_single_cell(b, engine::board::BEGIN_TAG);
}

bool solve_single_value_col(engine::board& b)
{
    return engine::details::solve_single_value_col(b, engine::board::BEGIN_TAG);
}

bool solve_single_value_row(engine::board& b)
{
    return engine::details::solve_single_value_row(b, engine::board::BEGIN_TAG);
}

bool solve_single_value_section(engine::board& b)
{
    return engine::details::solve_single_value_section(b, engine::board::BEGIN_TAG);
}

engine::board::value_t value(const engine::board& b, const size_t r, const size_t c)
{
    return b.value(engine::details::to_position(r, c));
}

} // <anonymous> namespace

TEST(sudoku_utils, solve_single_cell)
{
    const engine::board::grid_t etalon = {
        {{3, 0, 6, 5, 0, 8, 4, 0, 0}, 
         {5, 2, 0, 0, 0, 0, 0, 0, 0}, 
         {4, 8, 7, 0, 2, 9, 5, 3, 1}, 
         {0, 0, 3, 0, 1, 0, 0, 8, 0}, 
         {9, 0, 0, 8, 6, 3, 0, 0, 5}, 
         {0, 5, 0, 0, 9, 0, 6, 0, 0}, 
         {1, 3, 0, 0, 0, 0, 2, 5, 0}, 
         {0, 0, 0, 0, 0, 0, 0, 7, 4}, 
         {0, 0, 5, 2, 0, 6, 3, 0, 0}}
    };

    engine::board sb(td_1);
    solve_single_cell(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_utils, solve_single_value_col_case_1)
{
    const engine::board::grid_t etalon = {
        {{3, 0, 6, 5, 0, 8, 4, 0, 0},
         {5, 2, 0, 0, 0, 0, 0, 6, 0},
         {0, 8, 7, 0, 0, 0, 5, 3, 1},
         {0, 0, 3, 0, 1, 5, 0, 8, 0},
         {9, 0, 0, 8, 6, 3, 0, 0, 5},
         {0, 5, 0, 0, 9, 0, 6, 0, 3}, 
         {1, 3, 0, 0, 0, 0, 2, 5, 6},
         {0, 0, 0, 0, 5, 0, 0, 7, 4},
         {0, 0, 5, 2, 0, 6, 3, 0, 0}}
    };

    engine::board sb(td_1);
    solve_single_value_col(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_utils, solve_single_value_col_case_2)
{
    const engine::board::grid_t etalon = {
        {{0, 0, 1, 2, 0, 0, 0, 5, 0},
         {5, 0, 0, 0, 9, 4, 0, 1, 0},
         {0, 0, 8, 3, 0, 0, 0, 0, 7},
         {8, 7, 0, 0, 4, 2, 0, 9, 0},
         {0, 0, 0, 9, 0, 7, 0, 8, 0},
         {0, 1, 0, 0, 6, 0, 0, 4, 2}, 
         {1, 8, 0, 0, 0, 9, 5, 0, 0},
         {0, 5, 0, 4, 2, 0, 0, 0, 8},
         {0, 2, 0, 0, 0, 8, 0, 0, 0}}
    };

    engine::board sb(td_2);
    EXPECTED(is_set_value(sb, 1, 7));
    EXPECTED(value(sb, 1, 7) == 1);
    solve_single_value_col(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_utils, solve_single_value_row_case_1)
{
    const engine::board::grid_t etalon = {
        {{3, 1, 6, 5, 0, 8, 4, 0, 0},
         {5, 2, 0, 0, 0, 0, 0, 0, 0},
         {0, 8, 7, 6, 0, 9, 5, 3, 1},
         {0, 0, 3, 0, 1, 5, 0, 8, 0},
         {9, 0, 0, 8, 6, 3, 0, 0, 5},
         {0, 5, 0, 0, 9, 0, 6, 0, 3}, 
         {1, 3, 0, 0, 0, 0, 2, 5, 6},
         {0, 0, 0, 0, 5, 0, 0, 7, 4},
         {0, 0, 5, 2, 0, 6, 3, 1, 0}}
    };

    engine::board sb(td_1);
    solve_single_value_row(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_utils, solve_single_value_row_case_2)
{
    const engine::board::grid_t etalon = {
        {{0, 0, 0, 2, 0, 0, 0, 5, 0},
         {5, 0, 0, 0, 9, 4, 0, 1, 0},
         {0, 0, 8, 3, 0, 0, 0, 0, 7},
         {8, 7, 0, 0, 4, 0, 0, 9, 0},
         {0, 0, 0, 9, 0, 7, 0, 0, 0},
         {0, 1, 0, 8, 6, 0, 7, 4, 2}, 
         {1, 8, 0, 0, 0, 9, 5, 2, 0},
         {0, 5, 0, 4, 2, 0, 0, 0, 8},
         {0, 2, 0, 0, 0, 8, 0, 0, 0}}
    };

    engine::board sb(td_2);
    solve_single_value_row(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_utils, solve_single_value_section)
{
    const engine::board::grid_t etalon = {
        {{3, 0, 6, 5, 0, 8, 4, 0, 0},
         {5, 2, 0, 0, 0, 0, 0, 0, 0},
         {4, 8, 7, 0, 0, 0, 0, 3, 1},
         {0, 0, 3, 0, 1, 0, 0, 8, 0},
         {9, 0, 0, 8, 6, 3, 0, 0, 5},
         {0, 5, 0, 0, 9, 0, 6, 0, 0}, 
         {1, 3, 0, 0, 0, 0, 2, 5, 0},
         {0, 0, 0, 0, 0, 0, 0, 7, 4},
         {0, 0, 5, 2, 0, 6, 3, 0, 0}}
    };

    engine::board sb(td_1);
    solve_single_value_section(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_utils, solve_single_value)
{
    const engine::board::grid_t etalon_step_1 = {
        {{0, 0, 1, 2, 0, 6, 0, 5, 0},
         {5, 0, 0, 0, 9, 4, 0, 1, 0},
         {0, 0, 8, 3, 0, 0, 0, 0, 7},
         {8, 7, 0, 0, 4, 2, 0, 9, 0},
         {0, 0, 0, 9, 0, 7, 0, 8, 0},
         {0, 1, 0, 8, 6, 0, 7, 4, 2}, 
         {1, 8, 0, 0, 0, 9, 5, 2, 0},
         {0, 5, 0, 4, 2, 0, 0, 0, 8},
         {0, 2, 0, 0, 0, 8, 0, 0, 0}}
    };
    const engine::board::grid_t etalon_step_2 = {
        {{7, 0, 1, 2, 8, 6, 0, 5, 0},
         {5, 6, 2, 7, 9, 4, 8, 1, 3},
         {0, 0, 8, 3, 0, 0, 2, 6, 7},
         {8, 7, 0, 0, 4, 2, 0, 9, 0},
         {2, 0, 0, 9, 0, 7, 0, 8, 0},
         {0, 1, 0, 8, 6, 0, 7, 4, 2}, 
         {1, 8, 0, 6, 0, 9, 5, 2, 0},
         {0, 5, 0, 4, 2, 0, 0, 0, 8},
         {0, 2, 0, 0, 0, 8, 0, 0, 0}}
    };
    const engine::board::grid_t etalon_step_3 = {
        {{7, 3, 1, 2, 8, 6, 4, 5, 9},
         {5, 6, 2, 7, 9, 4, 8, 1, 3},
         {0, 0, 8, 3, 0, 0, 2, 6, 7},
         {8, 7, 0, 0, 4, 2, 0, 9, 0},
         {2, 4, 0, 9, 0, 7, 0, 8, 0},
         {0, 1, 0, 8, 6, 0, 7, 4, 2}, 
         {1, 8, 0, 6, 0, 9, 5, 2, 4},
         {0, 5, 0, 4, 2, 0, 0, 0, 8},
         {0, 2, 0, 0, 0, 8, 0, 0, 0}}
    };
    const engine::board::grid_t etalon_step_4 = {
        {{7, 3, 1, 2, 8, 6, 4, 5, 9},
         {5, 6, 2, 7, 9, 4, 8, 1, 3},
         {4, 9, 8, 3, 0, 0, 2, 6, 7},
         {8, 7, 0, 0, 4, 2, 0, 9, 0},
         {2, 4, 0, 9, 0, 7, 0, 8, 0},
         {0, 1, 0, 8, 6, 0, 7, 4, 2}, 
         {1, 8, 0, 6, 0, 9, 5, 2, 4},
         {0, 5, 0, 4, 2, 0, 0, 0, 8},
         {0, 2, 4, 0, 0, 8, 0, 0, 0}}
    };

    engine::board sb(td_2);
    solve_single_cell(sb);
    solve_single_value_col(sb);
    solve_single_value_row(sb);
    solve_single_value_section(sb);
    EXPECTED(sb.grid() == etalon_step_1)
        << "Etalon: " << std::endl << print(etalon_step_1) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;

    solve_single_cell(sb);
    solve_single_value_col(sb);
    solve_single_value_row(sb);
    solve_single_value_section(sb);
    EXPECTED(sb.grid() == etalon_step_2)
        << "Etalon: " << std::endl << print(etalon_step_2) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;

    solve_single_cell(sb);
    solve_single_value_col(sb);
    solve_single_value_row(sb);
    solve_single_value_section(sb);
    EXPECTED(sb.grid() == etalon_step_3)
        << "Etalon: " << std::endl << print(etalon_step_3) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;

    solve_single_cell(sb);
    solve_single_value_col(sb);
    solve_single_value_row(sb);
    solve_single_value_section(sb);
    EXPECTED(sb.grid() == etalon_step_4)
        << "Etalon: " << std::endl << print(etalon_step_4) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_utils, row_by_position)
{
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            const size_t pos = i * 9 + j;
            EXPECTED(engine::details::row_by_position(pos) == i)
                << "position " << pos << " not equal " << i << " row" << std::endl;
        }
    }
}

TEST(sudoku_utils, col_by_position)
{
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            const size_t pos = i * 9 + j;
            EXPECTED(engine::details::col_by_position(pos) == j)
                << "position " << pos << " not equal " << j << " col" << std::endl;
        }
    }
}

int main()
{
    return RUN_TESTS();
}

