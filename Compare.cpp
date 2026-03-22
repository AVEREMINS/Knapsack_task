#include "Compare.h"

bool betterState(const State& a, const State& b) {
    // сначала сравниваем по общей ценности два решения
    if (a.totalValue != b.totalValue) { return a.totalValue > b.totalValue;}
    return a.totalWeight < b.totalWeight;
}