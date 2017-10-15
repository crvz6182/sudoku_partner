#pragma once

#include <iostream>
#include <time.h>

using namespace std;

class NumberException :public exception
{
public:
    NumberException();
};

class ModeException :public exception
{
public:
    ModeException();
};

class RangeException :public exception
{
public:
    RangeException();
};

class ValidException :public exception
{
public:
    ValidException();
};

class FinalMaker
{
public:
    void make(int final[9][9]);
    void shuffle(int array[9]);
    bool fillRow(int final[9][9], int row);
    bool fill(int final[9][9], int row, int col);
    bool checkValid(int final[9][9], int row, int col, int value);
};

class PuzzleSovlver
{
public:
    bool checkValid(int final[9][9]);
    void init();
    void link(int final[9][9]);
    void insertNode(int col, int no);
    void remove(int col);
    void restore(int col);
    bool findOneSolution(int final[9][9], int select);
    bool findSolutions(int final[9][9], int select, int& solution);
    bool checkUnique(int final[9][9]);   
};

class Core
{
public:
    __declspec(dllexport) void generate(int number, int mode, int result[][81]);
    __declspec(dllexport) void generate(int number, int lower, int upper, bool unique, int result[][81]);
    __declspec(dllexport) bool solve(int puzzle[81], int solution[81]);
private:
    FinalMaker fm;
    PuzzleSovlver ps;
    void hollowUnique(int final[9][9], int blanks);
    void hollowRandom(int final[9][9], int blanks);
};