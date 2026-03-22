#include "SimpleValueStart.h"
#include <algorithm>

using namespace std;

State simpleValueStart(const vector<Item>& items, int maxWeight) {
    int n = (int)items.size();
    vector<int> order(n);
    for (int i = 0; i < n; i++) { order[i] = i;}

    sort(order.begin(), order.end(), [&](int a, int b) {
        if (items[a].value != items[b].value) {return items[a].value > items[b].value;}
        return items[a].weight < items[b].weight;
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
}