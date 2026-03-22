#include "Moves.h"
#include "Compare.h"

using namespace std;

bool oneBitMove(State& s, const vector<Item>& items, int maxWeight) {
    int n = items.size();
    bool improved = false;
    // пробуем изменить каждый бит в маске решения
    for (int i = 0; i < n; i++) {
        State candidate = s;
        // если предмет уже взят, пробуем его убрать
        if (candidate.take[i]) {
            candidate.take[i] = 0;
            candidate.totalWeight -= items[i].weight;
            candidate.totalValue -= items[i].value;
        } else { // если предмет не взят, пробуем его добавить
            if (candidate.totalWeight + items[i].weight <= maxWeight) {
                candidate.take[i] = 1;
                candidate.totalWeight += items[i].weight;
                candidate.totalValue += items[i].value;
            } else continue;
        }
    // если новое состояние лучше, запоминаем его
        if (betterState(candidate, s)) {
            s = candidate;
            improved = true;
        }
    }

    return improved;
}

bool swapMove(State& s, const vector<Item>& items, int maxWeight) {
    int n = items.size();
    bool improved = false;
    // выбираем предмет, который сейчас лежит в рюкзаке
    for (int i = 0; i < n; i++) {
        if (!s.take[i]) continue;
        // выбираем предмет, который сейчас не лежит в рюкзаке
        for (int j = 0; j < n; j++) {
            if (s.take[j]) continue;
            State candidate = s;
            candidate.take[i] = 0;
            candidate.take[j] = 1;
            candidate.totalWeight = s.totalWeight-items[i].weight+items[j].weight;
            candidate.totalValue = s.totalValue-items[i].value+items[j].value;
            if (candidate.totalWeight <= maxWeight && betterState(candidate, s)) {
                s = candidate;
                improved = true;
            }
        }
    }
    return improved;
}