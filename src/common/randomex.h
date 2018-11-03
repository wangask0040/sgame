
#pragma once


#include <vector>


struct RAND_UNIT
{
	unsigned int idx = 0;
	unsigned int prob = 0;
};

class CRandom
{
public:
	CRandom();
	~CRandom();
	explicit CRandom(unsigned int prob_base);
	//返回任意随机数
	static unsigned int Rand();
	//返回start和end之间的随机数，包括start和end
	static unsigned int Rand(unsigned int start, unsigned int end);
	//返回0到value之间的随机数,包括0和value
	static unsigned int Rand(unsigned int value);
	//给定一个概率数组，随机出其中一个，返回它的索引
	static int Rand(const std::vector<unsigned int>& vec);
	//给定一个概率数组，随机出其中多个，返回它的索引数组
	void Rand(const std::vector<unsigned int>& vec, unsigned int num,
		std::vector<unsigned int>& vecindex);
	//取基础概率
	unsigned int ProbBase() const { return _prob_base; }
	//基础概率
	static const unsigned int GAME_BASE_PROB = 10000;

private:
	int Rand(const std::vector<RAND_UNIT>& vec) const;
	unsigned int _prob_base = GAME_BASE_PROB;
};
