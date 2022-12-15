#include <array>
#include <stack>

#include "engine/board.h"
#include "engine/board_view.h"
#include "engine/generator.h"

namespace engine {
namespace details {

class checker final
{
private:
    using random_indices_t = std::array<size_t, board::BOARD_SIZE>;

public:
    using difficult = generator::difficult;

    checker();

    difficult calc_difficulty(const board::grid_t& g);
    difficult calc_difficulty(const board_view& b);
    difficult calc_difficulty(board b);

    size_t calc_solutions(const board::grid_t& g, const size_t limit = 2);
    size_t calc_solutions(const board_view& b, const size_t limit = 2);
    size_t calc_solutions(board b, const size_t limit = 2);

    difficult difficulty() const { return m_dif; }

    size_t solutions_count() const { return m_solutions_count; }

    static difficult calculate_difficulty(const board::grid_t& g);
    static difficult calculate_difficulty(const board& b);

    static std::string difficult_to_str(const difficult d);

private:
    struct log_item
    {
        board::tag_t tag = board::INVALID_TAG;

        bool is_easy = false;
        bool is_medium = false;
        bool is_hard = false;
        bool is_very_hard = false;
    };

private:
    log_item& add_item(const board::tag_t t);
    void add_easy_item(const board::tag_t t);
    void add_hard_item(const board::tag_t t);
    void add_medium_item(const board::tag_t t);
    void add_very_hard_item(const board::tag_t t);

    size_t calc_solutions(board& b, const board::tag_t t, const size_t limit);

    size_t random_pos(size_t p) const { return m_rand_board_idx[p]; }

    void reset();
    void reset_solutions();

    void rollback_to_tag(board& b, const board::tag_t t);

    void set_guess_value(board& b, const size_t p, const board::value_t v, const board::tag_t t);

    bool solve(board& b, const board::tag_t t);

    bool solve_single(board& b, const board::tag_t t);
    bool solve_single_cell(board& b, const board::tag_t t);
    bool solve_single_value_col(board& b, const board::tag_t t);
    bool solve_single_value_row(board& b, const board::tag_t t);
    bool solve_single_value_section(board& b, const board::tag_t t);

private:
    random_indices_t m_rand_board_idx;
    std::stack<log_item> m_log;
    difficult m_dif = difficult::INVALID;
    size_t m_solutions_count = 0;
};

} // namespace details
} // namespace engine

