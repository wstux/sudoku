#include <limits>
#include <string>

#include "engine/board.h"
#include "engine/solver.h"
#include "engine/details/checker.h"

#include "testdefs.h"

namespace {

size_t calc_solutions(engine::details::checker& checker, const engine::board::grid_t& td)
{
    size_t attempts = 10;
    size_t solutions_count = 0;
    while ((solutions_count != 1) && (attempts > 0)) {
        --attempts;
        checker.calc(td);
        solutions_count = checker.solutions_count();
    }
    return solutions_count;
}

} // <anonymous> namespace

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

    EXPECTED(calc_solutions(checker, td) == 1)
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

    EXPECTED(calc_solutions(checker, td) == 1)
        << "solutions_count: " << checker.solutions_count() << std::endl;
    EXPECTED(checker.difficulty() == engine::details::checker::difficult::MEDIUM)
        << engine::details::checker::difficult_to_str(checker.difficulty()) << std::endl;
}

TEST(sudoku_checker, hard)
{
    const engine::board::grid_t td = {
        {{0, 3, 0, 0, 0, 0, 0, 5, 6},
         {0, 0, 8, 0, 3, 7, 0, 0, 0},
         {0, 7, 0, 1, 0, 0, 3, 0, 0},
         {2, 0, 0, 0, 0, 9, 0, 0, 0},
         {3, 0, 0, 8, 5, 0, 2, 9, 0},
         {6, 0, 0, 0, 0, 3, 0, 0, 0},
         {0, 9, 0, 2, 0, 0, 5, 0, 0},
         {0, 0, 1, 0, 9, 8, 0, 0, 0},
         {0, 2, 0, 0, 0, 0, 0, 1, 9}}
    };

    engine::details::checker checker;

    EXPECTED(calc_solutions(checker, td) == 1)
        << "solutions_count: " << checker.solutions_count() << std::endl;
    EXPECTED(checker.difficulty() == engine::details::checker::difficult::HARD)
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

    EXPECTED(calc_solutions(checker, td) == 1)
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
        EXPECTED(calc_solutions(checker, td) == 1)
            << "solutions_count: " << checker.solutions_count() << std::endl;
        EXPECTED(checker.difficulty() == engine::details::checker::difficult::VERY_HARD)
            << engine::details::checker::difficult_to_str(checker.difficulty()) << std::endl;
    }
}

int main()
{
    return RUN_TESTS();
}

