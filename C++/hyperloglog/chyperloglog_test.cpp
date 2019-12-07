/***********************************************************************************************
				created: 		2019-12-03

				author:			chensong

				purpose:		布隆过滤器的正态分布图分析 桶的数量的关系分配
				 
************************************************************************************************/



#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
// 掩码
static const uint64_t m_mask = 1;
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



/**
* 测试hash的均匀性
* @param file_name
* @param p  :  桶的数量  使用是 位的 参数是: 10， 12, 14
*/
void test_hash(const char *file_name, const char * count_file_name, int p)
{
	uint64_t hash = 0;
	uint64_t mask = (m_mask << p) - 1;
	uint64_t index = 0;
	std::ofstream			m_fd;
	std::ofstream			m_fd_count;
	//const char *			m_count_file_name = "index_count7_100.csv";
	if (!m_fd.is_open())
	{
		m_fd.open(std::string(file_name).append(".csv").c_str(), std::ios::out | std::ios::trunc);
		if (!m_fd.is_open())
		{

			std::cout << "not open log file dest url = " << file_name  << std::endl;
			return;
		}
	}

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
	m_fd << "index, bucket, count" << std::endl;
	m_fd.flush();
	int temp = 0;
	for (int i = 0; i < 2000; ++i)
	{
		temp = i * 5;;
		hash = MurmurHash64A(&temp, sizeof(int), 0xadc83b19ULL);

		index = hash & mask;

		hash >>= p; 
		hash |= ((uint64_t)1 << (64 - p));
		int bit = 1;
		int count = 1;
		while ((hash & bit) == 0) {
			count++; 
			bit <<= 1;
		}
		
		m_fd <<  i << "," << index << ", " << count  << std::endl;
		m_fd_count << i << "," << count << std::endl;
	}
	
	m_fd.flush();
	if (m_fd.is_open())
	{
		m_fd.flush();
		m_fd.close();
	}
	m_fd_count.flush();
	if (m_fd_count.is_open())
	{
		m_fd_count.flush();
		m_fd_count.close();
	}
}



int main(int argc,  char *argv[])
{
	
	for (int i = 0; i < 3; ++i)
	{
		time_t start_time = ::time(NULL);
		test_hash(std::string("index_count_2000_").append(std::to_string(10+(2*i))).c_str(), std::string("count_file_name_2000_").append(std::to_string(10+ (2 * i))).c_str(), 10 + (2 * i));
		time_t end_time = ::time(NULL);
		time_t scend_time = end_time - start_time;
		printf("[i= %d][secend = %d]\n", i, scend_time );
	}
	
	return EXIT_SUCCESS;
}