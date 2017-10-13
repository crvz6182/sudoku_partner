#include "stdafx.h"
#include "CppUnitTest.h"
#include "../sudoku_2/core.h"

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
                ,8,4,5,2};
            Core test;
            test.blank(puzzle, 1, result);
            for (int i = 0; i < 81; i++)
            {
                if (result[i] == 0)
                {
                    blankNum++;
                }
            }
            Assert::AreEqual((20<=blankNum&&blankNum<=35),true);
            blankNum = 0;
            test.blank(puzzle, 2, result);
            for (int i = 0; i < 81; i++)
            {
                if (result[i] == 0)
                {
                    blankNum++;
                }
            }
            Assert::AreEqual((36 <= blankNum&&blankNum <= 45), true);
            blankNum = 0;
            test.blank(puzzle, 3, result);
            for (int i = 0; i < 81; i++)
            {
                if (result[i] == 0)
                {
                    blankNum++;
                }
            }
            Assert::AreEqual((46 <= blankNum&&blankNum <= 55), true);
		}

        TEST_METHOD(TestSolve)
        {
            // TODO: 在此输入测试代码
            int puzzle[81] = { 8,0,0,0,0,0,0,0,0,0,0,3,6,0,0,0,0,0,0,7,0,0,9,0,2,0,0,0,5,0,0,0,7,
                0,0,0,0,0,0,0,4,5,7,0,0,0,0,0,1,0,0,0,3,0,0,0,1,0,0,0,0,6,8,0,0,8,5,0,0,0,1,0,0,9,
                0,0,0,0,4,0,0, };
            int result[81] = { 0 };
            int answer[81] = { 8,1,2,7,5,3,6,4,9,9,4,3,6,8
                ,2,1,7,5,6,7,5,4,9,1,2,8,3,1,5,4,2,3,7,8,9
                ,6,3,6,9,8,4,5,7,2,1,2,8,7,1,6,9,5,3,4,5,2
                ,1,9,7,4,3,6,8,4,3,8,5,2,6,9,1,7,7,9,6,3,1
                ,8,4,5,2 };
            Core test;
            test.blank(answer, 20, 30, true, puzzle);
            test.solve(puzzle, result);
            for (int i = 0; i < 81; i++)
            {
                Assert::AreEqual(result[i], answer[i]);
            }
        }
	};
}