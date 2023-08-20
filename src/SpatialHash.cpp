#include "SpatialHash.h"

#include <iostream>



SpatialHash::SpatialHash()
{

}

SpatialHash::~SpatialHash()
{
    for (int i = 0; i < size; i++)
        delete [] grid[i];
    delete [] grid;
    delete [] nodes;
}

void SpatialHash::init(int count, int size)
{
    this->size = size;

    nodes = new Node[count];
    for (int i = 0; i < count; i++)
        nodes[i] = { i, -1, -1, nullptr, nullptr };

    grid = new Node**[size];
    for (int i = 0; i < size; i++) {
        grid[i] = new Node*[size];
        for (int j = 0; j < size; j++)
            grid[i][j] = nullptr;
    }
}

void SpatialHash::mapToGrid(int i, Vector2 pos)
{
    Node& node = nodes[i];
    int row, col;
    rowColHash(pos, row, col);
    
    // if particle has moved cells
    if (node.row != row || node.col != col) {

        // set prev nodes next ptr to null
        if (node.prev == nullptr) {
            if (node.row != -1 && node.col != -1)
                grid[node.row][node.col] = nullptr;
        }
        else node.prev->next = nullptr;

        // set null pointer for previous node
        node.prev = nullptr;

        // set next node ptr to current row/col cells head
        node.next = grid[row][col];

        // set grid[row][col] to point to node
        grid[row][col] = &node;

        // set new row and col of node
        node.row = row;
        node.col = col;

    }
}

// NOTE: will implement another way to deal with out of bounds
void SpatialHash::rowColHash(Vector2 pos, int& row, int& col)
{
    row = (pos.y < size*2) ? pos.y/2.0f : size-1;
    col = (pos.x < size*2) ? pos.x/2.0f : size-1;
}