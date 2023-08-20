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
    void rowColHash(Vector2 pos, int& row, int& col);

    struct Node {
        int index;
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

};