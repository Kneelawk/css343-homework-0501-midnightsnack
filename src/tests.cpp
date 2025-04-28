//
// Created by cyan on 4/21/25.
//

#include "dijkstra.h"
#include "input.h"
#include "ktest.hpp"

KTEST(solver_construct) {
    const std::string input = "######\n"
                              "#F   #\n"
                              "#    #\n"
                              "#   %#\n"
                              "######";

    dijkstra::Solver solver(input, costs, start, end, '.', movesetChecks, moveset);

    // we assert nothing throws or crashes
}

KTEST(solver_bad_input) {
    const std::string input = "######\n"
                              "#F   #\n"
                              "#   #\n"
                              "#   %#\n"
                              "######";

    KASSERT_THROWS(std::invalid_argument, [&], {
        dijkstra::Solver solver(input, costs, start, end, '.', movesetChecks, moveset);
    });
}

KTEST(solver_bad_last_input) {
    const std::string input = "######\n"
                              "#F   #\n"
                              "#    #\n"
                              "#   %#\n"
                              "#####";

    KASSERT_THROWS(std::invalid_argument, [&], {
        dijkstra::Solver solver(input, costs, start, end, '.', movesetChecks, moveset);
    });
}

KTEST(solver_bad_movesets) {
    const std::string input = "######\n"
                              "#F   #\n"
                              "#    #\n"
                              "#   %#\n"
                              "######";

    KASSERT_THROWS(std::invalid_argument, [&], {
        dijkstra::Solver solver(input, costs, start, end, '.', {}, moveset);
    });
}

KTEST(solver_solve) {
    const std::string input = "######\n"
                              "#F   #\n"
                              "#    #\n"
                              "#   %#\n"
                              "######";

    dijkstra::Solver solver(input, costs, start, end, '.', movesetChecks, moveset);

    int res = 0;
    while (!((res = solver.step()))) {
        KASSERT_EQ(dijkstra::SOLVE_INCOMPLETE, res) << "solver.step() should return SOLVE_INCOMPLETE(0)";
    }

    KASSERT_EQ(dijkstra::SOLVE_COMPLETE, res) << "final solver.step() should return SOLVE_COMPLETE(1)";

    KASSERT_EQ(dijkstra::SOLVE_COMPLETE, solver.step()) << "calling solver.step() after completion should return SOLVE_COMPLETE(1)";
}

KTEST(solver_no_solution) {
    const std::string input = "######\n"
                              "#F   #\n"
                              "#    #\n"
                              "#    #\n"
                              "######";

    dijkstra::Solver solver(input, costs, start, end, '.', movesetChecks, moveset);

    int res = 0;
    while (!((res = solver.step()))) {
        KASSERT_EQ(dijkstra::SOLVE_INCOMPLETE, res) << "solver.step() should return SOLVE_INCOMPLETE(0)";
    }

    KASSERT_EQ(dijkstra::SOLVE_NO_SOLUTION, res) << "final solver.step() should return SOLVE_NO_SOLUTION(2)";

    KASSERT_EQ(dijkstra::SOLVE_NO_SOLUTION, solver.step()) << "calling solver.step() after completion should return SOLVE_NO_SOLUTION(2)";
}
