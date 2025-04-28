//
// Created by cyan on 4/27/25.
//

#ifndef INPUT_H
#define INPUT_H

#include <functional>
#include <string>
#include <map>
#include <stdint.h>
#include <vector>

const std::map<char, int> costs = {{' ', 1}, {'Y', 9}, {'%', 1}};
constexpr char start = 'F';
constexpr char end = '%';
const std::vector<std::function<bool(size_t, size_t)> > movesetChecks = {
    [](const size_t x, size_t) {
        return x < SIZE_MAX;
    },
    [](size_t, const size_t y) {
        return y < SIZE_MAX;
    },
    [](const size_t x, size_t) {
        return x > 0;
    },
    [](size_t, const size_t y) {
        return y > 0;
    }
};
const std::vector<std::function<void(size_t &, size_t &)> > moveset = {
    [](size_t &x, size_t &) {
        x += 1;
    },
    [](size_t &, size_t &y) {
        y += 1;
    },
    [](size_t &x, size_t &) {
        x -= 1;
    },
    [](size_t &, size_t &y) {
        y -= 1;
    }
};

const std::string input = "###############################\n"
        "#F    YYYY      ##  ##  ###   #\n"
        "# ##  ####   #####  ##  ##    #\n"
        "# ##  ####   #####            #\n"
        "# ##  ####   #####  ###  #### #\n"
        "#        YYYY       ###  #### #\n"
        "# #######YYYY#####  ###YY#### #\n"
        "# #######YYYY#####  ###YY#### #\n"
        "# ##         #####     YY     #\n"
        "#            #####  ######    #\n"
        "# ####              ##% ##    #\n"
        "# ####YYY###  ####  ##  ##  ###\n"
        "#     YYY###        ##        #\n"
        "###############################";

#endif //INPUT_H
