// Core.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include <algorithm>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h> 
#include <exception>  
#include "../Core/Core.h"




numberException::numberException() :exception("Exception: sudoku number error\n")
{
}

boundException::boundException() : exception("Exception: lower or upper error\n")
{
}

modeException::modeException() : exception("Exception: sudoku mode error\n")
{
}

bool __stdcall Core::solve(int puzzle[81], int solution[81])
{
    int tag = -1;
    DLX(puzzle, solution, tag);
    if (tag == 1)
    {
        return true;
    }
    return false;
}

void __stdcall Core::generate(int number, int result[][81])
{
    int count_total = 0;
    int nums1[9] = { 7,1,2,3,4,5,6,8,9 };
    int nums[9] = { 0 };
    int sudo[9][9] = { 0 };
    int mark[9] = { 0 };
    if (number < 1 || number>1000000)
    {
        throw numberException();
        return;
    }
    for (int i = 0; i < 9; i++) {
        int count = 0;
        int move = (rand() % (9 - i) + 1);
        for (int j = 0; j < 9; j++) {
            if (mark[j] == 0) count++;
            if (count == move) {
                nums[i] = nums1[j];
                mark[j] = 1;
                break;
            }
        }
    }
    produce(number, nums, 0, count_total, 0, sudo, result);
}

void __stdcall Core::generate(int number, int mode, int result[][81])
{
    int count_total = 0;
    int nums1[9] = { 7,1,2,3,4,5,6,8,9 };
    int nums[9] = { 0 };
    int sudo[9][9] = { 0 };
    int mark[9] = { 0 };
    if (number < 1 || number>10000)
    {
        throw numberException();
        return;
    }
    for (int i = 0; i < 9; i++) {
        int count = 0;
        int move = (rand() % (9 - i) + 1);
        for (int j = 0; j < 9; j++) {
            if (mark[j] == 0) count++;
            if (count == move) {
                nums[i] = nums1[j];
                mark[j] = 1;
                break;
            }
        }
    }
    produce(number, nums, 0, count_total, 0, sudo, result);
    for (int i = 0; i < number; i++)
    {
        blank(result[i], mode);
    }
}

void __stdcall Core::generate(int number, int lower, int upper, bool unique, int result[][81])
{
    int count_total = 0;
    int nums1[9] = { 7,1,2,3,4,5,6,8,9 };
    int nums[9] = { 0 };
    int sudo[9][9] = { 0 };
    int mark[9] = { 0 };
    if (number < 1 || number>10000)
    {
        throw numberException();
        return;
    }
    for (int i = 0; i < 9; i++) {
        int count = 0;
        int move = (rand() % (9 - i) + 1);
        for (int j = 0; j < 9; j++) {
            if (mark[j] == 0) count++;
            if (count == move) {
                nums[i] = nums1[j];
                mark[j] = 1;
                break;
            }
        }
    }
    produce(number, nums, 0, count_total, 0, sudo, result);
    int temp[81] = { 0 };
    for (int i = 0; i < number; i++)
    {
        for (int j = 0; j < 81; j++)
        {
            temp[j] = result[i][j];
        }
        blank(temp, lower, upper, unique);
        for (int j = 0; j < 81; j++)
        {
            if (temp[j] == 0)
            {
                result[i][j] = 0;;
            }
        }
    } 
}

bool __stdcall Core::blank(int puzzle[81], int mode)
{
    int lower;
    int upper;
    bool suc = false;
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
        throw modeException();
        return false;
    }
    suc = blank(puzzle, lower, upper, false);
    return suc;
}


bool __stdcall Core::blank(int puzzle[81], int lower, int upper, bool unique)
{
    int blankNum = 0;
    int tag = -1;
    int temp[81];
    int buf[81];
    if (lower < 20 || upper>55 || lower > upper)
    {
        throw boundException();
        return false;
    }
    for (int i = 0; i < 81; ++i) {
        buf[i] = puzzle[i];
    }
    blankNum = (rand() % (upper - lower + 1)) + lower;
    do
    {
        for (int i = 0; i < 81; i++)
        {
            puzzle[i] = buf[i];
        }
        int j = 0;
        for (int i = 0; i < blankNum; i++)
        {
            j = rand() % 81;
            if (puzzle[j] == 0)
            {
                i--;
            }
            else
            {
                puzzle[j] = 0;
            }
        }
        if (unique)
        {
            tag = 0;
            DLX(puzzle, temp, tag);
        }
    } while (unique && (tag == 2));
    return true;
}


Node* Core::toMatrix(int puzzle[81])
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

int Core::dance(Node* head, int solution[81], int &tag)
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
            delete head;
            return 1;
        }
        if (tag == -1)
        {
            tag = 1;
            return 1;
        }
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

int Core::DLX(int puzzle[81], int solution[81], int &tag)
{
    Node* head = toMatrix(puzzle);
    for (int i = 0; i < 81; i++)
    {
        solution[i] = 0;
    }
    return dance(head, solution, tag);
}

bool Core::isinraw(int num, int raw_num, int sudo[9][9])
{//行
    for (int i = 0; i < 9; i++)
    {
        if (sudo[raw_num][i] == num)
        {
            return true;
        }
    }
    return false;
}
bool Core::isincolumn(int num, int c_num, int sudo[9][9])
{//列
    for (int i = 0; i < 9; i++)
    {
        if (sudo[i][c_num] == num)
        {
            return true;
        }
    }
    return false;
}

int Core::insert(int num, int blocknum, int marked[], int sudo[9][9])
{

    int pos[9] = { 0,1,2,3,4,5,6,7,8 };
    int x;
    int y;
    for (int i = 0; i < 9; i++)
    {
        x = (pos[i] / 3) + ((blocknum - 1) / 3) * 3;
        y = (pos[i] % 3) + (((blocknum - 1) % 3) * 3);
        if (sudo[x][y] != 0 || marked[pos[i]] == 1 || isinraw(num, x, sudo) || isincolumn(num, y, sudo))
        {
            continue;
        }
        else
        {
            sudo[x][y] = num;
            return pos[i];
        }
    }
    return -1;
}


void Core::out(int sudo[9][9], int result[][81], int count_total)
{
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            result[count_total - 1][count++] = sudo[i][j];
        }
    }
}

void Core::produce(int total, int nums[], int block_num, int & count_total, int count_nums, int sudo[9][9], int result[][81])
{
    int marked[9] = { 0 };//标记已经试过的位置
    int new_block_num, new_count_nums;
    int now_sudo[9][9];
    while (true)
    {
        new_block_num = block_num + 1;
        new_count_nums = count_nums;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                now_sudo[i][j] = sudo[i][j];
            }
        }
        int now = insert(nums[new_count_nums], new_block_num, marked, now_sudo);

        if (now < 0)
        {
            return;
        }
        else
        {
            marked[now] = 1;
        }

        if (new_block_num == 9)
        {
            if (new_count_nums < 8)
            {
                new_count_nums = count_nums + 1;
                new_block_num = 0;
            }
            else
            {//填写至最后一个
                count_total++;
                out(now_sudo, result, count_total);
                return;
            }
        }
        produce(total, nums, new_block_num, count_total, new_count_nums, now_sudo, result);
        if (count_total == total) return;
    }
}
