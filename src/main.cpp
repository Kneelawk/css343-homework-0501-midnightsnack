#include <iostream>
#include <thread>
#include <chrono>

#include "dijkstra.h"
#include "input.h"
#include "ktest.hpp"

int main(const int argc, char **argv) {
    ktest::runAllTests();

    const bool animated = argc > 1 && !std::strcmp(argv[1], "-a");
    if (animated) {
        std::cout << "Animating output..." << std::endl;
    }

    dijkstra::Solver solver(input, costs, start, end, '.', movesetChecks, moveset);

    std::cout << "Initial:\n" << solver.print() << std::endl;
    std::cout << std::endl;

    bool first = true;

    while (!solver.step()) {
        if (animated && !first) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "\33[" << (solver.height() + 1) << "A" << std::flush;
        }

        std::cout << solver.print() << std::endl;
        std::cout << std::endl;

        first = false;
    }

    std::cout << "Total cost: " << solver.finalPath()->totalCost << std::endl;
    std::cout << "Found:\n" << solver.print() << std::endl;

    return 0;
}
