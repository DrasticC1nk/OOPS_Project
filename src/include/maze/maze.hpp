#ifndef MAZE_HPP
#define MAZE_HPP

#include <vector>
#include <string>

struct Point2D 
{
    int x, y;

    Point2D(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
};
 
class Maze2D 
{
private:

    int rows, cols;
    
    std::vector<std::vector<int>> grid;

public:

    explicit Maze2D(const std::string& filename);

    bool isValidMove(int x, int y) const;

    int getRows() const;
    int getCols() const;

    void printMaze() const;

    const std::vector<std::vector<int>>& getGrid() const { return grid; }
};

class Path1D 
{
private:

    std::vector<int> moves;

public:

    explicit Path1D(const std::string& filename);

    bool isValidPath(const Maze2D& maze, std::vector<Point2D>& pathTrace) const;
};

#endif
