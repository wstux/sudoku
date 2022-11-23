LibTarget(sudoku_engine STATIC
    HEADERS
        board.h
        generator.h
        solver.h
        details/utils.h
    SOURCES
        details/board.cpp
        details/generator.cpp
        details/solver.cpp
        details/utils.cpp
    INCLUDE_DIR libs
)
