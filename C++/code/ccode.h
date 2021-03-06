/********************************************************************
	created:	2019-03-01

	author:		chensong

	purpose:	
*********************************************************************/

#ifndef _C_CODE_H_ 
#define _C_CODE_H_
#include <iostream>

//////////////////////////////////////////////////////////////////
// 1. 给定N张扑克牌和一个随机函数，设计一个洗牌算法
//////////////////////////////////////////////////////////////////
template<typename T>
void shuffle(T cards[],int n)
{
    if(cards == NULL)
	{
		return ;
	}
 
    srand(time(0));
 
    for(int i=0; i < n-1; ++i)
    {
        //保证每次第i位的值不会涉及到第i位以前
        int index = i + rand() % ( n - i );
        T temp = cards[i];
        cards[i]=cards[index];
        cards[index]=temp;
    }
}



///////////////////////////////////////////////////////////////////
//2. 实现一个memcpy函数
///////////////////////////////////////////////////////////////////
void *memcpy(void *dst, const void *src, size_t len)
{
	if(NULL == dst || NULL == src)
	{
		return NULL;
	}
  
	void *ret = dst;

	if(dst <= src || (char *)dst >= (char *)src + len)
	{
		//没有内存重叠，从低地址开始复制
		while(len--)
		{
			*(char *)dst = *(char *)src;
			dst = (char *)dst + 1;
			src = (char *)src + 1;
		}
	}
	else
	{
		//有内存重叠，从高地址开始复制
		src = (char *)src + len - 1;
		dst = (char *)dst + len - 1;
		while(--len)
		{
			*(char *)dst = *(char *)src;
			dst = (char *)dst - 1;
			src = (char *)src - 1;
		}
	}
	return ret;
}


#endif  // _C_CODE_H_