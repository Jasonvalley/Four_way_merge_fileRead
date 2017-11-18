#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void makeRandomNumber(void);
enum makeRandomNumber { Generated_Random_Number = 2000000, Initial_Runsize = 10000 };
void mergeSort(int orig[], int EndIndex);
void mergePass(int initList[], int mergedList[], int EndIndex, int runSize);
void merge(int initList[], int mergedList[], int LeftRunStart, int LeftRunEnd, int RightRunEnd);
enum merge { MAX_SIZE = 100000 };
void readRun(void);
typedef struct element {
	int data;
	int loc;
}element;
element four_way_merge_list[8];
void Init_four_way_merge(void);
element four_way_merge(int list[4]);//return smallest number and location
void test_four_way_merge(void);
void four_way_merge2(char *orginalFile, char *tempFile, int Run_Size);
enum four_way_merge2{Run_Max=Generated_Random_Number/Initial_Runsize+7};
void test_four_way_merge2(void);
void four_way_merge3(void);
int main(void)
{
	makeRandomNumber();
	Init_four_way_merge();
	four_way_merge3();
	readRun();
	system("pause");
}
//make random number
void makeRandomNumber(void)
{
	FILE *f = fopen("original.bin", "wb");
	int countNum = 0, countRun = 0;
	int list[Initial_Runsize];
	while (countNum != Generated_Random_Number)
	{
		list[countNum - countRun*Initial_Runsize] = rand() % 2000000 + 1;
		countNum++;
		if (countNum%Initial_Runsize == 0)
		{
			countRun++;
			//Generate Run(size : Initialize_Runssize+1)
			mergeSort(list, Initial_Runsize-1);
			int i;
			fwrite(list, sizeof(int), Initial_Runsize, f);
			int min = -1;
			fwrite(&min, sizeof(int), 1, f);
		}
	}
	fclose(f);
}
//mergeSort
void mergeSort(int orig[], int EndIndex)
{
	int runSize = 1;
	int extra[MAX_SIZE];
	while (runSize < EndIndex)
	{
		mergePass(orig, extra, EndIndex, runSize);
		runSize *= 2;
		mergePass(extra, orig, EndIndex, runSize);
		runSize *= 2;
	}
}
void mergePass(int initList[], int mergedList[], int EndIndex, int runSize) {
	int left, target;      left = 0;
	for (; left <= (EndIndex - 2 * runSize + 1);
		left += 2 * runSize)
		merge(initList, mergedList, left, left + runSize - 1, left + 2 * runSize - 1);
	if (left + runSize - 1 < EndIndex) {
		merge(initList, mergedList, left, left + runSize - 1, EndIndex);
	}
	else
	{
		for (target = left; target <= EndIndex; target++)
			mergedList[target] = initList[target];
	}
}
void merge(int initList[], int mergedList[], int LeftRunStart, int LeftRunEnd, int RightRunEnd)
{
	int left, right, target;
	left = LeftRunStart;
	right = LeftRunEnd + 1;
	target = LeftRunStart;
	while (left <= LeftRunEnd && right <= RightRunEnd)
	{
		if (initList[left] <= initList[right])
			mergedList[target++] = initList[left++];
		else
			mergedList[target++] = initList[right++];
	}
	if (left > LeftRunEnd)
	{
		for (; right <= RightRunEnd; )
			mergedList[target++] = initList[right++];
	}
	else {
		for (; left <= LeftRunEnd; )
			mergedList[target++] = initList[left++];
	}
}
//read Run
void readRun(void)
{
	FILE *f = fopen("original.bin", "rb");
	int i;
	for (i = 0; i < Generated_Random_Number; i++)
	{
		int n;
		fread(&n, sizeof(int), 1, f);
		printf("%d ", n);
	}
	fclose(f);
}
//four way merge
void Init_four_way_merge(void)
{
	int i;
	for (i = 4; i < 8; i++)
	{
		four_way_merge_list[i].loc = i - 4;
	}
}
element four_way_merge(int list[4])//return smallest number and location
{
	int i;
	for (i = 4; i < 8; i++)
	{
		four_way_merge_list[i].data = list[i - 4];
	}
	for (i = 3; i > 0; i--)
	{
		if (four_way_merge_list[i * 2].data < four_way_merge_list[i * 2 + 1].data)
		{
			if (four_way_merge_list[i * 2].data == -1)
			{
				four_way_merge_list[i] = four_way_merge_list[i * 2 + 1];
			}
			else
			{
				four_way_merge_list[i] = four_way_merge_list[i * 2];
			}
		}
		else
		{
			if (four_way_merge_list[i * 2 + 1].data == -1)
			{
				four_way_merge_list[i] = four_way_merge_list[i * 2];
			}
			else
			{
				four_way_merge_list[i] = four_way_merge_list[i * 2 + 1];
			}
		}

	}
	if (four_way_merge_list[1].data == -1)
	{
		element end;
		end.data = -1;
		end.loc = -1;
		return end;
	}
	else
	{
		return four_way_merge_list[1];
	}
}
void test_four_way_merge(void)
{
	int list[4];
	list[0] = -1;	list[1] = 3;	list[2] = 2;	list[3] = -1;
	element test;
	test=four_way_merge(list);
	printf("%d %d", test.data,test.loc);
}
void four_way_merge2(char *orginalFile,char *tempFile,int Run_Size)
{
	FILE *Temp_FIle = fopen(tempFile, "wb");
	int i, Run_Number = Generated_Random_Number / Run_Size;
	FILE *file[Run_Max];//last four filePointer will be used to check last position
	//Initialize filePointer
	for (i = 0; i < Run_Number; i++)
	{
		file[i] = fopen(orginalFile, "rb");
		fseek(file[i], i*(Run_Size+1)*sizeof(int), 0);
	}
	for (i = Run_Number; i < Run_Number + 7; i++)
	{
		file[i] = NULL;
	}
	int Merge_Count = 0;
	//mergePass
	while(1)
	{
		int list[4], i,check_break=0;
		//check break point
		for (i = 0; i < 4; i++)
		{
			if (file[i+4*Merge_Count]!=NULL)
			{
				check_break = 1;
				break;
			}
		}
		if (check_break == 0)	break;
		//Initialize four_way_merge
		for (i = 0; i < 4; i++)
		{
			if (file[i + 4 * Merge_Count] == NULL)	list[i] = -1;
			else {
				fread(&list[i], sizeof(int), 1, file[i + 4 * Merge_Count]);
			}
		}
		// four way merge
		element receive = four_way_merge(list);
		while (receive.data != -1)
		{
			fwrite(&receive.data, sizeof(int), 1, Temp_FIle);
			int data;
			fread(&data, sizeof(int), 1, file[receive.loc + 4*Merge_Count]);
			list[receive.loc] = data;
			receive = four_way_merge(list);
		}
		int min = -1;
		fwrite(&min, sizeof(int), 1, Temp_FIle);
		Merge_Count++;
	}
	//file close
	fclose(Temp_FIle);
}
void test_four_way_merge2(void)
{
	four_way_merge2("original", "temp.bin", Initial_Runsize);
}
void four_way_merge3(void)
{
	int Run_Size = Initial_Runsize;
	while (Run_Size < Generated_Random_Number)
	{
		four_way_merge2("original.bin", "temp.bin", Run_Size);
		Run_Size *= 4;
		four_way_merge2("temp.bin", "original.bin", Run_Size);
		Run_Size *= 4;
	}
}
