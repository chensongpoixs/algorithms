/***********************************************************************************************
	created: 		2020/7/6

	author:			chensong

	purpose:		calloc_mem_pool
我可能会遇到很多的人，听他们讲好2多的故事，我来写成故事或编成歌，用我学来的各种乐器演奏它。
然后还可能在一个国家遇到一个心仪我的姑娘，她可能会被我帅气的外表捕获，又会被我深邃的内涵吸引，在某个下雨的夜晚，她会全身淋透然后要在我狭小的住处换身上的湿衣服。
3小时候后她告诉我她其实是这个国家的公主，她愿意向父皇求婚。我不得已告诉她我是穿越而来的男主角，我始终要回到自己的世界。
然后我的身影慢慢消失，我看到她眼里的泪水，心里却没有任何痛苦，我才知道，原来我的心被丢掉了，我游历全世界的原因，就是要找回自己的本心。
于是我开始有意寻找各种各样失去心的人，我变成一块砖头，一颗树，一滴水，一朵白云，去听大家为什么会失去自己的本心。
我发现，刚出生的宝宝，本心还在，慢慢的，他们的本心就会消失，收到了各种黑暗之光的侵蚀。
从一次争论，到嫉妒和悲愤，还有委屈和痛苦，我看到一只只无形的手，把他们的本心扯碎，蒙蔽，偷走，再也回不到主人都身边。
我叫他本心猎手。他可能是和宇宙同在的级别 但是我并不害怕，我仔细回忆自己平淡的一生 寻找本心猎手的痕迹。
沿着自己的回忆，一个个的场景忽闪而过，最后发现，我的本心，在我写代码的时候，会回来。
安静，淡然，代码就是我的一切，写代码就是我本心回归的最好方式，我还没找到本心猎手，但我相信，顺着这个线索，我一定能顺藤摸瓜，把他揪出来。
************************************************************************************************/

//



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define bool int
#define true (1)
#define false (0)
void show(int * array, int len)
{
    for (int i = 0; i < len; ++i)
    {
        printf(" %d , " , array[i]);
    }
    printf("\n");
}
/**
* 交换数据
*/
void exchange(int * a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

/************************************************************************/
/* 检查是否有序                                                                     */
/************************************************************************/
static bool is_sorted(int *array, int len)
{
	for (int i = 0; i < len; ++i)
	{
		for (int j = i+1; j < len; ++j)
		{
			if (array[i] > array[j])
			{
				return false;
			}
		}
	}
	return true;
}

/**
 * 选择排序
 * @param array
 * @param len
 */
void selection(int * array, int len)
{

    for (int i = 0; i < len; ++i)
    {
        //最小元素的下标
        int min_index = i;
        for (int j = i; j < len; ++j)
        {
            if (array[min_index] > array[j])
            {
                min_index = j;
            }
        }
        //交换元素
		exchange(&array[i], &array[min_index]);
		
    }
}

/************************************************************************/
/* 插入排序                                                                     */
/************************************************************************/
void insertion(int* array, int len)
{
	for (int i = 0; i < len; ++i)
	{
		//判断j下标前面数据是否小于j-1的元素
		for (int j = i; j > 0 && array[j] < array[j -1]; --j)
		{
			//交换元素
			exchange(&array[j-1], &array[j]);
		}
	}
}

/************************************************************************/
/* 合并两个有序的数组数据 [start, mid][mid +1, end] ==> [start, end]                                                                    */
/************************************************************************/
inline static void merge(int *array, int *new_array/*临时变量*/, int start, int mid, int end)
{
	printf("merge start = %d, mid = %d, end = %d\n", start, mid, end);
	int i = start; int j = mid + 1;

	for (int array_index = start; array_index <= end; ++array_index)
	{
		new_array[array_index] = array[array_index];
	}
	for (int array_index = start; array_index <=end; ++array_index)
	{
		if (j > end ) //
		{
			array[array_index] = new_array[i++];
		}
		else if (i > mid)
		{
			array[array_index] = new_array[j++];
		}
		else if (new_array[i] > new_array[j])
		{
			array[array_index] = new_array[j++];
		}
		else
		{
			array[array_index] = new_array[i++];
		}
	}
}



/************************************************************************/
/* 排序                                                                     */
/************************************************************************/
inline static sort(int *array, int *new_array, int start, int end)
{
	//int len = sizeof(*array) / sizeof(int);
	int temp_position = 5;
	int temp_start = start >= temp_position ? (start - temp_position): start;
	//printf("[len = %d, temp_start = %d]\n", 5, temp_start);
	for (int i = 0; i < temp_start; ++i)
	{
		printf("\t");
	}
	
	printf("sort start = %d, end = %d\n", start, end);
	if (start>= end)
	{
		return;
	}
	int position = start + (end - start) / 2;//取中间位置的排序
	sort(&array[0], &new_array[0], start, position);
	sort(&array[0], &new_array[0], position + 1, end);
	merge(&array[0], &new_array[0], start, position, end);
}

/************************************************************************/
/* 归并排序                                                                     */
/************************************************************************/
inline static void merge1(int *array, int len)
{
	int * new_array = (int*)malloc(len * sizeof(int));
	if (!new_array)
	{
		printf("alloc failed !!!\n");
		return;
	}
	sort(&array[0], &new_array[0], 0, len-1);
	if (new_array)
	{
		free(new_array);
		new_array = NULL;
	}
}
int main(int argc, char *argv[])
{

    int array[] = { 78, 23, 56, 12, 1, 100, 23, 11, 9, 2 };
    int len = sizeof(array) / sizeof(int);
    printf("len = %d\n", len);
    show(&array[0], len);
	merge1(&array[0], len);
    show(&array[0], len);
	assert(is_sorted(&array[0], len));
	system("pause");
    return 0;
}

//namespace chen{
//
//}// namespace chen
