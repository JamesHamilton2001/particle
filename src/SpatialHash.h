#pragma once

#include <raylib.h>
#include <vector>


class SpatialHash
{
 public:

    SpatialHash();
    ~SpatialHash();

    void init(int count, int size);

    void mapGrid(int count, Vector2* positions);
    void rowColHash(Vector2 pos, int& r, int& c);

    int** counts;
    std::vector<int>** indexes;

 private:
    
    int size;

};