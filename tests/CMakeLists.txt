TestTarget(ut_sudoku_board
    SOURCES
        ut_sudoku_board.cpp
    LIBRARIES
        sudoku_engine
)

TestTarget(ut_sudoku_checker
    SOURCES
        ut_sudoku_checker.cpp
    LIBRARIES
        sudoku_engine
)

TestTarget(ut_sudoku_solver
    SOURCES
        ut_sudoku_solver.cpp
    LIBRARIES
        sudoku_engine
)

TestTarget(ut_sudoku_utils
    SOURCES
        ut_sudoku_utils.cpp
    LIBRARIES
        sudoku_engine
)

