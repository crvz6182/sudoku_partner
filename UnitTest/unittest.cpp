#include <time.h>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Core/Core.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(TestBlank)
        {
            // TODO: 在此输入测试代码
            int blankNum = 0;
            int result[81] = { 0 };
            int puzzle[81] = { 8,1,2,7,5,3,6,4,9,9,4,3,6,8
                ,2,1,7,5,6,7,5,4,9,1,2,8,3,1,5,4,2,3,7,8,9
                ,6,3,6,9,8,4,5,7,2,1,2,8,7,1,6,9,5,3,4,5,2
                ,1,9,7,4,3,6,8,4,3,8,5,2,6,9,1,7,7,9,6,3,1
                ,8,4,5,2 };
            Core test;
            test.blank(puzzle, 1);
            for (int i = 0; i < 81; i++)
            {
                if (puzzle[i] == 0)
                {
                    blankNum++;
                }
            }
            Assert::AreEqual((20 <= blankNum&&blankNum <= 35), true);
            blankNum = 0;
            test.blank(puzzle, 2);
            for (int i = 0; i < 81; i++)
            {
                if (puzzle[i] == 0)
                {
                    blankNum++;
                }
            }
            Assert::AreEqual((36 <= blankNum&&blankNum <= 45), true);
            blankNum = 0;
            test.blank(puzzle, 3);
            for (int i = 0; i < 81; i++)
            {
                if (puzzle[i] == 0)
                {
                    blankNum++;
                }
            }
            Assert::AreEqual((46 <= blankNum&&blankNum <= 55), true);
            blankNum = 0;
            test.blank(puzzle, 40, 45, true);
            for (int i = 0; i < 81; i++)
            {
                if (puzzle[i] == 0)
                {
                    blankNum++;
                }
            }
            Assert::AreEqual((40 <= blankNum&&blankNum <= 45), true);
        }

        TEST_METHOD(TestSolve)
        {
            // TODO: 在此输入测试代码
            int puzzle[81] = { 8,1,2,7,5,3,6,4,9,9,4,3,6,8
                ,2,1,7,5,6,7,5,4,9,1,2,8,3,1,5,4,2,3,7,8,9
                ,6,3,6,9,8,4,5,7,2,1,2,8,7,1,6,9,5,3,4,5,2
                ,1,9,7,4,3,6,8,4,3,8,5,2,6,9,1,7,7,9,6,3,1
                ,8,4,5,2 };
            int result[81] = { 0 };
            int answer[81] = { 8,1,2,7,5,3,6,4,9,9,4,3,6,8
                ,2,1,7,5,6,7,5,4,9,1,2,8,3,1,5,4,2,3,7,8,9
                ,6,3,6,9,8,4,5,7,2,1,2,8,7,1,6,9,5,3,4,5,2
                ,1,9,7,4,3,6,8,4,3,8,5,2,6,9,1,7,7,9,6,3,1
                ,8,4,5,2 };
            int wrong[81] = { 8,1,2,7,5,3,6,4,9,9,4,3,6,8
                ,2,1,7,5,6,7,5,4,9,1,8,2,3,1,5,4,2,3,7,8,9
                ,6,3,6,9,8,4,5,7,2,1,2,8,7,1,6,9,5,3,4,5,2
                ,1,9,7,4,3,6,8,4,3,8,5,2,6,9,1,7,7,9,6,3,1
                ,8,4,5,2 };
            Core test;
            bool isvalid = true;
            test.blank(puzzle, 20, 55, true);
            test.solve(puzzle, result);
            isvalid = test.solve(wrong, result);
            for (int i = 0; i < 81; i++)
            {
                Assert::AreEqual(result[i], answer[i]);
            }
            Assert::AreEqual(isvalid, false);
            isvalid = test.solve(answer, result);
            Assert::AreEqual(isvalid, true);
        }
        TEST_METHOD(TestGenerate)
        {
            // TODO: 在此输入测试代码
            int sudo[9][9];
            Core test;
            int result[3][81];
            test.generate(3, result);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 81; j++)
                {
                    sudo[j / 9][j % 9] = result[i][j];
                }
                for (int j = 0; j < 9; j++)
                {
                    for (int k = 0; k < 9; k++)
                    {
                        for (int n = 0; n < 9; n++)
                        {
                            if (n != j)
                            {
                                Assert::AreEqual(false, sudo[i][j] == sudo[i][n]);
                            }
                            if (n != i)
                            {
                                Assert::AreEqual(false, sudo[i][j] == sudo[n][j]);
                            }
                        }
                    }
                }
            }
        }
        TEST_METHOD(TestException)
        {
            Core test;
            int result[3][81];
            try {
                test.generate(2000000, result);
            }
            catch (numberException &e)
            {
                Assert::AreEqual(0, 0);
            }
            try {
                test.generate(20000, 2, result);
            }
            catch (numberException &e)
            {
                Assert::AreEqual(0, 0);
            }
            try {
                test.generate(20000, 20, 40, false, result);
            }
            catch (numberException &e)
            {
                Assert::AreEqual(0, 0);
            }
            try {
                test.generate(3, 5, result);
            }
            catch (modeException &e)
            {
                Assert::AreEqual(0, 0);
            }
            try {
                test.generate(3, 41, 40, false, result);
            }
            catch (boundException &e)
            {
                Assert::AreEqual(0, 0);
            }
            try {
                test.generate(3, 52, 40, false, result);
            }
            catch (boundException &e)
            {
                Assert::AreEqual(0, 0);
            }
            try {
                test.generate(3, 32, 70, false, result);
            }
            catch (boundException &e)
            {
                Assert::AreEqual(0, 0);
            }
        }
    };
}