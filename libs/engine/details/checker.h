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

    void calc(const board::grid_t& g, const size_t limit = 2);
    void calc(const board_view& b, const size_t limit = 2);
    void calc(const board& b, const size_t limit = 2);

    difficult difficulty() const { return m_dif; }

    size_t solutions_count() const { return m_solutions_count; }

    static difficult calc_difficulty(const board::grid_t& g);
    static difficult calc_difficulty(const board_view& b);
    static difficult calc_difficulty(const board& b);

    static size_t calc_solutions(const board::grid_t& g, const size_t limit = 2);
    static size_t calc_solutions(const board_view& b, const size_t limit = 2);
    static size_t calc_solutions(board b, const size_t limit = 2);

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

    difficult calculate_difficulty(board b);

    size_t calculate_solutions(board b, const size_t limit);

    size_t calculate_solutions(board& b, const board::tag_t t, const size_t limit);

    size_t random_pos(size_t p) const { return m_rand_board_idx[p]; }

    void reset();
    void reset_solutions();

    void rollback_to_tag(board& b, const board::tag_t t);

    void set_guess_value(board& b, const size_t p, const board::value_t v, const board::tag_t t);

    bool solve(board& b, const board::tag_t t);

    bool solve_single(board& b, const board::tag_t t);

    bool solve_single_easy(board& b, const board::tag_t t);
    bool solve_single_hard(board& b, const board::tag_t t);
    bool solve_single_medium(board& b, const board::tag_t t);

private:
    random_indices_t m_rand_board_idx;
    std::stack<log_item> m_log;
    difficult m_dif = difficult::INVALID;
    size_t m_solutions_count = 0;
};

} // namespace details
} // namespace engine

