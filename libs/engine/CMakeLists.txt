LibTarget(sudoku_engine STATIC
    HEADERS
        board.h
        board_view.h
        generator.h
        solver.h
        details/utils.h
    SOURCES
        details/board.cpp
        details/board_view.cpp
        details/generator.cpp
        details/solver.cpp
        details/utils.cpp
    INCLUDE_DIR libs
)
