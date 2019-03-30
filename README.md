# calgorithms

### 一, 排序算法

常见算法八种常用的排序算法:
1. 插入排序
2. 冒泡排序
3. 选择排序
4. 希尔排序
5. 快速排序
6. 归并排序
7. 堆排序和
8. LST基数排序

### 二, 查找算法

常见算法七种常用的查找算法:

1. 顺序查找
2. 二分查找
3. 插值查找
4. 斐波那契查找
5. 树表查找
5.2 平衡查找树之2-3查找树（2-3 Tree）
6. 分块查找
7. 哈希查找



###  三, 大O表示法


```
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//算法是最终要编译成具体的计算机指令
//每一个指令 在具体的计算机 cpu 上运行的时间是固定的
//通过具体的n的步骤的多少就可以推算法的复杂度
long sum1(int n)  // 2n + 4  ===>   n->无穷大时 可以忽略 + 4了 就是  O(n)
{
	long ret = 0; //1
	int* array = static_cast<int*>(malloc(sizeof(int)* n)); //1

	int i;// 1
	for (i = 0; i < n; ++i) //n
	{
		array[i] = i + 1;
	}

	for (i = 0; i < n; ++i) //n
	{
		ret += array[i];
	}

	free(array); //1

	return ret;
}

long sum2(int n) //n + 2    ===>   n->无穷大 是  O(n)
{
	long ret = 0; // 1
	int i = 0;  // 1
	for (i = 1; i <= n; ++i) // n
	{
		ret += i;
	}

	return ret;
}

long sum3(int n)  // 2  ==== 》 O(1)
{
	long ret = 0; // 1
	if (n > 0)
	{
		ret = (1 + n) * n / 2;  // 1  // 2
	}
	return ret;
}

int main(int argc, char *argv[])
{
	
	
	system("pause");
	return EXIT_SUCCESS;
}
```



### 四, 散列


