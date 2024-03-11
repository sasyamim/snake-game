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
int offset = 75; //width of edge borders

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

//check if food generated is on the snake's body
bool ElementInDeque(Vector2 element,deque<Vector2> deque) {
    for (unsigned int i = 0; i < deque.size(); i++) {
        if (Vector2Equals(deque[i], element)) {
            return true;
        }
    }
    return false;
}

class Snake {

public:
    deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
    Vector2 direction = { 1,0 };

    void Draw();
    void Update();
    bool addSegment = false;
    void Reset();
};

void Snake::Draw() {
    for (unsigned int i = 0; i < body.size(); i++) {
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = Rectangle{offset+ x * cell_size,offset+ y * cell_size, (float)cell_size, (float)cell_size };
        DrawRectangleRounded(segment, 0.5, 6, Dark_Green);
    }
}

void Snake::Update() {
    body.push_front(Vector2Add(body[0], direction)); //add a segment when food is eaten.

    if (addSegment == true) {
        addSegment = false;
    }
    else {
        body.pop_back();
    }
}

void Snake::Reset() {
    body = { Vector2{6,9},Vector2{5,9},Vector2{4,9} };
    direction = { 1,0 };
}

class Food {

public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody) {
        Image image = LoadImage("graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food() {
        UnloadTexture(texture);
    }

    
    void Draw();

    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);
        return Vector2{ x, y };
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
 
        Vector2 position = GenerateRandomCell();
        //if food gen is on the snake's body, gen a new food elsewhere
        while (ElementInDeque(position, snakeBody)) {
            position = GenerateRandomCell();
        }
        return position;
    }
};

void Food::Draw() {
    DrawTexture(texture, offset+ position.x * cell_size, offset+ position.y * cell_size, WHITE);
}

class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;

    Game() {
        InitAudioDevice();
        eatSound = LoadSound("sounds/eat.mp3");
        wallSound = LoadSound("sounds/wall.mp3");
    }

    ~Game() {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update() {
        if (running) {
            snake.Update();
            CheckCollisionFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionFood() {
        if (Vector2Equals(snake.body[0], food.position)) {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges() {
        //snake.body[0].x == cell_count; //pass beyond right edge of grid
        //snake.body[0].x == -1; //pass the left edge of grid
        if (snake.body[0].x == cell_count || snake.body[0].x == -1) { //horizontal limit
            GameOver();
        }
        if (snake.body[0].y == cell_count || snake.body[0].y == -1) { //vertical limit
            GameOver();
        }
    }

    void CheckCollisionWithTail() {
        deque<Vector2> headlessBody = snake.body; //copy of snake body without head named headlessBody
        headlessBody.pop_front(); //remove head

        if (ElementInDeque(snake.body[0], headlessBody)) { //if snake head in headleeBody deque, collided with tail
            GameOver();
        }
    }

    void GameOver() {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }
};

int main()
{
    cout << "Starting the game..." << endl;
    InitWindow(2 * offset + cell_size * cell_count, 2 * offset + cell_size * cell_count, "Retro Snake");
    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (eventTrigger(0.2)) {
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = { 0,-1 };
            game.running = true;
        }

        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = { 0,1 };
            game.running = true;
        }

        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = { -1,0 };
            game.running = true;
        }

        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = { 1,0 };
            game.running = true;
        }
        
        //Drawing
        ClearBackground(Green);
        DrawRectangleLinesEx(Rectangle{ (float)offset - 5,(float)offset - 5,(float)cell_size * cell_count + 10,(float)cell_size * cell_count + 10 }, 5, Dark_Green);
        DrawText("Retro Snake", offset - 5, 20, 40, Dark_Green);
        DrawText(TextFormat("%i", game.score), offset - 5, offset + cell_size * cell_count + 10, 40, Dark_Green);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
