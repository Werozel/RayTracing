#include <random>

int randrange(const int &start, const int &end) {
    return rand() % end + start;
}