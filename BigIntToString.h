#pragma once

#include <vector>
#include <string>



extern class ToString
{
private:
public:
	static std::vector<char> add(std::vector<char> op1, std::vector<char> op2);
	static std::vector<char> mul(std::vector<char> op1, std::vector<char> op2);
	static std::vector<char> sub(std::vector<char> op1, std::vector<char> op2);

	static bool is_bigger_or_eq(const std::vector<char>& op1, const std::vector<char>& op2);
	static std::vector<char> pow(std::vector<char> op, uint64_t e);
public:
	static std::string get_string(std::vector<bool> binary_represent);
	static std::vector<bool> get_binar_repr(std::string item);
};