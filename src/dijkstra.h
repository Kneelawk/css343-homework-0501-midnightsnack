//
// Created by cyan on 4/27/25.
//

#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <functional>
#include <limits.h>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>

namespace dijkstra {
    inline char getAt(const std::string &field, const size_t width, const size_t x, const size_t y) {
        // width + 1 because we never removed the \n characters
        return field[x + (width + 1) * y];
    }

    inline void setAt(std::string &field, const size_t width, const size_t x, const size_t y, const char c) {
        field[x + (width + 1) * y] = c;
    }

    struct Position {
        size_t x;
        size_t y;
        int cost;

        Position(const size_t x, const size_t y, const int cost)
            : x(x),
              y(y),
              cost(cost) {
        }

        friend bool operator==(const Position &lhs, const Position &rhs) {
            return lhs.x == rhs.x
                   && lhs.y == rhs.y;
        }

        friend bool operator!=(const Position &lhs, const Position &rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const Position &lhs, const Position &rhs) {
            if (lhs.x < rhs.x)
                return true;
            if (rhs.x < lhs.x)
                return false;
            return lhs.y < rhs.y;
        }

        friend bool operator<=(const Position &lhs, const Position &rhs) {
            return !(rhs < lhs);
        }

        friend bool operator>(const Position &lhs, const Position &rhs) {
            return rhs < lhs;
        }

        friend bool operator>=(const Position &lhs, const Position &rhs) {
            return !(lhs < rhs);
        }
    };

    struct Node {
        Position pos;
        int totalCost;
        Node *prev = nullptr;
        std::vector<Node *> next;

        Node(const Position &pos, const int total_cost, Node *const prev, const std::vector<Node *> &next)
            : pos(pos),
              totalCost(total_cost),
              prev(prev),
              next(next) {
        }

        ~Node() {
            // we delete from the root
            for (const auto n: next) {
                delete n;
            }
        }

        void print(std::string &field, const size_t width, const char marker) const {
            auto node = this;
            while (node != nullptr) {
                setAt(field, width, node->pos.x, node->pos.y, marker);
                node = node->prev;
            }
        }

        bool parentsContain(const Position &pos) const {
            auto node = this;
            while (node != nullptr) {
                if (node->pos == pos) {
                    return true;
                }
                node = node->prev;
            }
            return false;
        }
    };

    struct NodeComparator {
        bool operator()(const Node *a, const Node *b) const {
            return a->totalCost > b->totalCost;
        }
    };

    class Solver {
        const std::string field;
        size_t width_;
        size_t height_;
        const std::map<char, int> costs;
        const char endChar;
        const char marker;
        const std::vector<std::function<bool(size_t, size_t)> > movesetChecks;
        const std::vector<std::function<void(size_t &, size_t &)> > moveset;
        const size_t movesetCount;

        Node *root;
        std::priority_queue<Node *, std::vector<Node *>, NodeComparator> next;
        std::set<Position> nextSet;
        std::set<Position> lookedAt;

        Node *found = nullptr;

        void addAroundCur(Node *cur, std::vector<Node *> &found) {
            for (size_t moveI = 0; moveI < movesetCount; moveI++) {
                size_t nextX = cur->pos.x;
                size_t nextY = cur->pos.y;

                if (movesetChecks[moveI](nextX, nextY)) {
                    moveset[moveI](nextX, nextY);
                    char there = getAt(field, width_, nextX, nextY);

                    if (costs.find(there) != costs.end()) {
                        const int cost = costs.at(there);
                        const Position nextPos{nextX, nextY, cost};

                        if (lookedAt.find(nextPos) == lookedAt.end()) {
                            auto nextNode = new Node(nextPos, cur->totalCost + cost, cur, {});
                            cur->next.push_back(nextNode);
                            next.push(nextNode);
                            nextSet.insert(nextPos);

                            if (there == endChar) {
                                found.push_back(nextNode);
                            }
                        }
                    }
                }
            }
        }

    public:
        Solver(const std::string &input, const std::map<char, int> &costs, const char start, const char end,
               const char marker, const std::vector<std::function<bool(size_t, size_t)> > &movesetChecks,
               const std::vector<std::function<void(size_t &, size_t &)> > &moveset)
            : field(input),
              costs(costs),
              endChar(end),
              marker(marker),
              movesetChecks(movesetChecks),
              moveset(moveset),
              movesetCount(moveset.size()) {
            size_t width1 = 0, width2 = 0, height1 = 1;
            size_t startX = 0, startY = 0;
            const size_t inputSize = input.size();
            for (size_t i = 0; i < inputSize; i++) {
                const char ch = input[i];
                if (ch == start) {
                    startX = width2;
                    startY = height1 - 1;
                }
                if (ch == '\n') {
                    if (width1 == 0) {
                        width1 = width2;
                    } else if (width2 != width1) {
                        std::cout << "width1: " << width1 << ", width2: " << width2 << std::endl;
                        throw std::invalid_argument("input string lines have different lengths");
                    }
                    width2 = 0;
                    height1++;
                } else {
                    width2++;
                }
            }
            if (width2 != width1) {
                throw std::invalid_argument("input string last line has different length from other lines");
            }

            std::cout << "startx: " << startX << ", startY: " << startY << std::endl;

            width_ = width1;
            height_ = height1;
            this->root = new Node(Position(startX, startY, 0), 0, nullptr, {});

            if (movesetChecks.size() != movesetCount) {
                throw std::invalid_argument("different number of movesets than moveset checks");
            }
        }

        ~Solver() {
            delete root;
        }

        std::string print() const {
            std::string newField = field;

            if (found == nullptr) {
                for (const auto &pos: nextSet) {
                    setAt(newField, width_, pos.x, pos.y, marker);
                }
            } else {
                found->print(newField, width_, marker);
            }

            return newField;
        }

        bool step() {
            if (this->found != nullptr) {
                return true;
            }

            std::vector<Node *> found;

            if (next.empty()) {
                addAroundCur(root, found);
            } else {
                Node *node = next.top();
                const int curCost = node->totalCost;
                while (!next.empty()) {
                    // try to do everything with the same cost in a single step
                    node = next.top();
                    if (node->totalCost != curCost) {
                        break;
                    }

                    next.pop();

                    // only consider the highest-priority paths to each position
                    if (nextSet.find(node->pos) != nextSet.end()) {
                        nextSet.erase(node->pos);
                        lookedAt.insert(node->pos);

                        addAroundCur(node, found);
                    }
                }
            }

            if (!found.empty()) {
                int minCost = INT_MAX;
                Node *minNode = nullptr;
                for (const auto n: found) {
                    if (n->totalCost < minCost) {
                        minCost = n->totalCost;
                        minNode = n;
                    }
                }

                this->found = minNode;

                return true;
            }

            return false;
        }

        size_t width() const {
            return width_;
        }

        size_t height() const {
            return height_;
        }

        Node *finalPath() const {
            return found;
        }
    };
}

#endif //DIJKSTRA_H
