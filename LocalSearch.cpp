#include "LocalSearch.h"
#include "Moves.h"
using namespace std;

void localSearch(State& s, const vector<Item>& items, int maxWeight) {
    bool improved = true;

    while (improved) {
        improved = false;

        if (oneBitMove(s, items, maxWeight)) improved = true;
        if (swapMove(s, items, maxWeight)) improved = true;
    }
}