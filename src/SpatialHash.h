#pragma once

#include <raylib.h>
#include <vector>


class SpatialHash
{
 public:

    SpatialHash();
    ~SpatialHash();

    void init(int count, int size);

    void mapToGrid(int i, Vector2 pos);

    struct Node {
        int row;
        int col;
        Node* prev;
        Node* next;
    };

    Node* nodes;
    Node*** grid;

 private:

    int count;
    int size;

    void rowColHash(Vector2 pos, int& row, int& col);

};