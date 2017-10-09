#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h> 
#include "../sudoku_2/core.h"

bool Core::solve(int* puzzle, int* solution)
{
    int tag = -1;
    DLX(puzzle, solution, tag);
    return true;
}

bool Core::blank(int* puzzle, int mode, int* result)
{
    int lower;
    int upper;
    switch (mode)
    {
    case 1:
        lower = 20;
        upper = 35;
        break;
    case 2:
        lower = 36;
        upper = 45;
        break;
    case 3:
        lower = 46;
        upper = 55;
        break;
    default:
        return false;
    }
    return blank(puzzle, lower, upper, false, result);
}


bool Core::blank(int* puzzle, int lower, int upper, bool unique, int* result)
{
    int blankNum = 0;
    int tag = -1;
    int temp[81] = { 0 };
    if (lower < 20 || upper>55 || lower > upper)
    {
        return false;
    }
    for (int i = 0; i < 81; i++)
    {
        result[i] = puzzle[i];
    }
    srand((unsigned)time(NULL));
    blankNum = (rand() % (upper - lower + 1)) + lower;
    do
    {
        int j = 0;
        for (int i = 0; i < blankNum; i++)
        {
            j = rand() % 81;
            if (result[j] == 0)
            {
                i--;
            }
            else
            {
                result[j] = 0;
            }
        }
        if (unique)
        {
            tag = 0;
        }
        DLX(result, temp, tag);
    } while (unique && (tag == 2));
    return true;
}

Node* Core::toMatrix(int* puzzle)
{
    int row_num = 0;
    Node *head, *temp[4];
    head = (Node*)malloc(sizeof(Node));
    head->col = head->down = head->up = head;
    Node *c[324];
    for (int i = 0; i < 324; i++)
    {
        c[i] = (Node*)malloc(sizeof(Node));
        c[i]->up = c[i];
        c[i]->down = c[i];
    }
    head->right = c[0];
    head->left = c[323];
    for (int i = 0; i < 324; i++)
    {
        c[i]->row = 0;
        c[i]->count = 0;
        c[i]->col = c[i];
        c[i]->num[0] = i;
        if (i == 0)
        {
            c[i]->left = head;
            c[i]->right = c[1];
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
            }
            for (int m = 0; m < 4; m++)
            {
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
                temp[m]->num[0] = i / 9;
                temp[m]->num[1] = i % 9;
                temp[m]->num[2] = k + 1;
                temp[m]->col->count++;
                temp[m]->up = temp[m]->col->up;
                temp[m]->down = temp[m]->col;
                temp[m]->col->up->down = temp[m];
                temp[m]->col->up = temp[m];
            }
        }
    }
    return head;
}

void Core::remove(Node* c)
{
    c->left->right = c->right;
    c->right->left = c->left;
    Node *x = c->down;
    while (x != c)
    {
        Node *y = x->right;
        while (y != x)
        {
            y->up->down = y->down;
            y->down->up = y->up;
            y = y->right;
            y->col->count--;
        }
        x = x->down;
    }
}

void Core::resume(Node* c)
{
    c->left->right = c;
    c->right->left = c;
    Node *x = c->down;
    while (x != c)
    {
        Node *y = x->right;
        while (y != x)
        {
            y->up->down = y;
            y->down->up = y;
            y = y->right;
            y->col->count++;
        }
        x = x->down;
    }
}

int Core::dance(Node* head, int* solution,int &tag)
{
    if (head->right == head)
    {
        if (tag == 0)
        {
            tag = 1;
            return 0;
        }
        if (tag == 1)
        {
            tag = 2;
            return 1;
        }
        return 1;
    }
    Node *i = head->right->right;
    Node *temp = head->right;
    while (i != head)
    {
        if (i->count<temp->count)
            temp = i;
        i = i->right;
    }
    if (temp->down == temp)return 0;
    remove(temp);
    i = temp->down;
    while (i != temp)
    {
        Node *j = i->right;
        solution[i->num[0] * 9 + i->num[1]] = i->num[2];
        while (j != i)
        {
            remove(j->col);
            j = j->right;
        }
        if (dance(head, solution, tag))
        {
            return 1;
        }
        j = i->left;
        while (j != i)
        {
            resume(j->col);
            j = j->left;
        }
        solution[i->num[0] * 9 + i->num[1]] = 0;
        i = i->down;
    }
    resume(temp);
    return 0;
}

int Core::DLX(int* puzzle, int* solution, int &tag)
{
    Node* head = toMatrix(puzzle);
    for (int i = 0; i < 81; i++)
    {
        solution[i] = 0;
    }
    return dance(head, solution, tag);
}