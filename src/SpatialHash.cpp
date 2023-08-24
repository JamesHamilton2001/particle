#include "SpatialHash.h"

#include <vector>
#include <iostream>



SpatialHash::SpatialHash()
{

}

SpatialHash::~SpatialHash()
{

}

void SpatialHash::init(int count, int size)
{
    this->size = size;
    counts = new int*[size];
    indexes = new std::vector<int>*[size];
    for (int i = 0; i < size; i++) {
        counts[i] = new int[size];
        indexes[i] = new std::vector<int>[size];
        for (int j = 0; j < size; j++) {
            counts[i][j] = 0;
            indexes[i][j] = std::vector<int>(6*count/size, -1);
        }
    }
}

void SpatialHash::mapGrid(int count, Vector2* positions)
{
    // set counts to 0 and indexes to -1
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            for (int i = 0; i <= counts[r][c]; i++)
                indexes[r][c][i] = -1;
            counts[r][c] = 0;
        }
    }

    // for each position, map the index and increment count in the grid
    for (int i = 0, r, c; i < count; i++) {
        std::cout << positions[i].x << std::endl;
        std::cout << positions[i].y << std::endl;
        rowColHash(positions[i], r, c);
        indexes[r][c][counts[r][c]++] = i;
    }
}

// NOTE: will implement another way to deal with out of bounds
void SpatialHash::rowColHash(Vector2 pos, int& r, int& c)
{
    r = (pos.y <= 0) ?  0  :  ((pos.y >= 2*size) ?  size-1  :  pos.y/2);
    c = (pos.x <= 0) ?  0  :  ((pos.x >= 2*size) ?  size-1  :  pos.x/2);
}