#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h> 

using namespace std;

struct Node {
    Node *up, *down, *left, *right, *col;
    int row, count;
};

class Core
{
public:
    bool solve(int* puzzle, int* solution)
    {

        return true;
    }
    bool blank(int* puzzle, int mode, int* result)
    {
        int lower;
        int upper;
        switch (mode)
        {
        case 1:
            lower = 20;
            upper = 35;
        case 2:
            lower = 36;
            upper = 45;
        case 3:
            lower = 46;
            upper = 55;
        default:
            return false;
        }
        return blank(puzzle, lower, upper, false, result);
    }

//private:
    bool blank(int* puzzle, int lower, int upper, bool unique, int* result)
    {
        int blankNum[9] = { 0 };
        std::vector<int> seed;
        for (int i = 1; i < 10; ++i)
        {
            seed.push_back(i);
        }
        if (lower < 20 || upper>55 || lower > upper)
        {
            return false;
        }
        blankNum[8] = upper;
        for (int i = 0; i < 81; i++)
        {
            result[i] = puzzle[i];
        }
        for (int i = 0; i < 8; i++)
        {
            srand((unsigned)time(NULL));
            blankNum[i] = (rand() % (upper / 9 - lower / 9 + 1)) + lower / 9;
            blankNum[8] -= blankNum[i];
        }
        for (int i = 0; i < 9; i++)
        {
            std::random_shuffle(seed.begin(), seed.end());
            for (int j = 0; j < blankNum[i]; j++)
            {
                result[9 * i + seed[j]] = 0;
            }
        }
        return true;
    }

    Node* toMatrix(int* puzzle)
    {
        int row_num = 0;
        Node *head, *temp[4];
        head = (Node*)malloc(sizeof(Node));
        head->col = head->down = head->up = head;
        Node *c[324];
        for (int i = 0; i < 324; i++)
        {
            c[i] = (Node*)malloc(sizeof(Node));
            c[i]->row = 0;
            c[i]->col = c[i];
            if (i == 0)
            {
                c[i]->left = head;
                c[i]->right = c[323];
            }
            else if (i == 323)
            {
                c[i]->left = c[322];
                c[i]->right = head;
            }
            else
            {
                c[i]->left = c[i - 1];
                c[i]->right = c[i + 1];
            }
        }
        for (int i = 0; i < 81; i++)
        {
            for (int k = (puzzle[i] == 0 ? 0 : puzzle[i] - 1); k < (puzzle[i] == 0 ? 9 : puzzle[i]); k++)
            {
                row_num++;
                for (int m = 0; m < 4; m++)
                {
                    temp[m] = (Node*)malloc(sizeof(Node));
                    temp[m]->row = row_num;
                    if (m == 3)
                    {
                        temp[2]->right = temp[3];
                        temp[3]->left = temp[2];
                        temp[3]->right = temp[0];
                        temp[0]->left = temp[3];
                    }
                    else if (m != 0)
                    {
                        temp[m]->left = temp[m - 1];
                        temp[m - 1]->right = temp[m];
                    }
                }
                temp[0]->col = c[i];
                temp[1]->col = c[81 + i / 9 * 9 + k];
                temp[2]->col = c[162 + i % 9 * 9 + k];
                temp[3]->col = c[243 + (i / 27 * 3 + i % 9 / 3) * 9 + k];
                for (int m = 0; m < 4; m++)
                {
                    temp[m]->up = temp[m]->col->up;
                    temp[m]->down = temp[m]->col;
                    temp[m]->col->up->down = temp[m];
                    temp[m]->col->up = temp[m];
                }
            }
        }
        return head;
    }

    void remove(Node* n)
    {
        n->left->right = n->right;
        n->right->left = n->left;
    }

    void resume(Node* n)
    {
        n->left->right = n;
        n->right->left = n;
    }

    int DLX(int* puzzle)
    {
        Node* head = toMatrix(puzzle);
        int ansNum = 0;
    }
};

    

int main(int argc,char* argv) 
{
    Core test;
}