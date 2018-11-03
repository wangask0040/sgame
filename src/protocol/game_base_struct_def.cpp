#include "game_base_struct_def.h"
#include "common/logging.h"
#include "common/stringex.h"
#include "common/randomex.h"

using namespace protocol;

void TPToAwardUnit(const TYPE_AND_PARAM& st, protocol::award_unit& award)
{
	award.Clear();
	award.set_type(st.type);
	award.set_gid(st.id);
	award.set_val(st.min);
}

bool ReadTypeAndParam(const tinyxml2::XMLElement* item, VEC_TYPE_AND_PARAM& vec)
{
	return ReadTypeAndParamByPrefix(item, vec, std::string(""));
}

bool ReadTypeAndParamByPrefix(const tinyxml2::XMLElement* item, VEC_TYPE_AND_PARAM& vec, const std::string& prefix)
{
	std::vector<unsigned int> vectype;
	std::vector<unsigned int> vecgid;
	std::vector<unsigned int> vecmin;
	std::vector<unsigned int> vecmax;
	std::vector<unsigned int> vecprob;

	split2uint32(item->Attribute((prefix + "type").c_str()), DIRSPLIT, vectype);
	split2uint32(item->Attribute((prefix + "gid").c_str()), DIRSPLIT, vecgid);
	split2uint32(item->Attribute((prefix + "min").c_str()), DIRSPLIT, vecmin);
	split2uint32(item->Attribute((prefix + "max").c_str()), DIRSPLIT, vecmax);

	auto probstr = item->Attribute((prefix + "prob").c_str());
	if (probstr != nullptr)
		split2uint32(probstr, DIRSPLIT, vecprob);

	assert(vectype.size() == vecgid.size());
	assert(vecgid.size() == vecmin.size());
	assert(vecmin.size() == vecmax.size());
	if (vecprob.size() > 0)
		assert(vecmax.size() == vecprob.size());

	for (size_t i = 0; i < vectype.size(); ++i)
	{
		TYPE_AND_PARAM st;
		st.type = vectype[i];
		st.id = vecgid[i];
		st.min = vecmin[i];
		st.max = vecmax[i];
		assert(st.min <= st.max);
		if (vecprob.size() > 0)
			st.prob = vecprob[i];
		vec.push_back(st);
		st.Check();
	}
	return true;
}

TYPE_AND_PARAM::TYPE_AND_PARAM()
{
	Reset();
}

void TYPE_AND_PARAM::Reset()
{
	type = 0;
	id = 0;
	min = 0;
	max = 0;
	prob = 0;
}

void TYPE_AND_PARAM::Check() const
{
	assert(AWARD_TYPE_IsValid(type));
	assert(max >= min);
	assert(min > 0);
}

unsigned int TYPE_AND_PARAM::Value() const
{
	if (prob > 0)
	{
		auto pb = CRandom::Rand(CRandom::GAME_BASE_PROB);
		if (prob > pb)
			return CRandom::Rand(min, max);
		else
			return 0;
	}
	else
		return CRandom::Rand(min, max);
}

unsigned int TYPE_AND_PARAM::Id() const
{
	return id;
}

bool TYPE_AND_PARAM::operator == (const TYPE_AND_PARAM& st) const
{
	return st.type == type && st.id == id;
}

void TYPE_AND_PARAM::operator += (const TYPE_AND_PARAM& st)
{
	if (st == *this)
	{
		this->min += st.min;
		this->max += st.max;
	}
}

void MergeTypeAndParam(const VEC_TYPE_AND_PARAM& before, VEC_TYPE_AND_PARAM& after)
{
	for (auto& itb : before)
	{
		bool find = false;
		for (auto& ita : after)
		{
			if (itb == ita)
			{
				ita += itb;
				find = true;
				continue;
			}
		}

		if (!find)
		{
			after.push_back(itb);
		}
	}
}

void AwardUnitToTP(const protocol::award_unit& award, TYPE_AND_PARAM& st)
{
	st.type = award.type();
	st.id = award.gid();
	st.min = award.val();
	st.max = award.val();
}

void MergeVecTPToRPAward(const VEC_TYPE_AND_PARAM& awardsParam, REPEATED_AWARD_UNIT& pbAwards)
{
	pbAwards.Clear();
	VEC_TYPE_AND_PARAM merge;
	MergeTypeAndParam(awardsParam, merge);
	for (auto& it : merge)
	{
		TPToAwardUnit(it, *pbAwards.Add());
	}
}


unsigned int TYPE_AND_PARAM::DispMax() const
{
	return max;
}

unsigned int TYPE_AND_PARAM::DispMin() const
{
	if (prob > 0 && prob < CRandom::GAME_BASE_PROB)
		return 0;
	else
		return min;
}

unsigned int RandTypeAndParam(const VEC_TYPE_AND_PARAM& vec)
{
	std::vector<unsigned int> vecprob;
	for (auto& it : vec)
	{
		vecprob.push_back(it.prob);
	}

	return CRandom::Rand(vecprob);
}