#include <chrono>
#include <iostream>
#include <vector>
#include "Compare.h"
#include "GreedyStart.h"
#include "Item.h"
#include "LocalSearch.h"
#include "RandomStart.h"
#include "SimpleValueStart.h"
#include "State.h"

using namespace std;

int main() {
    int n;
    int maxWeight;
    cin >> n >> maxWeight;
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {cin >> items[i].value >> items[i].weight;}

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();
    State best = simpleValueStart(items, maxWeight);
    State greedy = greedyStart(items, maxWeight);
    if (betterState(greedy, best)) {best = greedy;}

    State improvedGreedy = greedy;
    localSearch(improvedGreedy, items, maxWeight);
    if (betterState(improvedGreedy, best)) {best = improvedGreedy;}

    for (int it = 0; it < 10; it++) {
        State current = randomStart(items, maxWeight);
        localSearch(current, items, maxWeight);
        if (betterState(current, best)) {best = current;}
    }

    chrono::steady_clock::time_point finishTime = chrono::steady_clock::now();
    double passed = chrono::duration<double>(finishTime - startTime).count();
    cout << "Best value: " << best.totalValue << "\n";
    cout << "Total weight: " << best.totalWeight << "\n";
    cout << "Mask Hamming:\n";
    for (int i = 0; i < n; i++) {
        cout << best.take[i];
        if (i + 1 < n) { cout << ' ';}
    }
    cout << "\n";
    cout << "Chosen items:\n";
    for (int i = 0; i < n; i++) {
        if (best.take[i] == 1) {
            cout << i + 1 << '\n';
        }
    }
    cout << "\n";
    cout << "Time: " << passed << "\n";
    return 0;
}