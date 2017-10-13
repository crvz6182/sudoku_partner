#pragma once
#include<exception>

struct Node {
    Node *up, *down, *left, *right, *col;
    int row, count;
    int num[3] = { 0 };
};

class numberException :public std::exception
{
public:
    numberException();
};

class boundException :public std::exception
{
public:
    boundException();
};

class modeException :public std::exception
{
public:
    modeException();
};

class Core
{
public:
    _declspec(dllexport) bool __stdcall solve(int puzzle[81], int solution[81]);
    _declspec(dllexport) bool __stdcall blank(int puzzle[81], int mode);
    _declspec(dllexport) bool __stdcall blank(int puzzle[81], int lower, int upper, bool unique);
    Node* toMatrix(int puzzle[81]);
    void remove(Node* c);
    void resume(Node* c);
    int dance(Node* head, int solution[81], int &tag);
    int DLX(int puzzle[81], int solution[81], int &tag);

    _declspec(dllexport) void __stdcall generate(int number, int result[][81]);
    _declspec(dllexport) void __stdcall generate(int number, int mode, int result[][81]);
    _declspec(dllexport) void __stdcall generate(int number, int lower, int upper, bool unique, int result[][81]);
    void produce(int total, int nums[], int block_num, int & count_total, int count_nums, int sudo[9][9], int result[][81]);
    bool isinraw(int num, int raw_num, int sudo[9][9]);
    bool isincolumn(int num, int c_num, int sudo[9][9]);
    int insert(int num, int blocknum, int marked[], int sudo[9][9]);
    void out(int sudo[9][9], int result[][81], int count_total);
};
