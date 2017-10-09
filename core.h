#pragma once

struct Node {
    Node *up, *down, *left, *right, *col;
    int row, count;
    int num[3] = { 0 };
};

class Core 
{
public:
    bool solve(int* puzzle, int* solution);
    bool blank(int* puzzle, int mode, int* result);
    bool blank(int* puzzle, int lower, int upper, bool unique, int* result);
    Node* toMatrix(int* puzzle);
    void remove(Node* c);
    void resume(Node* c); 
    int dance(Node* head, int* solution, int &tag);
    int DLX(int* puzzle, int* solution, int &tag);
};
