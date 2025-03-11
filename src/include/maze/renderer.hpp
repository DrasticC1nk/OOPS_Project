#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "maze.hpp"  

class Renderer 
{
private:

    int windowWidth;
    int windowHeight;

    SDL_Window* window;
    SDL_Renderer* renderer;

public:

    Renderer(int width, int height);

    ~Renderer();

    void drawMaze(const Maze2D& maze);

    void animatePath(const std::vector<std::pair<int, int>>& pathTrace, bool isValid, const Maze2D& maze);

    void drawText(const std::string& text, int x, int y, SDL_Color color); 

    void drawButton(const std::string& label, int x, int y);
};

#endif 
