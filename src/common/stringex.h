#pragma once
#include <string>
#include <vector>

void split(const std::string& str, const std::string& delimiters, std::vector<std::string>& tokens);
void str2uint32(uint32_t &int_temp, const std::string &string_temp);
void split2uint32(const std::string& str, const std::string& delimiters, std::vector<uint32_t>& tokens);
void str2int32(int32_t &int_temp, const std::string &string_temp);
void split2int32(const std::string& str, const std::string& delimiters, std::vector<int32_t>& tokens);
void str2float(float &int_temp, const std::string &string_temp);
void split2float(const std::string& str, const std::string& delimiters, std::vector<float>& tokens);

