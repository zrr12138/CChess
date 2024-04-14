//
// Created by zrr on 24-4-14.
//
#include <vector>
#include <random>
#include <numeric>

#ifndef CCHESS_WEIGHTEDRANDOMSELECTOR_H
#define CCHESS_WEIGHTEDRANDOMSELECTOR_H

class WeightedRandomSelector {
private:
    struct Node {
        Node *left;
        Node *right;
        int weight;

        Node() : left(nullptr), right(nullptr), weight(0) {}

        ~Node() {
            delete left;
            delete right;
        }
    };

public:

    WeightedRandomSelector(int *arr, int size);

    ~WeightedRandomSelector();

    void UpdateWeight(int index, int weight);

    int getRandomIndex();

private:
    Node *root;
    int *weights;
    int size;
    std::minstd_rand generator{std::random_device{}()};
    std::uniform_real_distribution<double> distribution;
    double totalWeight;

    Node *build(int l, int r);
    void update(Node *ptr, int l, int r, int index, int weight);
    static inline void up(Node *ptr) {
        ptr->weight = ptr->left->weight + ptr->right->weight;
    }
    int getIndex(WeightedRandomSelector::Node *ptr, int l, int r);

};

#endif //CCHESS_WEIGHTEDRANDOMSELECTOR_H
