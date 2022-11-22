#include <sstream>
#include <string>

#include "engine/solver.h"

#include "testdefs.h"

namespace {

std::string print(const engine::board_t& board)
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

TEST(sudoku_solver, case_1)
{
    const engine::board_t td = {
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
    const engine::board_t etalon = {
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
    const engine::board_t res = sl.solved_board();

    EXPECTED(engine::solver::is_solved(res));
    EXPECTED(etalon == res) << "Etalon: " << std::endl << print(etalon) << std::endl
                            << "Test result: " << std::endl << print(res) << std::endl;
}

TEST(sudoku_solver, case_2)
{
    const engine::board_t td = {
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
    const engine::board_t etalon = {
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
    const engine::board_t res = sl.solved_board();

    EXPECTED(engine::solver::is_solved(res));
    EXPECTED(etalon == res) << "Etalon: " << std::endl << print(etalon) << std::endl
                            << "Test result: " << std::endl << print(res) << std::endl;
}

TEST(sudoku_solver, case_3)
{
    const engine::board_t td = {
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
    const engine::board_t etalon = {
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
    const engine::board_t res = sl.solved_board();

    EXPECTED(engine::solver::is_solved(res));
    EXPECTED(etalon == res) << "Etalon: " << std::endl << print(etalon) << std::endl
                            << "Test result: " << std::endl << print(res) << std::endl;
}

int main()
{
    return RUN_TESTS();
}
