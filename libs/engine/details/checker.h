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

    difficult difficulty() const;

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

    size_t random_pos(size_t p) const { return m_rand_board_idx[p]; }

    void reset();

    void rollback_to_tag(board& b, const board::tag_t t);

    void set_guess_value(board& b, const size_t p, const board::value_t v, const board::tag_t t);

    bool solve_single(board& b, const board::tag_t t);
    bool solve_single_cell(board& b, const board::tag_t t);
    bool solve_single_value_col(board& b, const board::tag_t t);
    bool solve_single_value_row(board& b, const board::tag_t t);
    bool solve_single_value_section(board& b, const board::tag_t t);

private:
    random_indices_t m_rand_board_idx;

    std::stack<log_item> m_log;
};

} // namespace details
} // namespace engine

