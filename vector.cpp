#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <ctime>
#include <vector>
#define WIDTH 75
#define HEIGHT 25
using namespace std;
int score;
class Snake
{
private:
    COORD pos;
    int length;
    int velocity;
    char direction;
    vector<COORD> body;

public:
    Snake(COORD pos, int vel)
    {
        this->pos = pos;
        this->velocity = vel;
        length = 1;
        direction = 'n';
        body.push_back(pos);
    }
    void change_dir(char dir)
    {
        this->direction = dir;
    }

    COORD get_pos()
    {
        return pos;
    }
    bool eaten(COORD food_pos)
    {
        if (food_pos.X == pos.X && food_pos.Y == pos.Y)
            return true;
        return false;
    }
    void grow()
    {
        length++;
    }
    void move_snake()
    {
        switch (direction)
        {
        case 'u':
            pos.Y -= velocity;
            break;
        case 'd':
            pos.Y += velocity;
            break;
        case 'l':
            pos.X -= velocity;
            break;
        case 'r':
            pos.X += velocity;
            break;
        }

        body.push_back(pos);
        if (body.size() > length)
            body.erase(body.begin());
    }
    bool collided()
    {
        if (pos.X < 1 || pos.X > WIDTH - 2 || pos.Y > HEIGHT - 2 || pos.Y < 0)
        {
            return true;
        }
        for (int i = 0; i < length - 1; i++)
        {
            if (pos.X == body[i].X && pos.Y == body[i].Y)
                return true;
        }
        return false;
    }
    vector<COORD> get_body()
    {
        return body;
    }
};

class Food
{
private:
    COORD pos;

public:
    void gen_food()
    {
        pos.X = rand() % (WIDTH - 3) + 1;
        pos.Y = rand() % (HEIGHT - 3) + 1;
    }
    COORD get_pos() { return pos; }
};

Snake snake({WIDTH / 2, HEIGHT / 2}, 1);
Food food;

void board()
{
    COORD snake_pos = snake.get_pos();
    COORD food_pos = food.get_pos();
    vector<COORD> snake_body = snake.get_body();
    cout << endl
         << "SCORE :" << score << "\n"
         << "\n";
    for (int i = 0; i < HEIGHT; i++)
    {

        cout << "#";

        for (int j = 0; j < WIDTH - 2; j++)
        {

            if (i == 0 || i == HEIGHT - 1)
                cout << "#";
            else if (i == snake_pos.Y && j + 1 == snake_pos.X)
                cout << 'O';
            else if (i == food_pos.Y && j + 1 == food_pos.X)
                cout << "@";
            else
            {
                bool isBodyPart = false;
                for (int k = 0; k < snake_body.size() - 1; k++)
                {
                    if (i == snake_body[k].Y && j + 1 == snake_body[k].X)
                    {
                        cout << "O";
                        isBodyPart = true;
                        break;
                    }
                }
                if (!isBodyPart)
                    cout << " ";
            }
        }
        cout << "#\n";
    }
}

int main()
{
    score = 0;
    srand(time(NULL));
    food.gen_food();
    bool game_over = false;
    while (!game_over)
    {
        board();
        if (kbhit())
        {
            switch (getch())
            {
            case 'w':
                snake.change_dir('u');
                break;
            case 'a':
                snake.change_dir('l');
                break;
            case 's':
                snake.change_dir('d');
                break;
            case 'd':
                snake.change_dir('r');
                break;
            }
        }
        if (snake.collided())
            game_over = true;
        if (snake.eaten(food.get_pos()))
        {
            food.gen_food();
            snake.grow();
            score += 10;
        }

        snake.move_snake();
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0}); // tekes cursor back to origin
    }
}
