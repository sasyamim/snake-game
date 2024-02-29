// Snake Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"
#include <deque> //data structure

using namespace std;

Color Green = { 173,204,69,255 };
Color Dark_Green = { 43,51,24,255 };

//size of screen
int cell_size = 30;
int cell_count = 25;

class Snake {

public:
    deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9 }, Vector2{4, 9} };

    void Draw();
};

void Snake::Draw() {
    for (unsigned int i = 0; i < body.size(); i++) {
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = Rectangle{ x * cell_size, y * cell_size, (float)cell_size, (float)cell_size };
        DrawRectangleRounded(segment, 0.5, 6, Dark_Green);
    }
}

class Food {

public:
    Vector2 position;
    Texture2D texture;

    Food() {
        Image image = LoadImage("graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos();
    }

    ~Food() {
        UnloadTexture(texture);
    }

    
    void Draw();
    Vector2 GenerateRandomPos() {
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);
        return Vector2{ x,y };
    }
};

void Food::Draw() {
    DrawTexture(texture, position.x * cell_size, position.y * cell_size, WHITE);
}

int main()
{
    cout << "Starting the game..." << endl;
    InitWindow(cell_size * cell_count, cell_size * cell_count, "Retro Snake");
    SetTargetFPS(60);

    Food food;
    Snake snake;

    while (!WindowShouldClose()) {
        BeginDrawing();

        //Drawing
        ClearBackground(Green);
        food.Draw();
        snake.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
