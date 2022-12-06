#include "engine/board_view.h"
#include "engine/details/utils.h"

namespace engine {

board_view::board_view(grid_t& g)
    : m_p_grid(&g)
{}

bool board_view::is_set_value(const size_t p) const
{
    return ((*m_p_grid)[to_row(p)][to_col(p)] != 0);
}

void board_view::set_value(const size_t p, const value_t v)
{
    (*m_p_grid)[to_row(p)][to_col(p)] = v;
}

size_t board_view::to_col(const size_t p)
{
    return details::col_by_position(p);
}

size_t board_view::to_row(const size_t p)
{
    return details::row_by_position(p);
}

board_view::value_t board_view::value(const size_t p) const
{
    return (*m_p_grid)[to_row(p)][to_col(p)];
}

} // namespace engine

