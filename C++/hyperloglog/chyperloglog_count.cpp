/***********************************************************************************************
				created: 		2019-12-07

				author:			chensong

				purpose:		布隆过滤器的正态分布图分析 桶的中count分析的关系分配
				 
************************************************************************************************/



#include <iostream>
#include <string>
#include <fstream>

#include <iostream>
#include <string>
static const int  HLL_P = 14; /* The greater is P, the smaller the error. */
static const int HLL_Q = 50; /* The number of bits of the hash value used for*/

static const int HLL_BITS = 6;
static const int HLL_REGISTERS = 16384;   // 分桶数 = 2^p = 16384;
static const int HLL_DENSE_SIZE = (HLL_REGISTERS*HLL_BITS + 7) / 8;
static const int HLL_TEST_CYCLES = 1000;
static const int HLL_P_MASK = 16383; //  ‭0011 1111 1111 1111‬
static const int HLL_REGISTER_MAX =  63; // 0011 1111

uint8_t  *_registers;//data bytes


					 /* Our hash function is MurmurHash2, 64 bit version.
					 * It was modified for Redis in order to provide the same result in
					 * big and little endian archs (endian neutral). */
uint64_t MurmurHash64A(const void * key, int len, unsigned int seed) 
{
	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;
	uint64_t h = seed ^ (len * m);
	const uint8_t *data = (const uint8_t *)key;
	const uint8_t *end = data + (len - (len & 7));

	while (data != end) 
	{
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

	switch (len & 7) 
	{
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
int hllPatLen(unsigned char *ele, size_t elesize, long *regp) 
{
	uint64_t hash, bit, index;
	int count;


	hash = MurmurHash64A(ele, elesize, 0xadc83b19ULL);
	
	index = hash & HLL_P_MASK;
	hash >>= HLL_P;
	hash |= ((uint64_t)1 << HLL_Q); 
	bit = 1; 
	count = 1;
	while ((hash & bit) == 0) 
	{
		count++;        
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
int hllDenseSet(uint8_t *registers, long index, uint8_t count) 
{
	uint8_t oldcount;

	HLL_DENSE_GET_REGISTER(&oldcount, &registers, index);
	if (count > oldcount)
	{
		HLL_DENSE_SET_REGISTER(&registers, index, count);
		return 1;
	}
	else 
	{
		return 0;
	}
}


/**
* 插入一条数据
*/
int hllDenseAdd(uint8_t *registers, unsigned char *ele, size_t elesize)
{
	long index;
	uint8_t count = hllPatLen(ele, elesize, &index);
	/* Update the register if this element produced a longer run of zeroes. */
	return hllDenseSet(registers, index, count);
}

void hllDenseRegHisto(uint8_t *registers, const char * count_file_name) 
{
	int j;
	int index = 0;
	uint8_t *r = registers;
	unsigned long r0, r1, r2, r3, r4, r5, r6, r7, r8, r9,
		r10, r11, r12, r13, r14, r15;
	// 一共内存的大小是 86016  ---> 下面统计时使用的内存大小是 12288--->还有没有使用的内存？？怎么处理呢
	std::ofstream			m_fd_count;
	

	if (!m_fd_count.is_open())
	{
		m_fd_count.open(std::string(count_file_name).append(".csv").c_str(), std::ios::out | std::ios::trunc);
		if (!m_fd_count.is_open())
		{

			std::cout << "[m_fd_count]not open log file dest url =" << count_file_name << std::endl;
			return;
		}
	}
	m_fd_count << "index, count" << std::endl;
	m_fd_count.flush();
	
	for (j = 0; j < 1024; ++j) 
	{
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
		
		
		index = j * 16;
		m_fd_count << index  		<< "," << r0 << std::endl;
		m_fd_count << (index + 1 ) 	<< "," << r1 << std::endl;
		m_fd_count << (index  + 2) 	<< "," << r2 << std::endl;
		m_fd_count << (index  + 3)	<< "," << r3 << std::endl;
		m_fd_count << (index  +4)	<< "," 	<< r4 << std::endl;
		m_fd_count << (index  +5)	<< "," 	<< r5 << std::endl;
		m_fd_count << (index  +6)	<< "," 	<< r6 << std::endl;
		m_fd_count << (index  +7)	<< "," 	<< r7 << std::endl;
		m_fd_count << (index  +8)	<< "," 	<< r8 << std::endl;
		m_fd_count << (index  +9)	<< "," 	<< r9 << std::endl;
		m_fd_count << (index  +10)	<< "," << r10 << std::endl;
		m_fd_count << (index  +11)	<< "," << r11 << std::endl;
		m_fd_count << (index  +12)	<< "," << r12 << std::endl;
		m_fd_count << (index  +13)	<< "," << r13 << std::endl;
		m_fd_count << (index  +14)	<< "," << r14 << std::endl;
		m_fd_count << (index  +15)	<< "," << r15 << std::endl;
		m_fd_count.flush();
		// 指针数组增加移动
		r += 12;
	}

	m_fd_count.flush();
	if (m_fd_count.is_open())
	{
		m_fd_count.flush();
		m_fd_count.close();
	}
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
	uint64_t seed = (uint64_t)rand() | (uint64_t)rand() << 32;
	uint64_t ele;
	long long m_count = 10000;
	// 插入数据
	for (long long j = 1; j <= 1000000000000; ++j)
	{
		ele = j ^ seed;
		hllDenseAdd(_registers, (unsigned char *)&ele, sizeof(ele));
		
		if  (j == m_count)
		{
			hllDenseRegHisto(_registers, std::string("index_count_").append(std::to_string(j)).c_str());
			m_count = j *10000;
		}
	}
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