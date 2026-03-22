#include "RandomStart.h"
#include <random>

using namespace std;

State randomStart(const vector<Item>& items, int maxWeight) {
    int n = items.size();
    State s;
    s.take.assign(n, 0);
    s.totalValue = 0;
    s.totalWeight = 0;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 1);

    for (int i = 0; i < n; i++) {
        if (dist(gen)) {
            if (s.totalWeight + items[i].weight <= maxWeight) {
                s.take[i] = 1;
                s.totalWeight += items[i].weight;
                s.totalValue += items[i].value;
            }
        }
    }
    return s;
}