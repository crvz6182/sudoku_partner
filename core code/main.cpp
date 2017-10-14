#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h> 
#include "../Core/Core.h"

char outputBuff[163000001];
int outputTag;
int(*result_c)[81] = new int[1000000][81];

void output(int sudoku[81]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            outputBuff[outputTag++] = char(sudoku[i * 9 + j] + '0');
            if (j == 8)
                outputBuff[outputTag++] = '\n';
            else
                outputBuff[outputTag++] = ' ';
        }
    }
    outputBuff[outputTag++] = '\n';
}

int myatoi(const char* p) {
    if (p == NULL)
        return 0;
    int result = 0;
    while (isdigit(*p))
    {
        result = result * 10 + (*p++ - '0');
    }
    if (*p != '\0')
        return 0;
    return result;
}

enum
{
    BEGIN = 0,
    TEST_CREATE = 1,
    TEST_SOLVE = 2,
    NUMBER = 3,
    MODE = 4,
    AREA = 5,
    UNIQUE = 6,
    AREA_UNIQUE = 7,
    NUMBER_MODE = 8,
    NUMBER_AREA = 9,
    NUMBER_UNIQUE = 10,
    NUMBER_AREA_UNIQUE = 11
}state;

int main(int argc, char* argv[])
{
    Core test;
    state = BEGIN;
    int number = 0;
    int mode = 0;
    int lower = 0;
    int upper = 0;
    bool unique = false;
    char temp = ' ';
    int sudoku[81] = { 0 };
    int result[81] = { 0 };
    std::string path;
    std::ifstream in;
    std::ofstream out;
    for (int i = 1; i < argc;)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            if (argc != 3 || i != 1)
            {
                std::cout << "参数错误：-c不和其他参数同时使用" << std::endl;
                return 1;
            }
            state = TEST_CREATE;
            number = myatoi(argv[2]);
            break;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            if (argc != 3 || i != 1)
            {
                std::cout << "参数错误：-s不和其他参数同时使用" << std::endl;
                return 1;
            }
            state = TEST_SOLVE;
            in.open(argv[2]);
            break;
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            if (state >= 8 || state == 3)
            {
                std::cout << "参数重复：-n" << std::endl;
                return 1;
            }
            switch (state)
            {
            case 0:
                state = NUMBER;
                break;
            case 4:
                state = NUMBER_MODE;
                break;
            case 5:
                state = NUMBER_AREA;
                break;
            case 6:
                state = NUMBER_UNIQUE;
                break;
            case 7:
                state = NUMBER_AREA_UNIQUE;
                break;
            }
            if (argc < i + 2)
            {
                std::cout << "缺少参数：-n后面没有数字" << std::endl;
                return 1;
            }
            number = myatoi(argv[i + 1]);
            if (number == 0)
            {
                std::cout << "参数错误：-n后面数字不合法" << std::endl;
                return 1;
            }
            i = i + 2;
        }
        else if (strcmp(argv[i], "-m") == 0)
        {
            if (state == 4 || state == 8)
            {
                std::cout << "参数重复：-m" << std::endl;
                return 1;
            }
            if (state >= 5)
            {
                std::cout << "参数错误：-m不与-r,-u同时使用" << std::endl;
                return 1;
            }
            switch (state)
            {
            case 0:
                state = MODE;
                break;
            case 3:
                state = NUMBER_MODE;
                break;
            }
            if (argc < i + 2)
            {
                std::cout << "缺少参数：-m后面没有数字" << std::endl;
                return 1;
            }
            mode = myatoi(argv[i + 1]);
            if (mode == 0)
            {
                std::cout << "参数错误：-m后面数字不合法" << std::endl;
                return 1;
            }
            i = i + 2;
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            if (state == 5 || state == 7 || state == 9 || state == 11)
            {
                std::cout << "参数重复：-r" << std::endl;
                return 1;
            }
            if (state == 4 || state == 8)
            {
                std::cout << "参数重复：-r不与-m同时使用" << std::endl;
                return 1;
            }
            switch (state)
            {
            case 0:
                state = AREA;
                break;
            case 3:
                state = NUMBER_AREA;
                break;
            case 6:
                state = AREA_UNIQUE;
                break;
            case 10:
                state = NUMBER_AREA_UNIQUE;
                break;
            }
            lower = myatoi(strtok(argv[i + 1], "~"));
            upper = myatoi(strtok(NULL, "~"));
            if (lower < 20 || upper > 55 || strtok(NULL, "~") != NULL)
            {
                std::cout << "参数错误：-r后面范围不合法" << std::endl;
                return 1;
            }
            i = i + 2;
        }
        else if (strcmp(argv[i], "-u") == 0)
        {
            if (state == 6 || state == 7 || state == 10 || state == 11)
            {
                std::cout << "参数重复：-u" << std::endl;
                return 1;
            }
            if (state == 4 || state == 8)
            {
                std::cout << "参数重复：-u不与-m同时使用" << std::endl;
                return 1;
            }
            i++;
            switch (state)
            {
            case 0:
                state = UNIQUE;
                break;
            case 3:
                state = NUMBER_UNIQUE;
                break;
            case 5:
                state = AREA_UNIQUE;
                break;
            case 9:
                state = NUMBER_AREA_UNIQUE;
                break;
            }
            unique = true;
        }
        else
        {
            std::cout << "参数错误：有不合法的参数" << std::endl;
            return 1;
        }
    }
    try
    {
        switch (state)
        {
        case 0:
            return 0;
        case 1:
        case 3:
        case 8:
        case 9:
        case 10:
        case 11:
            out.open("sudoku.txt");
            if (!out.is_open())
            {
                std::cout << "打开文件时出错" << std::endl;
                return 1;
            }
            switch (state)
            {
            case 1:
                test.generate(number, result_c);
                break;
            case 3:
                test.generate(number, 20, 55, false, result_c);
                break;
            case 8:
                test.generate(number, mode, result_c);
                break;
            case 9:
                test.generate(number, lower, upper, false, result_c);
                break;
            case 10:
                test.generate(number, 20, 55, true, result_c);
                break;
            case 11:
                test.generate(number, lower, upper, true, result_c);
                break;
            }
            for (int i = 0; i < number; i++) {
                output(result_c[i]);
            }
            out << outputBuff;
            out.close();
            std::cout << "输出完成" << std::endl;
            return 0;
        case 2:
            out.open("sudoku.txt");
            if (!in.is_open())
            {
                std::cout << "打开文件时出错" << std::endl;
                return 1;
            }
            if (!out.is_open())
            {
                std::cout << "打开文件时出错" << std::endl;
                return 1;
            }
            do {
                while ('0' > temp || temp > '9')
                    in >> temp;
                sudoku[0] = temp - '0';
                for (int i = 1; i < 81; i++)
                {
                    in >> temp;
                    if (in.eof()) break;
                    if ('0' > temp || temp > '9')
                    {
                        std::cout << "文件内容错误" << std::endl;
                        return 1;
                    }
                    sudoku[i] = temp - '0';
                }
                if (in.eof())break;
                test.solve(sudoku, result);
                output(result);
                in >> temp;
            } while (!in.eof());
            out << outputBuff;
            in.close();
            out.close();
            std::cout << "输出完成" << std::endl;
            return 0;
        default:
            std::cout << "参数错误" << std::endl;
            return 1;
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    };
}