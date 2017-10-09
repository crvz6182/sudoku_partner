#include<iostream>

using namespace std;

void generate(int num, int result[][81]);
void produce(int total, int nums[], int block_num, int & count_total, int count_nums, int sudo[9][9], int result[][81]);
bool isinraw(int num, int raw_num, int sudo[9][9]);
bool isincolumn(int num, int c_num, int sudo[9][9]);
int insert(int num, int blocknum, int marked[], int sudo[9][9]);
void out(int sudo[9][9], int result[][81], int count_total);

int result[1000000][81];

int main()
{
	int total = 1;
	generate(total, result);

	int count = 0;
	for (int i = 0; i < total; i++)
	{
		for (int j = 0; j < 81; j++)
		{
			//	cout<<result[i][j]<<" ";
			//if ((j + 1) % 9 == 0) cout << "\n";
		}
		//cout << "\n";
	}

	return 0;
}
void generate(int num, int result[][81])
{
	int count_total = 0;
	int nums1[9] = { 7,1,2,3,4,5,6,8,9 };
	int nums[9] = { 0 };
	int sudo[9][9] = { 0 };
	int mark[9] = { 0 };
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
		cout << nums[i] << " ";
	}

	produce(num, nums, 0, count_total, 0, sudo, result);
}

bool isinraw(int num, int raw_num, int sudo[9][9])
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
bool isincolumn(int num, int c_num, int sudo[9][9])
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

int insert(int num, int blocknum, int marked[], int sudo[9][9])
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


void out(int sudo[9][9], int result[][81], int count_total)
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

void produce(int total, int nums[], int block_num, int & count_total, int count_nums, int sudo[9][9], int result[][81])
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