#pragma once
#include "State.h"
#include "Item.h"

bool oneBitMove(State& s, const std::vector<Item>& items, int maxWeight);
bool swapMove(State& s, const std::vector<Item>& items, int maxWeight);