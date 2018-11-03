/**
 * @file:   random_util.cpp
 * @brief:  随机工具类
 */

#include "randomex.h"
#include <iostream>
#include <vector>
#include <random>

CRandom::CRandom()
{
}


CRandom::~CRandom()
{
}

CRandom::CRandom(unsigned int prob_base)
{
	_prob_base = prob_base;
}

unsigned int CRandom::Rand(unsigned int start, unsigned int end)
{
	if (end > start)
	{
		unsigned int offset = (end - start);
		return start + Rand(offset);
	}
	else if (end == start)
		return start;
	else
		return 0;
}

unsigned int CRandom::Rand(unsigned int value)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	return mt() % (value + 1);
}

int CRandom::Rand(const std::vector<unsigned int>& vec)
{
	unsigned int total = 0;
	for (auto it : vec)
		total += it;

	unsigned int calc = Rand(total);
	unsigned int prob_cur = 0;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		prob_cur += vec[i];
		if (prob_cur >= calc)
			return i;
	}
	return -1;
}

void CRandom::Rand(const std::vector<unsigned int>& vec, unsigned int num,
	std::vector<unsigned int>& vecindex) 
{
	if (vec.size() == 0)
		return;

	if (num < vec.size())
	{
		std::vector<RAND_UNIT> bak;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			RAND_UNIT st;
			st.idx = i;
			st.prob = vec[i];
			bak.push_back(st);
		}

		for (unsigned int i = 0; i < num; ++i)
		{
			//随机
			auto idx = Rand(bak);

			//计算出索引
			vecindex.push_back(bak[idx].idx);

			//删除已经出现的项
			bak.erase(bak.begin() + idx);

			//计算随机基础概率
			_prob_base = 0;
			for (auto& it : bak)
				_prob_base += it.prob;
		}
	}
	else
	{
		//先将所有索引放入一遍
		for (unsigned int i = 0; i < vec.size(); ++i)
			vecindex.push_back(i);

		//余下的次数每次随机一遍
		auto f = (num - vec.size());
		for (unsigned int i = 0; i < f; ++i)
			vecindex.push_back(Rand(vec));
	}
}

unsigned int CRandom::Rand()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	return mt();
}

int CRandom::Rand(const std::vector<RAND_UNIT>& vec) const
{
	std::vector<unsigned int> tmp;
	for (auto& it : vec)
	{
		tmp.push_back(it.prob);
	}

	return Rand(tmp);
}