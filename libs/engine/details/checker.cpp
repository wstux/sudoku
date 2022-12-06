#include <cassert>
#include <functional>

#include "engine/solver.h"
#include "engine/details/checker.h"
#include "engine/details/utils.h"

namespace engine {
namespace details {

checker::checker()
{
    for (size_t i = 0; i < m_rand_board_idx.size(); ++i) {
        m_rand_board_idx[i] = i;
    }
    shaffle_array(m_rand_board_idx);
}

checker::log_item& checker::add_item(const board::tag_t t)
{
    if (! m_log.empty()) {
        if (m_log.top().tag != t) {
            log_item item;
            item.tag = t;
            item.is_easy = m_log.top().is_easy;
            item.is_medium = m_log.top().is_medium;
            item.is_hard = m_log.top().is_hard;
            item.is_very_hard = m_log.top().is_very_hard;
            m_log.push(item);
        }
    } else {
        m_log.push(log_item());
        m_log.top().tag = t;
    }

    return m_log.top();
}

void checker::add_easy_item(const board::tag_t t)
{
    log_item& item = add_item(t);
    item.is_easy = true;
}

void checker::add_hard_item(const board::tag_t t)
{
    log_item& item = add_item(t);
    item.is_hard = true;
}

void checker::add_medium_item(const board::tag_t t)
{
    log_item& item = add_item(t);
    item.is_medium = true;
}

void checker::add_very_hard_item(const board::tag_t t)
{
    log_item& item = add_item(t);
    item.is_very_hard = true;
}

} // namespace details
} // namespace engine

