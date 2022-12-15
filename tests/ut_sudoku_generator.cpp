#include <limits>
#include <sstream>
#include <string>

#include "engine/board.h"
#include "engine/generator.h"
#include "engine/solver.h"

#include "testdefs.h"

namespace {

engine::board::grid_t generate(engine::generator& gen, const engine::generator::difficult dif)
{
    size_t attempts = engine::generator::ATTEMPTS_COUNT;
    engine::board::grid_t g;
    while ((gen.difficulty() != dif) && (attempts > 0)) {
        --attempts;
        g = gen.generate();
    }
    return g;
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

TEST(sudoku_generator, easy)
{
    engine::generator gen;

    const engine::board::grid_t gen_grid = generate(gen, engine::generator::difficult::EASY);
    EXPECTED(! engine::solver::is_solved(gen_grid))
        << "Generated grid:" << std::endl << print(gen_grid) << std::endl;

    EXPECTED(engine::solver::can_solve(gen_grid));

    EXPECTED(gen.solutions_count() == 1)
        << "solutions count: " << gen.solutions_count() << std::endl
        << "Generated grid:" << std::endl << print(gen_grid) << std::endl;
    EXPECTED(gen.difficulty() == engine::generator::difficult::EASY)
        << gen.difficulty_str() << std::endl
        << "Generated grid:" << std::endl << print(gen_grid) << std::endl;
}

TEST(sudoku_generator, easy_repeat)
{
    engine::generator gen;

    for (size_t i = 0; i < std::numeric_limits<char>::max(); ++i) {
        const engine::board::grid_t gen_grid = generate(gen, engine::generator::difficult::EASY);
        EXPECTED(! engine::solver::is_solved(gen_grid))
            << "Generated grid:" << std::endl << print(gen_grid) << std::endl;

        EXPECTED(engine::solver::can_solve(gen_grid));

        EXPECTED(gen.solutions_count() == 1)
            << "solutions count: " << gen.solutions_count() << std::endl
            << "Generated grid:" << std::endl << print(gen_grid) << std::endl;
        EXPECTED(gen.difficulty() == engine::generator::difficult::EASY)
            << gen.difficulty_str() << std::endl
            << "Generated grid:" << std::endl << print(gen_grid) << std::endl;
    }
}

TEST(sudoku_generator, medium)
{
    engine::generator gen;

    const engine::board::grid_t gen_grid = generate(gen, engine::generator::difficult::MEDIUM);
    EXPECTED(! engine::solver::is_solved(gen_grid))
        << "Generated grid:" << std::endl << print(gen_grid) << std::endl;

    EXPECTED(engine::solver::can_solve(gen_grid));

    EXPECTED(gen.solutions_count() == 1)
        << "solutions count: " << gen.solutions_count() << std::endl
        << "Generated grid:" << std::endl << print(gen_grid) << std::endl;
    EXPECTED(gen.difficulty() == engine::generator::difficult::MEDIUM)
        << gen.difficulty_str() << std::endl
        << "Generated grid:" << std::endl << print(gen_grid) << std::endl;
}

TEST(sudoku_generator, very_hard)
{
    engine::generator gen;

    const engine::board::grid_t gen_grid = generate(gen, engine::generator::difficult::VERY_HARD);
    EXPECTED(! engine::solver::is_solved(gen_grid))
        << "Generated grid:" << std::endl << print(gen_grid) << std::endl;

    EXPECTED(engine::solver::can_solve(gen_grid));

    EXPECTED(gen.difficulty() == engine::generator::difficult::VERY_HARD)
        << gen.difficulty_str() << std::endl
        << "Generated grid:" << std::endl << print(gen_grid) << std::endl;
}

int main()
{
    return RUN_TESTS();
}

