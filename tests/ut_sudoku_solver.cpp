#include <sstream>
#include <string>

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

engine::board::value_t value(const engine::board& b, const size_t r, const size_t c)
{
    return b.value(engine::details::to_position(r, c));
}

} // <anonymous> namespace

TEST(sudoku_solver, single_solve)
{
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
    const engine::board::grid_t etalon = {
        {{3, 1, 6, 5, 7, 8, 4, 9, 2},
         {5, 2, 9, 1, 3, 4, 7, 6, 8},
         {4, 8, 7, 6, 2, 9, 5, 3, 1},
         {2, 6, 3, 4, 1, 5, 9, 8, 7},
         {9, 7, 4, 8, 6, 3, 1, 2, 5},
         {8, 5, 1, 7, 9, 2, 6, 4, 3},
         {1, 3, 8, 9, 4, 7, 2, 5, 6},
         {6, 9, 2, 3, 5, 1, 8, 7, 4},
         {7, 4, 5, 2, 8, 6, 3, 1, 9}}
    };

    engine::solver sl;

    EXPECTED(sl.solve(td));
    const engine::board::grid_t res = sl.get_grid();

    EXPECTED(engine::solver::is_solved(res));
    EXPECTED(etalon == res) << "Etalon: " << std::endl << print(etalon) << std::endl
                            << "Test result: " << std::endl << print(res) << std::endl;
}

TEST(sudoku_solver, guess_case_1)
{
    const engine::board::grid_t td = {
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
    const engine::board::grid_t etalon = {
        {{7, 3, 1, 2, 8, 6, 4, 5, 9},
         {5, 6, 2, 7, 9, 4, 8, 1, 3},
         {4, 9, 8, 3, 1, 5, 2, 6, 7},
         {8, 7, 6, 1, 4, 2, 3, 9, 5},
         {2, 4, 3, 9, 5, 7, 1, 8, 6},
         {9, 1, 5, 8, 6, 3, 7, 4, 2},
         {1, 8, 7, 6, 3, 9, 5, 2, 4},
         {3, 5, 9, 4, 2, 1, 6, 7, 8},
         {6, 2, 4, 5, 7, 8, 9, 3, 1}}
    };

    engine::solver sl;

    EXPECTED(sl.solve(td));
    const engine::board::grid_t res = sl.get_grid();

    EXPECTED(engine::solver::is_solved(res));
    EXPECTED(etalon == res) << "Etalon: " << std::endl << print(etalon) << std::endl
                            << "Test result: " << std::endl << print(res) << std::endl;
}

TEST(sudoku_solver, guess_case_2)
{
    const engine::board::grid_t td = {
        {{0, 8, 0, 0, 0, 0, 0, 2, 0},
         {5, 9, 0, 0, 3, 0, 0, 4, 1},
         {4, 0, 0, 9, 0, 5, 0, 0, 6},
         {0, 0, 0, 2, 7, 3, 0, 0, 0},
         {0, 0, 0, 8, 0, 9, 0, 0, 0},
         {9, 0, 0, 0, 1, 0, 0, 0, 2},
         {0, 7, 0, 0, 0, 0, 0, 1, 0},
         {0, 3, 9, 0, 0, 0, 2, 5, 0},
         {2, 0, 0, 0, 0, 0, 0, 0, 4}}
    };
    const engine::board::grid_t etalon = {
        {{7, 8, 3, 4, 6, 1, 5, 2, 9},
         {5, 9, 6, 7, 3, 2, 8, 4, 1},
         {4, 2, 1, 9, 8, 5, 7, 3, 6},
         {1, 6, 8, 2, 7, 3, 4, 9, 5},
         {3, 4, 2, 8, 5, 9, 1, 6, 7},
         {9, 5, 7, 6, 1, 4, 3, 8, 2},
         {8, 7, 4, 5, 2, 6, 9, 1, 3},
         {6, 3, 9, 1, 4, 7, 2, 5, 8},
         {2, 1, 5, 3, 9, 8, 6, 7, 4}}
    };

    engine::solver sl;

    EXPECTED(sl.solve(td));
    const engine::board::grid_t res = sl.get_grid();

    EXPECTED(engine::solver::is_solved(res));
    EXPECTED(etalon == res) << "Etalon: " << std::endl << print(etalon) << std::endl
                            << "Test result: " << std::endl << print(res) << std::endl;
}

TEST(sudoku_solver, solve_single_cell)
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
    engine::solver::solve_single_cell(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_solver, solve_single_value_col_case_1)
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
    engine::solver::solve_single_value_col(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_solver, solve_single_value_col_case_2)
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
    engine::solver::solve_single_value_col(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_solver, solve_single_value_row_case_1)
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
    engine::solver::solve_single_value_row(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_solver, solve_single_value_row_case_2)
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
    engine::solver::solve_single_value_row(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_solver, solve_single_value_section)
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
    engine::solver::solve_single_value_section(sb);

    EXPECTED(sb.grid() == etalon)
        << "Etalon: " << std::endl << print(etalon) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

TEST(sudoku_solver, solve_single_value)
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
    engine::solver::solve_single_cell(sb);
    engine::solver::solve_single_value_col(sb);
    engine::solver::solve_single_value_row(sb);
    engine::solver::solve_single_value_section(sb);
    EXPECTED(sb.grid() == etalon_step_1)
        << "Etalon: " << std::endl << print(etalon_step_1) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;

    engine::solver::solve_single_cell(sb);
    engine::solver::solve_single_value_col(sb);
    engine::solver::solve_single_value_row(sb);
    engine::solver::solve_single_value_section(sb);
    EXPECTED(sb.grid() == etalon_step_2)
        << "Etalon: " << std::endl << print(etalon_step_2) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;

    engine::solver::solve_single_cell(sb);
    engine::solver::solve_single_value_col(sb);
    engine::solver::solve_single_value_row(sb);
    engine::solver::solve_single_value_section(sb);
    EXPECTED(sb.grid() == etalon_step_3)
        << "Etalon: " << std::endl << print(etalon_step_3) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;

    engine::solver::solve_single_cell(sb);
    engine::solver::solve_single_value_col(sb);
    engine::solver::solve_single_value_row(sb);
    engine::solver::solve_single_value_section(sb);
    EXPECTED(sb.grid() == etalon_step_4)
        << "Etalon: " << std::endl << print(etalon_step_4) << std::endl
        << "Test result: " << std::endl << print(sb.grid()) << std::endl;
}

int main()
{
    return RUN_TESTS();
}

