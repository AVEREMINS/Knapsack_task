#pragma once
#include <vector>
#include "Item.h"
#include "State.h"

State greedyStart(const std::vector<Item>& items, int maxWeight);