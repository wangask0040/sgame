#pragma once
#include <vector>
#include "game_base.pb.h"
#include "common/tinyxml2.h"

const unsigned int AWARD_FLAG_64_BIT_SIZE = 64;
const unsigned int AWARD_FLAG_32_BIT_SIZE = 32;

struct ID_AND_NUM
{
	unsigned int id = 0;
	unsigned int num = 0;
	bool operator < (const ID_AND_NUM& st) const { return num < st.num; }
	bool operator == (unsigned int godid) const { return id == godid; }
};

using VEC_ID_AND_NUM = std::vector<ID_AND_NUM>;

struct TYPE_AND_PARAM
{
	unsigned int type;
	unsigned int id;
	unsigned int min;
	unsigned int max;
	unsigned int prob;

	TYPE_AND_PARAM();
	void Reset();
	void Check() const;
	unsigned int Value() const;
	unsigned int Id() const;
	unsigned int DispMin() const;
	unsigned int DispMax() const;

	bool operator == (const TYPE_AND_PARAM& st) const;
	void operator += (const TYPE_AND_PARAM& st);
};

using VEC_TYPE_AND_PARAM = std::vector<TYPE_AND_PARAM>;
using REPEATED_AWARD_UNIT = ::google::protobuf::RepeatedPtrField< ::protocol::award_unit >;

void AwardUnitToTP(const protocol::award_unit& award, TYPE_AND_PARAM& st);
void TPToAwardUnit(const TYPE_AND_PARAM& st, protocol::award_unit& award);
bool ReadTypeAndParam(const tinyxml2::XMLElement* item, VEC_TYPE_AND_PARAM& vec);
bool ReadTypeAndParamByPrefix(const tinyxml2::XMLElement* item, VEC_TYPE_AND_PARAM& vec, const std::string& prefix);
void MergeTypeAndParam(const VEC_TYPE_AND_PARAM& before, VEC_TYPE_AND_PARAM& after);
unsigned int RandTypeAndParam(const VEC_TYPE_AND_PARAM& vec);//多个中随机出一个
void MergeVecTPToRPAward(const VEC_TYPE_AND_PARAM& awardsParam, REPEATED_AWARD_UNIT& pbAwards);
