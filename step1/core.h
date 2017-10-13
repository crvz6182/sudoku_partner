#pragma once

struct Node {
    Node *up, *down, *left, *right, *col;
    int row, count;
    int num[3] = { 0 };
};

class Core 
{
public:
    bool solve(int puzzle[81], int solution[81]);
    bool blank(int puzzle[81], int mode, int result[81]);
    bool blank(int puzzle[81], int lower, int upper, bool unique, int result[81]);
    Node* toMatrix(int puzzle[81]);
    void remove(Node* c);
    void resume(Node* c); 
    int dance(Node* head, int solution[81], int &tag);
    int DLX(int puzzle[81], int solution[81], int &tag);

    void generate(int number, int result[][81]);
    void generate(int number, int mode, int result[][81]);
    void generate(int number, int lower, int upper, bool unique, int result[][81]);
    void produce(int total, int nums[], int block_num, int & count_total, int count_nums, int sudo[9][9], int result[][81]);
    bool isinraw(int num, int raw_num, int sudo[9][9]);
    bool isincolumn(int num, int c_num, int sudo[9][9]);
    int insert(int num, int blocknum, int marked[], int sudo[9][9]);
    void out(int sudo[9][9], int result[][81], int count_total);
};
