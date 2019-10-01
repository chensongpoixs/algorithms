/***********************************************************************************************
created: 		2019-10-01

author:			chensong

purpose:		sha1
************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static const int parm[5] = { 
	0x67452301, 
	0xEFCDAB89,
	0x98BADCFE,
	0x10325476,
	0xC3D2E1F0 
};

static int chararraytoint(unsigned char* ptr, unsigned int index)
{
	return ((ptr[index] & 0xff) << 24) | ((ptr[index + 1] & 0xff) << 16) | ((ptr[index + 2] & 0xff) << 8) | (ptr[index + 3] & 0xff);
}

static int s(unsigned int x, unsigned int i)
{
	return (x << i) | x >> (32 - i);
}


static int f1(int x, int y, int z)
{
	return (x&y) | (~x&z);
}
static int f2(int x, int y, int z)
{
	return x^y^z;
}
static int f3(int x, int y, int z)
{
	return (x&y) | (x&z) | (y&z);
}
static int f4(int x, int y, int z)
{
	return x^y^z;
}


unsigned char * get_src_hex(char *p, unsigned int len, unsigned long long *size)
{
	if (!p || len <= 0)
	{
		return NULL;
	}
	unsigned int fill = 0; //填充'0'
	//unsigned long long size = *_size; //数据的大小
	
	unsigned int  m_size = len % 64; //有几个数据块     512位为一块
	if (m_size < 56) //  最后的16位是数据块的长度保存
	{
		fill = 55 - m_size;
		*size = len - m_size + 64; //数据的大小//size = strlen(p) - src_lenght + 64;//data_size的开始位置
	}
	else if (m_size == 56)
	{
		fill = 63;
		*size = len + 8 + 64;
	}
	else
	{
		fill = 63 - m_size + 56;
		*size = (len + 64) - m_size + 64;
	}

	unsigned char * ptr = (unsigned char *)malloc(sizeof(unsigned char) * (*size));
	if (!ptr)
	{
		return NULL;
	}

	memcpy(ptr, p, len);

	// 结束标志位 0x80
	ptr[len] = 0x80;
	// 补'0'操作
	for (int i = 0; i < fill; ++i)
	{
		ptr[len + i + 1] = (unsigned char)0x00;
	}
	//保存到最后的十六位中
	unsigned long long m_hex_size = len * 8;

	//memcpy(ptr[len + 2 + fill ], (unsigned char)m_hex_size, 8);
	ptr[strlen(p) + fill + 1] = (unsigned char)(m_hex_size >> 56);
	ptr[strlen(p) + fill + 2] = (unsigned char)((m_hex_size>> 48) & 0xFF);
	ptr[strlen(p) + fill + 3] = (unsigned char)((m_hex_size>> 40) & 0xFF);
	ptr[strlen(p) + fill + 4] = (unsigned char)((m_hex_size>> 32) & 0xFF);
	ptr[strlen(p) + fill + 5] = (unsigned char)((m_hex_size>> 24) & 0xFF);
	ptr[strlen(p) + fill + 6] = (unsigned char)((m_hex_size>> 16) & 0xFF);
	ptr[strlen(p) + fill + 7] = (unsigned char)((m_hex_size>> 8) & 0xFF);
	ptr[strlen(p) + fill + 8] = (unsigned char)(m_hex_size & 0xFF);


	return ptr;
}



int * get_sha1_update(unsigned char *ptr, unsigned long long size)
{
	if (!ptr || size == 0)
	{
		return NULL;
	}
	//计算有多少块
	int m_count = size / 64;
	int * temp = malloc(sizeof(int) * 80);
	if (!temp)
	{
		return NULL;
	}
	int * tempabcde = malloc(sizeof(int) * 5);
	if (!tempabcde)
	{
		return NULL;
	}
	int * h = malloc(sizeof(int) * 5);
	if (!h)
	{
		return NULL;
	}
	//memcpy(hex_enc, parm, 5);
	for (int i = 0; i < 5; ++i)
	{
		h[i] = parm[i];
	}
	for (int pos = 0; pos < m_count; pos++)
	{
		printf("pos = %d\n", pos);
		for (int i = 0; i < 16; i++)
		{
			temp[i] = chararraytoint(ptr, (pos * 64) + (i * 4));
			printf("[---> temp[%d] = %d]\n", i, temp[i]);
		}

		for (int t = 16; t <= 79; t++)
		{
			temp[t] = s(temp[t - 3] ^ temp[t - 8] ^ temp[t - 14] ^ temp[t - 16], 1);
			printf("m[%d]=%d\n", t, temp[t]);
		}
		for (int i = 0; i < 5; ++i)
		{
			tempabcde[i] = h[i];
		}

		for (int i = 0; i <= 19; i++)
		{
			int temp_1 = s(tempabcde[0], 5)
				+ f1(tempabcde[1], tempabcde[2], tempabcde[3])
				+ tempabcde[4]
				+ temp[i] + 0x5A827999;
			tempabcde[4] = tempabcde[3];
			tempabcde[3] = tempabcde[2];
			tempabcde[2] = s(tempabcde[1], 30);
			tempabcde[1] = tempabcde[0];
			tempabcde[0] = temp_1;
		}
		for (int i = 20; i <= 39; i++)
		{
			int temp_1 = s(tempabcde[0], 5)
				+ f2(tempabcde[1], tempabcde[2], tempabcde[3])
				+ tempabcde[4]
				+ temp[i] + 0x6ED9EBA1;
			tempabcde[4] = tempabcde[3];
			tempabcde[3] = tempabcde[2];
			tempabcde[2] = s(tempabcde[1], 30);
			tempabcde[1] = tempabcde[0];
			tempabcde[0] = temp_1;
		}
		for (int i = 40; i <= 59; i++) {
			int temp_1 = s(tempabcde[0], 5)
				+ f3(tempabcde[1], tempabcde[2], tempabcde[3])
				+ tempabcde[4]
				+ temp[i] + 0x8F1BBCDC;
			tempabcde[4] = tempabcde[3];
			tempabcde[3] = tempabcde[2];
			tempabcde[2] = s(tempabcde[1], 30);
			tempabcde[1] = tempabcde[0];
			tempabcde[0] = temp_1;
		}
		for (int i = 60; i <= 79; i++) {
			int temp_1 = s(tempabcde[0], 5)
				+ f4(tempabcde[1], tempabcde[2], tempabcde[3])
				+ tempabcde[4]
				+ temp[i] + 0xCA62C1D6;
			tempabcde[4] = tempabcde[3];
			tempabcde[3] = tempabcde[2];
			tempabcde[2] = s(tempabcde[1], 30);
			tempabcde[1] = tempabcde[0];
			tempabcde[0] = temp_1;
		}
		//5.令 H0 = H0 + A, H1 = H1 + B, H2 = H2 + C, H3 = H3 + D, H4 = H4 + E.
		for (int i = 0; i < 5; i++)
		{
			h[i] = h[i] + tempabcde[i];
			printf("[h[%d] = %d]\n", i, h[i]);
		}
		//完成了一次操作
		//清除之前的内容，开始下一个块的计算
		for (int i = 0; i < 80; i++)
		{
			temp[i] = 0;
		}
	}
	free(tempabcde);
	free(temp);
	return h;
}
int main(int argc, char *argv[])
{

	char * p = "abc";//616263   a=> 61, b => 62, c => 63
	
	unsigned long long size = 0;
	unsigned char * ptr = get_src_hex(p, strlen(p), &size);
	if (!ptr)
	{
		printf("[errro]\n");
		//system("pause");
		return -1;
	}
	printf("[");
	//61626380000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000018
	for (size_t i = 0; i < size; i++)
	{
		printf("%02x", ptr[i]);// 616263
	}
	printf("]\n");
	printf("strlen(ptr) = %zd\n", strlen(ptr));

	
	printf("----------------\n[");
	unsigned char hash[20];
	int * h = get_sha1_update(ptr, size);
	if (!h)
	{
		printf("[error]\n");
		//system("pause");
		return -1;
	}
	for (int i = 0; i < 5; ++i)
	{
		hash[(i * 4)] =(char)(( h [i]>>  24) & 0xff);
		hash[(i * 4) + 1] = (char)((h[i] >>  16) & 0xff);
		hash[(i * 4) + 2] = (char)((h[i] >>  8) & 0xff);
		hash[(i * 4) + 3] = (char)((h[i] & 0xff));
	}
	//memcpy(hash, h, 20);
	for (int i = 0; i < 20; ++i)
	{
		printf("%02x", hash[i]);// 616263
	}
	printf("]\n");
	//memcpy(ptr + strlen(p), 0x80, 1);
	//memcpy()
	free(h);
	free(ptr);
	
	//system("pause");
	return EXIT_SUCCESS;
}