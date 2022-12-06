#pragma once

#include <cstddef>
#include <array>

#include "engine/board.h"

namespace engine {

class board_view final
{
public:
    using value_t = board::value_t;
    using tag_t = board::tag_t;
    using row_t = board::row_t;
    using grid_t = board::grid_t;

public:
    explicit board_view(grid_t& g);

    const grid_t& grid() const { return *m_p_grid; }

    bool is_set_value(const size_t p) const;

    void set_value(const size_t p, const value_t v);

    value_t value(const size_t p) const;

private:
    static size_t to_col(const size_t p);
    static size_t to_row(const size_t p);

private:
    grid_t* m_p_grid;
};

} // namespace engine

