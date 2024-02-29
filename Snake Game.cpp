// Snake Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <deque> //data structure

using namespace std;

Color Green = { 173,204,69,255 };
Color Dark_Green = { 43,51,24,255 };

//size of screen
int cell_size = 30;
int cell_count = 25;

double lastUpdateTime = 0;

//set the speed of the snake
bool eventTrigger(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }

    return false;
}

class Snake {

public:
    deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
    Vector2 direction = { 1,0 };

    void Draw();
    void Update();
};

void Snake::Draw() {
    for (unsigned int i = 0; i < body.size(); i++) {
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = Rectangle{ x * cell_size, y * cell_size, (float)cell_size, (float)cell_size };
        DrawRectangleRounded(segment, 0.5, 6, Dark_Green);
    }
}

void Snake::Update() {
    body.pop_back();
    body.push_front(Vector2Add(body[0], direction));
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

class Game {
public:
    Snake snake;
    Food food;

    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update() {
        snake.Update();
    }
};

int main()
{
    cout << "Starting the game..." << endl;
    InitWindow(cell_size * cell_count, cell_size * cell_count, "Retro Snake");
    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (eventTrigger(0.2)) {
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = { 0,-1 };
        }

        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = { 0,1 };
        }

        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = { -1,0 };
        }

        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = { 1,0 };
        }
        
        //Drawing
        ClearBackground(Green);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
