#include "stringex.h"
#include <sstream>

using namespace std;

void split(const std::string& str, const std::string& delimiters,
	std::vector<std::string>& tokens)
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

void str2uint32(uint32_t &int_temp, const string &string_temp)
{
	stringstream stream(string_temp);
	stream >> int_temp;
}

void split2uint32(const std::string& str, const std::string& delimiters,
	std::vector<uint32_t>& tokens)
{
	std::vector<std::string> tmp_str;
	split(str, delimiters, tmp_str);
	for (uint32_t i = 0; i < tmp_str.size(); i++) {
		uint32_t tmp_int;
		str2uint32(tmp_int, tmp_str[i]);
		tokens.push_back(tmp_int);
	}
}

void str2int32(int32_t &int_temp, const string &string_temp)
{
	stringstream stream(string_temp);
	stream >> int_temp;
}

void split2int32(const std::string& str, const std::string& delimiters,
	std::vector<int32_t>& tokens)
{
	std::vector<std::string> tmp_str;
	split(str, delimiters, tmp_str);
	for (uint32_t i = 0; i < tmp_str.size(); i++) {
		int32_t tmp_int;
		str2int32(tmp_int, tmp_str[i]);
		tokens.push_back(tmp_int);
	}
}

void str2float(float &int_temp, const string &string_temp)
{
	stringstream stream(string_temp);
	stream >> int_temp;
}

void split2float(const std::string& str, const std::string& delimiters,
	std::vector<float>& tokens)
{
	std::vector<std::string> tmp_str;
	split(str, delimiters, tmp_str);
	for (uint32_t i = 0; i < tmp_str.size(); i++) {
		float tmp_int;
		str2float(tmp_int, tmp_str[i]);
		tokens.push_back(tmp_int);
	}
}