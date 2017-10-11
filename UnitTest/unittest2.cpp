#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Project1/generate.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: 在此输入测试代码
			int sudo[9][9];
			generate(3, result);
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 81; j++)
				{
					sudo[j/9][j%9] = result[i][j];
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
		

	};
}