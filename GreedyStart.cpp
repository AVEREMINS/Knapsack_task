#include <algorithm>
#include "GreedyStart.h"
using namespace std;

State greedyStart(const vector<Item>& items, int maxWeight) {
    int n = items.size();
    vector<int> order(n);
    for (int i = 0; i < n; i++) order[i] = i;
    sort(order.begin(), order.end(), [&](int a, int b) {
        double ra = (double)items[a].value / items[a].weight;
        double rb = (double)items[b].value / items[b].weight;
        return ra > rb;
    });
    State s;
    s.take.assign(n, 0);
    s.totalValue = 0;
    s.totalWeight = 0;
    for (int id : order) {
        if (s.totalWeight + items[id].weight <= maxWeight) {
            s.take[id] = 1;
            s.totalWeight += items[id].weight;
            s.totalValue += items[id].value;
        }
    }
    return s;
} //немного измений версию , если сравнивать с ОГРОМНОЙ программой (но в основном ничего нового)