#include <limits>
#include <sstream>
#include <string>

#include "engine/board.h"
#include "engine/solver.h"
#include "engine/details/checker.h"

#include "testdefs.h"

TEST(sudoku_checker, easy)
{
    const engine::board::grid_t td = {
        {{0, 0, 0, 0, 0, 7, 9, 4, 0},
         {4, 0, 0, 5, 0, 0, 2, 0, 0},
         {7, 6, 1, 0, 2, 0, 5, 0, 0},
         {8, 0, 0, 0, 5, 0, 0, 9, 0},
         {0, 0, 9, 1, 0, 2, 7, 0, 0},
         {0, 7, 0, 0, 4, 0, 0, 0, 6},
         {0, 0, 4, 0, 1, 0, 8, 7, 5},
         {0, 0, 7, 0, 0, 4, 0, 0, 3},
         {0, 8, 3, 2, 0, 0, 0, 0, 0}}
    };

    engine::details::checker checker;

    EXPECTED(checker.calc_solutions(td) == 1)
        << "solutions_count: " << checker.solutions_count() << std::endl;
    EXPECTED(checker.difficulty() == engine::details::checker::difficult::EASY)
        << engine::details::checker::difficult_to_str(checker.difficulty()) << std::endl;
}

TEST(sudoku_checker, medium)
{
    const engine::board::grid_t td = {
        {{9, 4, 0, 1, 0, 0, 7, 0, 0},
         {0, 1, 0, 0, 0, 3, 0, 2, 0},
         {0, 0, 0, 9, 8, 0, 4, 0, 1},
         {7, 5, 0, 0, 0, 1, 0, 0, 4},
         {0, 0, 0, 0, 2, 0, 0, 0, 0},
         {1, 0, 0, 3, 0, 0, 0, 7, 2},
         {3, 0, 5, 0, 1, 8, 0, 0, 0},
         {0, 8, 0, 6, 0, 0, 0, 5, 0},
         {0, 0, 1, 0, 0, 9, 0, 8, 3}}
    };

    engine::details::checker checker;

    EXPECTED(checker.calc_solutions(td) == 1)
        << "solutions_count: " << checker.solutions_count() << std::endl;
    EXPECTED(checker.difficulty() == engine::details::checker::difficult::MEDIUM)
        << engine::details::checker::difficult_to_str(checker.difficulty()) << std::endl;
}

TEST(sudoku_checker, very_hard)
{
    const engine::board::grid_t td = {
        {{0, 6, 0, 7, 2, 0, 0, 0, 0},
         {0, 2, 0, 0, 9, 0, 0, 4, 7},
         {0, 0, 0, 0, 0, 3, 0, 0, 0},
         {0, 0, 1, 5, 0, 2, 0, 0, 9},
         {8, 5, 0, 0, 0, 0, 0, 6, 2},
         {6, 0, 0, 4, 0, 8, 3, 0, 0},
         {0, 0, 0, 3, 0, 0, 0, 0, 0},
         {7, 1, 0, 0, 5, 0, 0, 9, 0},
         {0, 0, 0, 0, 8, 9, 0, 1, 0}}
    };

    engine::details::checker checker;

    EXPECTED(checker.calc_solutions(td) == 1)
        << "solutions_count: " << checker.solutions_count() << std::endl;
    EXPECTED(checker.difficulty() == engine::details::checker::difficult::VERY_HARD)
        << engine::details::checker::difficult_to_str(checker.difficulty()) << std::endl;
}

TEST(sudoku_checker, very_hard_repeat)
{
    const engine::board::grid_t td = {
        {{0, 6, 0, 7, 2, 0, 0, 0, 0},
         {0, 2, 0, 0, 9, 0, 0, 4, 7},
         {0, 0, 0, 0, 0, 3, 0, 0, 0},
         {0, 0, 1, 5, 0, 2, 0, 0, 9},
         {8, 5, 0, 0, 0, 0, 0, 6, 2},
         {6, 0, 0, 4, 0, 8, 3, 0, 0},
         {0, 0, 0, 3, 0, 0, 0, 0, 0},
         {7, 1, 0, 0, 5, 0, 0, 9, 0},
         {0, 0, 0, 0, 8, 9, 0, 1, 0}}
    };

    engine::details::checker checker;
    for (size_t i = 0; i < std::numeric_limits<char>::max(); ++i) {
        EXPECTED(checker.calc_solutions(td) == 1)
            << "solutions_count: " << checker.solutions_count() << std::endl;
        EXPECTED(checker.difficulty() == engine::details::checker::difficult::VERY_HARD)
            << engine::details::checker::difficult_to_str(checker.difficulty()) << std::endl;
    }
}

int main()
{
    return RUN_TESTS();
}

