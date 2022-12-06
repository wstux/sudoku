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

std::string checker::difficult_to_str(const difficult d)
{
    return generator::difficult_to_str(d);
}

checker::difficult checker::difficulty() const
{
    if (m_log.empty()) {
        return difficult::INVALID;
    }

    const log_item& item = m_log.top();
    if (item.is_very_hard) {
        return difficult::VERY_HARD;
    } else if (item.is_hard) {
        return difficult::HARD;
    } else if (item.is_medium) {
        return difficult::MEDIUM;
    } else if (item.is_easy) {
        return difficult::EASY;
    }
    return difficult::INVALID;
}

bool checker::solve_single(board& b, const board::tag_t t)
{
    if (solve_single_cell(b, t))          { return true; }
    if (solve_single_value_col(b, t))     { return true; }
    if (solve_single_value_row(b, t))     { return true; }
    if (solve_single_value_section(b, t)) { return true; }
    return false;
}

bool checker::solve_single_cell(board& b, const board::tag_t t)
{
    if (details::solve_single_cell(b, t)) {
        add_easy_item(t);
        return true;
    }
    return false;
}

bool checker::solve_single_value_col(board& b, const board::tag_t t)
{
    if (details::solve_single_value_col(b, t)) {
        add_medium_item(t);
        return true;
    }
    return false;
}

bool checker::solve_single_value_row(board& b, const board::tag_t t)
{
    if (details::solve_single_value_row(b, t)) {
        add_medium_item(t);
        return true;
    }
    return false;
}

bool checker::solve_single_value_section(board& b, const board::tag_t t)
{
    if (details::solve_single_value_section(b, t)) {
        add_medium_item(t);
        return true;
    }
    return false;
}

} // namespace details
} // namespace engine

