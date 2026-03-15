#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

struct Item {
    int value;
    int weight;
};
struct State { // наше решение
    vector<int> take; // битовый массив для Хэмминга
    long long totalValue;
    long long totalWeight;
};
// Метод Хэмминга это как раз рассмотрение соседа в этом побитовом массиве в поискам разницы лишь в одном бите
State greedyStart(const vector<Item>& items, int maxWeight) {//выгода это соотношение ценности к весу
    int n = items.size();
    vector<int> indices(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }

    // сортируем предметы по выгодности value/weight
    sort(indices.begin(), indices.end(), [&](int first, int second) {
        double ratioFirst = (double)items[first].value / items[first].weight;
        double ratioSecond = (double)items[second].value / items[second].weight;
        if (ratioFirst != ratioSecond) {return ratioFirst > ratioSecond;}
        return items[first].weight < items[second].weight;
    });

    State start; //начальное решение
    start.take.assign(n, 0);
    start.totalValue = 0;
    start.totalWeight = 0;
    //заполняем его
    for (int itemIndex : indices) {
        if (start.totalWeight + items[itemIndex].weight <= maxWeight) {
            start.take[itemIndex] = 1;
            start.totalWeight += items[itemIndex].weight;
            start.totalValue += items[itemIndex].value;
        }
    }
    return start;
}

State randomStart(const vector<Item>& items, int maxWeight, mt19937& gen) {
    int n = items.size();
    vector<int> indices(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }
    // перемешиваем предметы в случайном порядке
    // каждый раз стартовое решение будет строиться по разному
    shuffle(indices.begin(), indices.end(), gen);
    State start;
    start.take.assign(n,0); // пока что все нули
    start.totalValue = 0;
    start.totalWeight = 0;
    // идём по случайному порядку и берём всё, что помещается
    for (int itemIndex : indices) {
        if (start.totalWeight + items[itemIndex].weight <= maxWeight) {
            start.take[itemIndex] = 1;
            start.totalWeight += items[itemIndex].weight;
            start.totalValue += items[itemIndex].value;
        }
    }
    return start;
}

bool betterState(const State& first, const State& second) {
    // сначала сравниваем по общей ценности два решения
    if (first.totalValue != second.totalValue) {return first.totalValue > second.totalValue;}
    // если ценность одинаковая, лучше то решение, где вес меньше
    return first.totalWeight < second.totalWeight;
}
//меняем один бит и надеемся , что найдем лучше решение
bool oneBitMove(State& currentState, const vector<Item>& items, int maxWeight){ 
    int n = items.size();
    State bestState = currentState;
    bool improvementFound = false;
    // пробуем изменить каждый бит в маске решения
    for (int itemIndex = 0; itemIndex < n; itemIndex++) {
        State newState = currentState;
        // если предмет уже взят, пробуем его убрать
        if (newState.take[itemIndex] == 1) {
            newState.take[itemIndex] = 0;
            newState.totalWeight -= items[itemIndex].weight;
            newState.totalValue -= items[itemIndex].value;
        }
        // если предмет не взят, пробуем его добавить
        else {
            if (newState.totalWeight + items[itemIndex].weight > maxWeight) {
                continue;
            }
            newState.take[itemIndex] = 1;
            newState.totalWeight += items[itemIndex].weight;
            newState.totalValue += items[itemIndex].value;
        }
        // если новое состояние лучше, запоминаем его
        if (betterState(newState, bestState)) {
            bestState = newState;
            improvementFound = true;
        }
    }
    // если нашли улучшение, заменяем текущее состояние
    if (improvementFound) {currentState = bestState;}
    return improvementFound;
}
bool swapMove(State& currentState, const vector<Item>& items, int maxWeight){ //попробуй улучшить заменой, а не как выше
    int n = items.size();
    State bestState = currentState;
    bool improvementFound = false;
    // выбираем предмет, который сейчас лежит в рюкзаке
    for (int removeIndex = 0; removeIndex < n; removeIndex++) {
        if (currentState.take[removeIndex] == 0) {
            continue;
        }
        // выбираем предмет, который сейчас не лежит в рюкзаке
        for (int addIndex = 0; addIndex < n; addIndex++) {
            if (currentState.take[addIndex] == 1) {
                continue;
            }
            long long newWeight = currentState.totalWeight - items[removeIndex].weight + items[addIndex].weight;
            long long newValue = currentState.totalValue - items[removeIndex].value + items[addIndex].value;
            // если после замены рюкзак становится слишком тяжёлым, это плохо
            if (newWeight > maxWeight) {
                continue;
            }
            State newState = currentState;
            newState.take[removeIndex] = 0;
            newState.take[addIndex] = 1;
            newState.totalWeight = newWeight;
            newState.totalValue = newValue;
            if (betterState(newState, bestState)) {
                bestState = newState;
                improvementFound = true;
            }
        }
    }
    if (improvementFound) {
        currentState = bestState;
    }
    return improvementFound;
}

State localSearch(State currentState, const vector<Item>& items, int maxWeight){//многократно улучшает решение 
    //пробует функции oneBitMove и swapMove
    bool changed = true;
    while (changed) {
        changed = false;
        if (oneBitMove(currentState, items, maxWeight)) {
            changed = true;
        }
        if (swapMove(currentState, items, maxWeight)) {
            changed = true;
        }
    }
    return currentState;
}

int main() {
    int n;
    int maxWeight;
    cin >> n >> maxWeight;
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        cin >> items[i].value >> items[i].weight;
    }
    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();
    
    random_device randomSeed;
    mt19937 gen(randomSeed()); // вычитал это в интернете, как генератор случайных чисел
    State bestState = greedyStart(items, maxWeight);
    bestState = localSearch(bestState, items, maxWeight);

    int startCount = 50;// дополнительные запуски для случайных решений на старте
    for (int i = 0; i < startCount; i++) {
        State currentState = randomStart(items, maxWeight, gen);
        currentState = localSearch(currentState, items, maxWeight);
        if (betterState(currentState, bestState)) {
            bestState = currentState;
        }
    }

    chrono::steady_clock::time_point finishTime = chrono::steady_clock::now();
    double elapsed = chrono::duration<double>(finishTime - startTime).count();

    cout << "Best value: " << bestState.totalValue << "\n";
    cout << "Total weight: " << bestState.totalWeight << "\n";
    cout << "Bit array:\n";
    for (int i = 0; i < n; i++) {
        cout << bestState.take[i];
        if (i + 1 < n) {
            cout << ' ';
        }
    }
    cout << "\n";

    cout << "Chosen items:\n";
    bool firstItem = true;
    for (int i = 0; i < n; i++) {
        if (bestState.take[i] == 1) {
            if (!firstItem) {
                cout << ' ';
            }
            cout << i + 1;
            firstItem = false;
        }
    }
    cout << "\n";
    cout << "Time: " << elapsed << "\n";
    return 0;
}