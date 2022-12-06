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
    checker();

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

private:
    random_indices_t m_rand_board_idx;
    std::stack<log_item> m_log;
};

} // namespace details
} // namespace engine

