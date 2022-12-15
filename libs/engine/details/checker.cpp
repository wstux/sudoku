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

checker::difficult checker::calc_difficulty(const board::grid_t& g)
{
    return calc_difficulty(board(g));
}

checker::difficult checker::calc_difficulty(const board_view& b)
{
    return calc_difficulty(board(b.grid()));
}

checker::difficult checker::calc_difficulty(board b)
{
    reset();
    m_dif = difficult::INVALID;
    const bool is_solved = solve(b, board::BEGIN_TAG);
    if (is_solved) {
        if (m_log.empty()) {
            m_dif = difficult::INVALID;
        } else {
            const log_item& item = m_log.top();
            if (item.is_very_hard) {
                m_dif = difficult::VERY_HARD;
            } else if (item.is_hard) {
                m_dif = difficult::HARD;
            } else if (item.is_medium) {
                m_dif = difficult::MEDIUM;
            } else if (item.is_easy) {
                m_dif = difficult::EASY;
            } else {
                m_dif = difficult::INVALID;
            }
        }
    }
    return m_dif;
}

checker::difficult checker::calculate_difficulty(const board::grid_t& g)
{
    checker ch;
    return ch.calc_difficulty(g);
}

checker::difficult checker::calculate_difficulty(const board& b)
{
    checker ch;
    return ch.calc_difficulty(b);
}

size_t checker::calc_solutions(const board::grid_t& g, const size_t limit)
{
    return calc_solutions(board(g), limit);
}

size_t checker::calc_solutions(const board_view& b, const size_t limit)
{
    return calc_solutions(board(b.grid()), limit);
}

size_t checker::calc_solutions(board b, const size_t limit)
{
    //const board::grid_t raw_grid = b.grid();

    reset_solutions();
    m_solutions_count = calc_solutions(b, board::BEGIN_TAG, limit);

    //calc_difficulty(raw_grid);
    reset();
    return m_solutions_count;
}

size_t checker::calc_solutions(board& b, const board::tag_t t, const size_t limit)
{
    const board::tag_t single_tag = t + 1;
    while (solve_single(b, single_tag)) {
		if (solver::is_solved(b)) {
			rollback_to_tag(b, t);
			return 1;
		}
		if (solver::is_impossible(b)) {
			rollback_to_tag(b, t);
			return 0;
		}
	}

	size_t solutions_count = 0;
	const board::tag_t guess_tag = single_tag + 1;

	const details::is_set_fn_t is_set_fn =
	    [b](size_t p) -> bool { return b.is_set_value(p); };
    const details::is_poss_fn_t is_poss_fn =
        [b](size_t p, board::value_t v) -> bool { return b.is_possible(p, v); };

    details::guess_t guess = details::find_guess_cell(is_set_fn, is_poss_fn, m_rand_board_idx);
    for (size_t i = 0; i < guess.available.size(); ++i) {
        if (! guess.available[i]) {
            continue;
        }
        assert(guess.available[i]);

        const board::value_t value = i + 1;
        assert(value > 0 && value < 10);

        set_guess_value(b, guess.pos, value, guess_tag);
        solutions_count += calc_solutions(b, guess_tag, limit);
        if (solutions_count >= limit) {
            rollback_to_tag(b, t);
            return solutions_count;
        } else {
            rollback_to_tag(b, single_tag);
        }
    }

    rollback_to_tag(b, t);
    return solutions_count;
}

std::string checker::difficult_to_str(const difficult d)
{
    return generator::difficult_to_str(d);
}

void checker::reset()
{
    while (! m_log.empty()) {
        m_log.pop();
    }
    shaffle_array(m_rand_board_idx);
}

void checker::reset_solutions()
{
    reset();
    m_dif = difficult::INVALID;
    m_solutions_count = 0;
}

void checker::rollback_to_tag(board& b, const board::tag_t t)
{
    b.rollback_to_tag(t);
    while((! m_log.empty()) && (m_log.top().tag != t)) {
        m_log.pop();
    }
}

void checker::set_guess_value(board& b, const size_t p, const board::value_t v, const board::tag_t t)
{
    if (b.set_value(p, v, t)) {
        add_very_hard_item(t);
    }
}

bool checker::solve(board& b, const board::tag_t t)
{
    const board::tag_t single_tag = t + 1;
    while (solve_single(b, single_tag)) {
		if (solver::is_solved(b)) {
			return true;
		}
		if (solver::is_impossible(b)) {
			return false;
		}
	}

	const board::tag_t guess_tag = single_tag + 1;

	const details::is_set_fn_t is_set_fn =
	    [b](size_t p) -> bool { return b.is_set_value(p); };
    const details::is_poss_fn_t is_poss_fn =
        [b](size_t p, board::value_t v) -> bool { return b.is_possible(p, v); };

    details::guess_t guess = details::find_guess_cell(is_set_fn, is_poss_fn, m_rand_board_idx);
    if (! guess.is_valid()) {
        rollback_to_tag(b, t);
        return false;
    }

    assert(guess.available.count() > 0);
    for (size_t i = 0; i < guess.available.size(); ++i) {
        if (! guess.available[i]) {
            continue;
        }
        assert(guess.available[i]);

        const board::value_t value = i + 1;
        assert(value > 0 && value < 10);

        set_guess_value(b, guess.pos, value, guess_tag);
        if (solver::is_impossible(b) || ! solve(b, guess_tag)) {
            rollback_to_tag(b, t);
        } else {
            return true;
        }
    }

    return false;
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

