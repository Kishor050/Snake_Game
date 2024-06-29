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

class Node
{
public:
    COORD data;
    Node *next;
    Node(COORD data1)
    {
        data.X = data1.X;
        data.Y = data1.Y;
        this->next = NULL;
    }
};

class Linkedlist
{

public:
    Node *head;
    Linkedlist() { head = NULL; }
    void insertNode(COORD);

    void deleteNode(int);
    int body_length()
    {
        Node *temp1 = head;
        int ListLen = 0;
        while (temp1 != NULL)
        {
            temp1 = temp1->next;
            ListLen++;
        }
        return ListLen;
    }
};

void Linkedlist::deleteNode(int nodeOffset)
{
    Node *temp1 = head, *temp2 = NULL;
    int ListLen = 0;

    if (head == NULL)
    {
        return;
    }
    while (temp1 != NULL)
    {
        temp1 = temp1->next;
        ListLen++;
    }
    if (ListLen < nodeOffset)
    {
        return;
    }
    temp1 = head;
    if (nodeOffset == 1)
    {
        head = head->next;
        delete temp1;
        return;
    }
    while (nodeOffset-- > 1)
    {
        temp2 = temp1;
        temp1 = temp1->next;
    }
    temp2->next = temp1->next;
    delete temp1;
}

void Linkedlist::insertNode(COORD data)
{
    Node *newNode = new Node(data);
    if (head == NULL)
    {
        head = newNode;
        return;
    }
    Node *temp = head;
    while (temp->next != NULL)
    {

        temp = temp->next;
    }

    temp->next = newNode;
}

class Snake
{
private:
    COORD pos;
    int length;
    int velocity;
    char direction;
    // vector<COORD> body;
    Linkedlist body1;

public:
    Snake(COORD pos, int vel)
    {
        this->pos = pos;
        this->velocity = vel;
        length = 1;
        direction = 'n';
        body1.insertNode(pos);
        // body.push_back(pos);
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

        if (pos.X > WIDTH - 2)
            pos.X = 1;
        else if (pos.X < 1)
            pos.X = WIDTH - 2;
        if (pos.Y > HEIGHT - 2)
            pos.Y = 1;
        else if (pos.Y < 1)
            pos.Y = HEIGHT - 2;
        // body.push_back(pos);
        // if (body.size() > length)
        //     body.erase(body.begin());
        body1.insertNode(pos);
        if (body1.body_length() > length)
            body1.deleteNode(1);
    }
    bool collided()
    {
        if (pos.X < 1 || pos.X > WIDTH - 2 || pos.Y > HEIGHT - 2 || pos.Y < 0)
        {
            return true;
        }
        // for (int i = 0; i < length - 1; i++)
        // {
        //     if (pos.X == body[i].X && pos.Y == body[i].Y)
        //         return true;
        // }
        Node *temp = body1.head;
        for (int i = 0; i < body1.body_length() - 1; i++)
        {
            COORD c = temp->data;
            if (pos.X == c.X && pos.Y == c.Y)
                return true;
            temp = temp->next;
        }
        return false;
    }
    // vector<COORD> get_body()
    // {
    //     return body;
    // }
    Linkedlist get_body1()
    {
        return body1;
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
    // vector<COORD> snake_body = snake.get_body();
    Linkedlist snake_body1 = snake.get_body1();
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
                // for (int k = 0; k < snake_body.size() - 1; k++)
                // {
                //     if (i == snake_body[k].Y && j + 1 == snake_body[k].X)
                //     {
                //         cout << "O";
                //         isBodyPart = true;
                //         break;
                //     }
                // }
                Node *temp1 = snake_body1.head;
                for (int k = 0; k < snake_body1.body_length() - 1; k++)
                {
                    COORD c = temp1->data;
                    if (i == c.Y && j + 1 == c.X)
                    {
                        cout << "O";
                        isBodyPart = true;
                        break;
                    }
                    temp1 = temp1->next;
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