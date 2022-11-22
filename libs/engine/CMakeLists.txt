LibTarget(sudoku_engine STATIC
    HEADERS
        board.h
        solver.h
        types.h
        details/utils.h
    SOURCES
        details/board.cpp
        details/solver.cpp
        details/utils.cpp
    INCLUDE_DIR libs
)
