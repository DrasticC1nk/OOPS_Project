#include "../src/include/maze/renderer.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <set>
#include <algorithm>
#include <cmath>

Renderer::Renderer(int width, int height) 
{
    windowWidth = width;
    windowHeight = height;

    if(SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        std::cerr << "SDL couldn't be initialized >>> " << SDL_GetError() << std::endl;

        exit(1);
    }

    window = SDL_CreateWindow("Maze Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

    if(!window) 
    {
        std::cerr << "Window couldn't be created >>>" << SDL_GetError() << std::endl;

        SDL_Quit();

        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer) 
    {
        std::cerr << "Renderer couldn't be created >>> " << SDL_GetError() << std::endl;

        SDL_DestroyWindow(window);

        SDL_Quit();

        exit(1);
    }
}

Renderer::~Renderer() 
{
    if(renderer) 
    {
        SDL_DestroyRenderer(renderer);
    }

    if(window) 
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

void Renderer::drawMaze(const Maze2D& maze) 
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int mazeRows = maze.getRows();
    int mazeCols = maze.getCols();

    float cellWidth = static_cast<float>(windowWidth) / mazeCols;
    float cellHeight = static_cast<float>(windowHeight) / mazeRows;

    for(int i = 0; i < mazeRows; ++i) 
    {
        for(int j = 0; j < mazeCols; ++j) 
        {
            int cellX = static_cast<int>(std::floor(j * cellWidth));
            int cellY = static_cast<int>(std::floor(i * cellHeight));

            int cellW = static_cast<int>(std::ceil((j + 1) * cellWidth)) - cellX;
            int cellH = static_cast<int>(std::ceil((i + 1) * cellHeight)) - cellY;

            SDL_Rect cell = { cellX, cellY, cellW, cellH };

            if(!maze.isValidMove(i, j)) 
            {  
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); //GREY
            } 
            else 
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //WHITE
            }

            SDL_RenderFillRect(renderer, &cell);
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 

    for(int j = 1; j < mazeCols; ++j) 
    {
        int x = static_cast<int>(std::round(j * cellWidth));

        SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
    }

    for(int i = 1; i < mazeRows; ++i) 
    {
        int y = static_cast<int>(std::round(i * cellHeight));

        SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
    }

    SDL_RenderPresent(renderer);
}

void Renderer::animatePath(const std::vector<std::pair<int, int>>& pathTrace, bool isValid, const Maze2D& maze) 
{
    int mazeRows = maze.getRows();
    int mazeCols = maze.getCols();

    float cellWidth = static_cast<float>(windowWidth) / mazeCols;
    float cellHeight = static_cast<float>(windowHeight) / mazeRows;

    std::vector<std::pair<int, int>> visited;

    int cellW = static_cast<int>(std::ceil(cellWidth));  
    int cellH = static_cast<int>(std::ceil(cellHeight));

    for(size_t i = 0; i < pathTrace.size(); ++i) 
    {
        int x = pathTrace[i].first;
        int y = pathTrace[i].second;

        int cellX = static_cast<int>(std::floor(y * cellWidth));
        int cellY = static_cast<int>(std::floor(x * cellHeight));

        SDL_Rect cell = { cellX, cellY, cellW, cellH };

        auto it = std::find(visited.begin(), visited.end(), std::make_pair(x, y));

        if(it != visited.end()) 
        {
            while(!visited.empty() && visited.back() != std::make_pair(x, y)) 
            {
                auto last = visited.back();
                visited.pop_back();

                int lastX = static_cast<int>(std::floor(last.second * cellWidth));
                int lastY = static_cast<int>(std::floor(last.first * cellHeight));

                SDL_Rect lastCell = { lastX, lastY, cellW, cellH };

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //WHITE
                SDL_RenderFillRect(renderer, &lastCell);
                SDL_RenderPresent(renderer);
                SDL_Delay(100);
            }
        } 
        else 
        {
            visited.push_back(std::make_pair(x, y));
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //BLUE

        SDL_RenderFillRect(renderer, &cell);

        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    SDL_SetRenderDrawColor(renderer, isValid ? 0 : 255, isValid ? 255 : 0, 0, 255);

    for (const auto& pos : visited) 
    {
        int x = pos.first;
        int y = pos.second;

        int cellX = static_cast<int>(std::floor(y * cellWidth));
        int cellY = static_cast<int>(std::floor(x * cellHeight));

        SDL_Rect cell = { cellX, cellY, cellW, cellH };

        SDL_RenderFillRect(renderer, &cell);
    }

    SDL_RenderPresent(renderer);

    SDL_Event event;

    bool running = true;

    while(running) 
    {
        while(SDL_PollEvent(&event)) 
        {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) 
            {
                running = false;
            }
        }
    }
}

