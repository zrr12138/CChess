//
// Created by zrr on 24-4-14.
//

#include <cassert>
#include "WeightedRandomSelector.h"

WeightedRandomSelector::WeightedRandomSelector(int *arr, int size) : distribution(0.0, 1.0), root(nullptr), size(size) {
    weights = new int[size];
    std::copy(arr, arr + size, weights);
    root = build(0, size);
}

WeightedRandomSelector::Node *WeightedRandomSelector::build(int l, int r) {
    assert(r > l);
    auto ptr = new Node();
    if (r - l == 1) {
        ptr->weight = weights[l];
        return ptr;
    }
    int mid = (l + r) >> 1;
    ptr->left = build(l, mid);
    ptr->right = build(mid, r);
    up(ptr);
    return ptr;
}

WeightedRandomSelector::~WeightedRandomSelector() {
    if (weights) {
        delete[]weights;
    }
    delete root;
    root = nullptr;
}

void WeightedRandomSelector::UpdateWeight(int index, int weight) {
    update(root, 0, size, index, weight);
}

void WeightedRandomSelector::update(Node *ptr, int l, int r, int index, int weight) {
    assert(ptr != nullptr);
    assert(l < r);
    int mid = (l + r) >> 1;
    if (r - l == 1) {
        assert(l == index);
        ptr->weight = weight;
        weights[index] = weight;
        return;
    }
    if (index < mid) {
        update(ptr->left, l, mid, index, weight);
    } else {
        update(ptr->right, mid, r, index, weight);
    }
    up(ptr);
}

int WeightedRandomSelector::getRandomIndex() {
    return getIndex(root, 0, size);
}

int WeightedRandomSelector::getIndex(WeightedRandomSelector::Node *ptr, int l, int r) {
    double val = distribution(generator) * ptr->weight;
    if (r - l == 1) {
        return l;
    }
    int mid = (l + r) >> 1;
    if (val < ptr->left->weight) {
        return getIndex(ptr->left, l, mid);
    } else {
        return getIndex(ptr->right, mid, r);
    }
    return 0;
}


