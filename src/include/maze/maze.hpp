#ifndef MAZE_HPP
#define MAZE_HPP

#include <vector>
#include <string>

using namespace std;

struct Point2D 
{
    int x, y;

    Point2D(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
};
 
class Maze2D 
{
private:

    int rows, cols;
    
    vector<vector<int>> grid;

public:

    explicit Maze2D(const string& filename);

    bool isValidMove(int x, int y) const;

    int getRows() const;
    int getCols() const;

    void printMaze() const;

    const vector<vector<int>>& getGrid() const { return grid; }
};

class Path1D 
{
private:

    vector<int> moves;

public:

    explicit Path1D(const string& filename);

    bool isValidPath(const Maze2D& maze, vector<Point2D>& pathTrace) const;
};

#endif
