/********************************************************************
	created:	2019/02/26

	author:		chensong

	purpose:	查找算法- 
				1. 顺序查找
				2. 二分查找
				3. 插值查找
				4. 斐波那契查找
				5. 树表查找
				5.2 平衡查找树之2-3查找树（2-3 Tree）
				6. 分块查找
				7. 哈希查找
					
*********************************************************************/

#ifndef _C_SEARCH_H_ 
#define _C_SEARCH_H_
#include <iostream>
////////////////////////////////////////////////////////////////////////////////////
//1. 顺序查找
//　说明：顺序查找适合于存储结构为顺序存储或链接存储的线性表。
//　　基本思想：顺序查找也称为线形查找，属于无序查找算法。从数据结构线形表的一端开始，顺序扫描，依次将扫描到的结点关键字与给定值k相比较，若相等则表示///查找成功；若扫描结束仍没有找到关键字等于k的结点，表示查找失败。
//　　复杂度分析：　
//　　查找成功时的平均查找长度为：（假设每个数据元素的概率相等） ASL = 1/n(1+2+3+…+n) = (n+1)/2 ;
//　　当查找不成功时，需要n+1次比较，时间复杂度为O(n);
//　　所以，顺序查找的时间复杂度为O(n)。
template<class T>
int SequenceSearch(T a[], int n, const T& value)
{
    
    for(int i=0; i < n; ++i)
    {
		if(a[i] == value)
        {
			return i;
		}
	}
    return -1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// 2. 二分查找
//非递归方式，在有序数组中查找元素x，若不存在就返回-1

//　　说明：元素必须是有序的，如果是无序的则要先进行排序操作。
//
//　　基本思想：也称为是折半查找，属于有序查找算法。用给定值k先与中间结点的关键字比较，中间结点把线形表分成两个子表，若相等则查找成功；若不相等，再根据k与该中间结点关键字的比较结果确定下一步查找哪个子表，这样递归进行，直到查找到或查找结束发现表中没有这样的结点。
//
//　　复杂度分析：最坏情况下，关键词比较次数为log2(n+1)，且期望时间复杂度为O(log2n)；
//
//　　注：折半查找的前提条件是需要有序表顺序存储，对于静态查找表，一次排序后不再变化，折半查找能得到不错的效率。但对于需要频繁执行插入或删除操作的数据//集来说，维护有序的排序会带来不小的工作量，那就不建议使用。——《大话数据结构》
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
//3. 插值查找
//在介绍插值查找之前，首先考虑一个新问题，为什么上述算法一定要是折半，而不是折四分之一或者折更多呢？
//　　打个比方，在英文字典里面查“apple”，你下意识翻开字典是翻前面的书页还是后面的书页呢？如果再让你查“zoo”，你又怎么查？很显然，这里你绝对不会是从中间开始查起，而是有一定目的的往前或往后翻。
//　　同样的，比如要在取值范围1 ~ 10000 之间 100 个元素从小到大均匀分布的数组中查找5， 我们自然会考虑从数组下标较小的开始查找。
//　　经过以上分析，折半查找这种查找方式，不是自适应的（也就是说是傻瓜式的）。二分查找中查找点计算如下：
//　　mid=(low+high)/2, 即mid=low+1/2*(high-low);
//　　通过类比，我们可以将查找的点改进为如下：
//　　mid=low+(key-a[low])/(a[high]-a[low])*(high-low)，
//　　也就是将上述的比例参数1/2改进为自适应的，根据关键字在整个有序表中所处的位置，让mid值的变化更靠近关键字key，这样也就间接地减少了比较次数。
//　　基本思想：基于二分查找算法，将查找点的选择改进为自适应选择，可以提高查找效率。当然，差值查找也属于有序查找。
//　　注：对于表长较大，而关键字分布又比较均匀的查找表来说，插值查找算法的平均性能比折半查找要好的多。反之，数组中如果分布非常不均匀，那么插值查找未必是很合适的选择。
//　　复杂度分析：查找成功或者失败的时间复杂度均为O(log2(log2n))。
//
template<class T>
int insertionsearch(T a[], int low, int high, const T& value)
{
    int mid = low + ( value- a[low] ) / ( a[high] - a[low] ) * ( high - low );
    if(a[mid] == value)
    {
		return mid;
	}
    if(a[mid] > value)
    {
		return insertionsearch(a, value, low, mid - 1);
	}
    if(a[mid] < value)
    {
		return insertionsearch(a, value, mid + 1, high);
	}
	return 0;
}





#endif  // _C_SEARCH_H_