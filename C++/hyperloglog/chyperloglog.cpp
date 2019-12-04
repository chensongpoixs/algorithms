/***********************************************************************************************
				created: 		2019-12-03

				author:			chensong

				purpose:		chyperloglog
************************************************************************************************/



#include <iostream>
#include <string>
static const int  HLL_P = 14; /* The greater is P, the smaller the error. */
static const int HLL_Q = 50; /* The number of bits of the hash value used for*/

static const int HLL_BITS = 6;
static const int HLL_REGISTERS = 16384;
static const int HLL_DENSE_SIZE = (HLL_REGISTERS*HLL_BITS + 7) / 8;
static const int HLL_TEST_CYCLES = 1000;
static const int HLL_P_MASK = 16383; //  ‭0011 1111 1111 1111‬
static const int HLL_REGISTER_MAX =  63; // 0011 1111

uint8_t  *_registers;//data bytes


					 /* Our hash function is MurmurHash2, 64 bit version.
					 * It was modified for Redis in order to provide the same result in
					 * big and little endian archs (endian neutral). */
uint64_t MurmurHash64A(const void * key, int len, unsigned int seed) {
	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;
	uint64_t h = seed ^ (len * m);
	const uint8_t *data = (const uint8_t *)key;
	const uint8_t *end = data + (len - (len & 7));

	while (data != end) {
		uint64_t k;


		k = (uint64_t)data[0];
		k |= (uint64_t)data[1] << 8;
		k |= (uint64_t)data[2] << 16;
		k |= (uint64_t)data[3] << 24;
		k |= (uint64_t)data[4] << 32;
		k |= (uint64_t)data[5] << 40;
		k |= (uint64_t)data[6] << 48;
		k |= (uint64_t)data[7] << 56;


		k *= m;
		k ^= k >> r;
		k *= m;
		h ^= k;
		h *= m;
		data += 8;
	}

	switch (len & 7) {
	case 7: h ^= (uint64_t)data[6] << 48; /* fall-thru */
	case 6: h ^= (uint64_t)data[5] << 40; /* fall-thru */
	case 5: h ^= (uint64_t)data[4] << 32; /* fall-thru */
	case 4: h ^= (uint64_t)data[3] << 24; /* fall-thru */
	case 3: h ^= (uint64_t)data[2] << 16; /* fall-thru */
	case 2: h ^= (uint64_t)data[1] << 8; /* fall-thru */
	case 1: h ^= (uint64_t)data[0];
		h *= m; /* fall-thru */
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;
	return h;
}



/* Given a string element to add to the HyperLogLog, returns the length
* of the pattern 000..1 of the element hash. As a side effect 'regp' is
* set to the register index this element hashes to. */
int hllPatLen(unsigned char *ele, size_t elesize, long *regp) {
	uint64_t hash, bit, index;
	int count;

	/* Count the number of zeroes starting from bit HLL_REGISTERS
	* (that is a power of two corresponding to the first bit we don't use
	* as index). The max run can be 64-P+1 = Q+1 bits.
	*
	* Note that the final "1" ending the sequence of zeroes must be
	* included in the count, so if we find "001" the count is 3, and
	* the smallest count possible is no zeroes at all, just a 1 bit
	* at the first position, that is a count of 1.
	*
	* This may sound like inefficient, but actually in the average case
	* there are high probabilities to find a 1 after a few iterations. */
	hash = MurmurHash64A(ele, elesize, 0xadc83b19ULL);
	// 浣庣殑14浣嶄綔涓轰笅鏍囩储寮? 
	index = hash & HLL_P_MASK;/*鈥?011 1111 1111 1111鈥?/; /* Register index. */
							  //鍙栭珮鐨?0浣嶆暟鎹綔涓篶ount鏁版嵁鐨勬搷浣?	//00 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 
	hash >>= HLL_P; /* 楂樹簬14浣?鏁版嵁淇濈暀涓嬫潵 Remove bits used to address the register. */
					// 鎶婄50浣嶆敼涓? 鍐冲畾count鐨勬渶澶у€兼槸 50
					//10 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 
	hash |= ((uint64_t)1 << HLL_Q); /* Make sure the loop terminates
									and count will be <= Q+1. */
	bit = 1; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0001
	count = 1; /* Initialized to 1 since we count the "00000...1" pattern. */
	while ((hash & bit) == 0) {
		count++; //涓轰粈涔堣杩欐牱鍒跺畾count鐨勫€?        
		bit <<= 1;
	}
	*regp = (int)index;
	return count;
}


void HLL_DENSE_GET_REGISTER(uint8_t *target, uint8_t ** p, long  regnum)
{
	uint8_t *_p = (uint8_t*)* p;
	unsigned long _byte = regnum*HLL_BITS / 8;
	unsigned long _fb = regnum*HLL_BITS & 7;
	unsigned long _fb8 = 8 - _fb;
	unsigned long b0 = _p[_byte];
	unsigned long b1 = _p[_byte + 1];
	*target = ((b0 >> _fb) | (b1 << _fb8)) & HLL_REGISTER_MAX;
}

void HLL_DENSE_SET_REGISTER(uint8_t **p, long regnum, uint8_t val)
{
	uint8_t *_p = (uint8_t*)*p;
	unsigned long _byte = regnum*HLL_BITS / 8;
	unsigned long _fb = regnum*HLL_BITS & 7;
	unsigned long _fb8 = 8 - _fb;
	unsigned long _v = val;
	_p[_byte] &= ~(HLL_REGISTER_MAX << _fb);
	_p[_byte] |= _v << _fb;
	_p[_byte + 1] &= ~(HLL_REGISTER_MAX >> _fb8);
	_p[_byte + 1] |= _v >> _fb8;
}

/* Low level function to set the dense HLL register at 'index' to the
* specified value if the current value is smaller than 'count'.
*
* 'registers' is expected to have room for HLL_REGISTERS plus an
* additional byte on the right. This requirement is met by sds strings
* automatically since they are implicitly null terminated.
*
* The function always succeed, however if as a result of the operation
* the approximated cardinality changed, 1 is returned. Otherwise 0
* is returned. */
int hllDenseSet(uint8_t *registers, long index, uint8_t count) {
	uint8_t oldcount;

	HLL_DENSE_GET_REGISTER(&oldcount, &registers, index);
	if (count > oldcount) {
		HLL_DENSE_SET_REGISTER(&registers, index, count);
		return 1;
	}
	else {
		return 0;
	}
}


/**
*
*/
int hllDenseAdd(uint8_t *registers, unsigned char *ele, size_t elesize)
{
	long index;
	uint8_t count = hllPatLen(ele, elesize, &index);
	/* Update the register if this element produced a longer run of zeroes. */
	return hllDenseSet(registers, index, count);
}




static const char s_hex_digits[] = "0123456789ABCDEF";
static void byte2hex(char* dst_buf, const unsigned char *src_buf, int buf_len)
{
	for (int i = 0; i < buf_len; ++i)
	{
		*dst_buf++ = s_hex_digits[src_buf[i] / 16];
		*dst_buf++ = s_hex_digits[src_buf[i] % 16];
	}
	*dst_buf = '\0';
}


void hllDenseRegHisto(uint8_t *registers, int* reghisto) {
	int j;


	uint8_t *r = registers;
	unsigned long r0, r1, r2, r3, r4, r5, r6, r7, r8, r9,
		r10, r11, r12, r13, r14, r15;
	// 一共内存的大小是 86016  ---> 下面统计时使用的内存大小是 12288--->还有没有使用的内存？？怎么处理呢
	for (j = 0; j < 1024; j++) {
		/* Handle 16 registers per iteration. */
		r0 = r[0] & 63; // 63 => 0011 1111
		r1 = (r[0] >> 6 | r[1] << 2) & 63;
		r2 = (r[1] >> 4 | r[2] << 4) & 63;
		r3 = (r[2] >> 2) & 63;
		r4 = r[3] & 63;
		r5 = (r[3] >> 6 | r[4] << 2) & 63;
		r6 = (r[4] >> 4 | r[5] << 4) & 63;
		r7 = (r[5] >> 2) & 63;
		r8 = r[6] & 63;
		r9 = (r[6] >> 6 | r[7] << 2) & 63;
		r10 = (r[7] >> 4 | r[8] << 4) & 63;
		r11 = (r[8] >> 2) & 63;
		r12 = r[9] & 63;
		r13 = (r[9] >> 6 | r[10] << 2) & 63;
		r14 = (r[10] >> 4 | r[11] << 4) & 63;
		r15 = (r[11] >> 2) & 63;

		reghisto[r0]++;
		reghisto[r1]++;
		reghisto[r2]++;
		reghisto[r3]++;
		reghisto[r4]++;
		reghisto[r5]++;
		reghisto[r6]++;
		reghisto[r7]++;
		reghisto[r8]++;
		reghisto[r9]++;
		reghisto[r10]++;
		reghisto[r11]++;
		reghisto[r12]++;
		reghisto[r13]++;
		reghisto[r14]++;
		reghisto[r15]++;
		// 指针数组增加移动
		r += 12;
	}

}
double hllTau(double x) {
	if (x == 0. || x == 1.) return 0.;
	double zPrime;
	double y = 1.0;
	double z = 1 - x;
	do {
		x = sqrt(x);
		zPrime = z;
		y *= 0.5;
		z -= pow(1 - x, 2)*y;
	} while (zPrime != z);
	return z / 3;
}

double hllSigma(double x) {
	if (x == 1.)
	{
		return INFINITY;
	}
	double zPrime;
	double y = 1;
	double z = x;
	do {
		x *= x;
		zPrime = z;
		z += x * y;
		y += y;
	} while (zPrime != z);
	return z;
}

#define HLL_ALPHA_INF 0.721347520444481703680 /* constant for 0.5/ln(2) */
uint64_t hllCount(uint8_t *registers, int *invalid) {
	double m = HLL_REGISTERS;
	double E;
	int j;
	int reghisto[HLL_Q + 2] = { 0 };  // 52

									  /* Compute register histogram */
	hllDenseRegHisto(registers, reghisto);
	//1. 调和平均数
	// 调和平均数 公式 = n((1/a1 + 1/a2 + ... + 1/an)/2)
	double z = m * hllTau((m - reghisto[HLL_Q + 1]) / (double)m);
	//仔细观察    没有把第一个数据reghisto[0]和最后一个数据reghisto[51] 加入进入  ---> 正态分布
	for (j = HLL_Q; j >= 1; --j) {
		z += reghisto[j];
		z *= 0.5;
	}
	//2. 标准误差 计算
	z += m * hllSigma(reghisto[0] / (double)m);
	// 调和平均数使用 0.5/ln(2)
	E = llroundl(HLL_ALPHA_INF*m*m / z);

	return (uint64_t)E;
}



/**
*
*理论布隆过滤器的错误的可能性 = 1.04/sqrt(16384)
*/
bool hyperloglog_test()
{
	_registers = (uint8_t *)::malloc(HLL_DENSE_SIZE);
	if (!_registers)
	{
		std::cerr << "[error][alloc error]" << std::endl;
		return  -1;
	}
	memset(_registers, 0, HLL_DENSE_SIZE);
	//注册桶的信息
	uint8_t bytecounters[HLL_REGISTERS];

	// 1 测试存储桶的信息是否正确
	for (int j = 0; j < HLL_TEST_CYCLES; ++j)
	{
		for (int i = 0; i < HLL_REGISTERS; ++i)
		{
			unsigned int r = rand() & HLL_REGISTER_MAX;

			bytecounters[i] = r;
			HLL_DENSE_SET_REGISTER(&_registers, i, r);
		}

		for (int i = 0; i < HLL_REGISTERS; ++i)
		{
			uint8_t val;

			HLL_DENSE_GET_REGISTER(&val, &_registers, i);
			if (val != bytecounters[i])
			{
				printf("[%s][%d]TESTFAILED Register %d should be %d but is %d\n", __FUNCTION__, __LINE__, i, (int)bytecounters[i], (int)val);
				return false;
			}
		}
	}


	memset(_registers, 0, HLL_DENSE_SIZE);
	//2.  错误可能性 
	double relerr = 1.04 / sqrt(HLL_REGISTERS);
	int64_t checkpoint = 1;
	uint64_t seed = (uint64_t)rand() | (uint64_t)rand() << 32;
	uint64_t ele;
	
	for (int j = 1; j <= 10000000; ++j)
	{
		ele = j ^ seed;
		hllDenseAdd(_registers, (unsigned char *)&ele, sizeof(ele));

		/* Check error. */
		if (j == checkpoint)
		{
			//计算误差值
			int64_t abserr = checkpoint - (int64_t)hllCount(_registers, NULL);
			uint64_t maxerr = ceil(relerr * 6 * checkpoint);//返回大于或者等于指定表达式的最小整数

			if (j == 10)
			{
				maxerr = 1;
			}

			if (abserr < 0)
			{
				abserr = -abserr;
			}
			//判断是否大于错误可能性
			if (abserr >(int64_t)maxerr)
			{
				printf("[%s][%d]TESTFAILED Too big error. card:%llu abserr:%llu\n", __FUNCTION__, __LINE__, (unsigned long long) checkpoint, (unsigned long long) abserr);
				return false;
			}
			checkpoint *= 10;
		}

	}


	long count = hllCount(_registers, NULL);
	printf("[count = %ld]\n", count);
	//printf("[hex =\n");
	//for (int i = 0; i < HLL_DENSE_SIZE; ++i)
	//{
	//	printf("%c", s_hex_digits[_registers[i] / 16]);
	//	if (i % 20 == 0)
	//	{
	//		printf("\n");
	//	}
	//}
	//printf("]\n");

	if (_registers)
	{
		::free(_registers);
	}
	return true;
}






int main(int argc, char* argv[])
{

	if (hyperloglog_test())
	{
		printf("[info]ok\n");
	}
	else
	{
		printf("[error] fail!!!\n");
	}
	return EXIT_SUCCESS;
}