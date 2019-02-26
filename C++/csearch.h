/********************************************************************
	created:	2019/02/26

	author:		chensong

	purpose:	查找算法- 
					1. 二分查找
					
*********************************************************************/

#ifndef _C_SEARCH_H_ 
#define _C_SEARCH_H_
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////////////
// 1. 二分查找
//非递归方式，在有序数组中查找元素x，若不存在就返回-1
template<class T>
int bianrysearch(T a[], int n, const T& x) 
{ 
	int left = 0, right = n-1;
	while(left <= right) 
	{
		int middle = (left + right) / 2;
		if(x == a[middle]) 
		{
			return middle;
		}
		if(x > a[middle]) 
		{
			left = middle + 1;
		}
		else 
		{
			right = middle - 1;
		}
	}
	return -1;
}
//递归方式
template<class T>
int rebinarysearch(T array[], int left, int right, const T& value) 
{ 
    if(left > right) //递归退出条件
	{ 
        printf("%lf 没找到\n", value);
        return -1;
    }
    int mid = (left + right)/2;
    if(array[mid] == value) 
	{
        return mid;
    }
    int ret = -1;
    if(array[mid] > value) 
	{
        ret = rebinarysearch(array, left, mid - 1, value);
    }
	else 
	{
        ret = rebinarysearch(array, mid+1, right, value);
    }
    return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
int main(int argc, char *argv[]) 
{
	

	return 0;
}*/




#endif  // _C_SEARCH_H_