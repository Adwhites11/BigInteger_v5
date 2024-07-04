#include "BigIntToString.h"
#include <iostream>

std::vector<char> ToString::add(std::vector<char> op1, std::vector<char> op2)
{
	std::vector<char> ret;
	if (is_bigger_or_eq(op1, op2))
	{
		ret = op1;
		for (int i = ret.size() - 1, j = op2.size() - 1;
			j >= 0; i--, j--)
		{
			ret[i] += op2[j];
			if (ret[i] >= 10)
			{
				if (j == 0 && i == 0)
				{
					ret.insert(ret.begin(), ret[i] / 10);
					ret[i + 1] = ret[i + 1] % 10;
					break;
				}
				ret[i - 1] += ret[i] / 10;
				ret[i] = ret[i] % 10;
				int k = i - 1;
				while (ret[k] > 9 && k != 0)
				{
					ret[k - 1] += ret[k] / 10;
					ret[k] = ret[k] % 10;
				}
			}
		}
	}
	else
	{
		std::vector<char> temp1 = op2;
		std::vector<char> temp2 = op1;
		for (int i = temp1.size() - 1, j = temp2.size() - 1;
			j >= 0; i--, j--)
		{
			temp1[i] += temp2[j];
			if (temp1[i] >= 10)
			{
				if (j == 0 && i == 0)
				{
					temp1.insert(temp1.begin(), temp1[i] / 10);
					temp1[i + 1] = temp1[i + 1] % 10;
					break;
				}
				temp1[i - 1] += temp1[i] / 10;
				temp1[i] = temp1[i] % 10;
				int k = i - 1;
				while (temp1[k] > 9 && k != 0)
				{
					temp1[k - 1] += temp1[k] / 10;
					temp1[k] = temp1[k] % 10;
				}
			}
		}
		ret = temp1;
	}
	return ret;
}

std::vector<char> ToString::mul(std::vector<char> op1, std::vector<char> op2)
{
	std::vector<char> ret;
	std::vector<char>* p_op1;
	std::vector<char>* p_op2;
	//int i, j;
	int i_ = 0;
	if (is_bigger_or_eq(op1, op2))
	{
		//i = op1.size() - 1;
		p_op1 = &op1;
		//j = op2.size() - 1;
		p_op2 = &op2;
	}
	else
	{
		//i = op2.size() - 1;
		p_op1 = &op2;
		//j = op1.size() - 1;
		p_op2 = &op1;
	}
	ret = std::vector<char>(op1.size() + op2.size(), 0);
	char temp;
	for (int i = p_op1->size() - 1; i >= 0; i--, i_++)
	{
		for (int j = p_op2->size() - 1, k = ret.size() - 1; j >= 0; j--, k--)
		{
			temp = p_op1->at(i) * p_op2->at(j);
			ret[k - i_] += temp;
			if (ret[k - i_] >= 10)
			{
				int c = k - i_;
				while (ret[c] >= 10)
				{
					if (i == 0)
					{
						ret.insert(ret.begin(), 0);
						c++;
						k++;
					}
					ret[c - 1] += ret[c] / 10;
					ret[c] = ret[c] % 10;
					c--;
				}
			}

		}
	}
	while (*ret.begin() == 0) ret.erase(ret.begin());
	return ret;
}
std::vector<char> ToString::sub(std::vector<char> op1, std::vector<char> op2)
{
	for (int i = op1.size() - 1, j = op2.size() - 1;
		j >= 0; i--, j--)
	{
		if (op1[i] < op2[j] && i > 0)
		{
			op1[i] += 10;
			int k = i - 1;
			while (op1[k] == 0 && k > 0)
			{
				op1[k] = 9;
				k--;
			}
			op1[k] -= 1;
			op1[i] -= op2[j];
		}
		else
			op1[i] -= op2[j];
	}

	while (op1.size() > 0 && op1[0] == 0)
		op1.erase(op1.begin());

	if (op1.size() == 0)
		op1.push_back(0);

	return op1;
}

bool ToString::is_bigger_or_eq(const std::vector<char>& op1, const std::vector<char>& op2)
{
	if (op1.size() > op2.size())
		return true;
	else if (op1.size() == op2.size())
	{
		for (int i = 0; i < op1.size(); i++)
		{
			if (op1[i] > op2[i])
				return true;
			else if (op1[i] < op2[i]) return false;
		}
		return true;
	}
	else
		return false;
}
std::vector<char> ToString::pow(std::vector<char> op, uint64_t e)
{
	if (e == 0) return std::vector<char>(1, 1);
	else if (e == 1) return op;
	std::vector<char> ret;
	//ret.push_back(1);

	int start = 0;
	bool binary_form[64] = { false };
	for (int i = 63; i >= 0; i--)
	{
		binary_form[i] = e % 2;
		e /= 2;
	}
	for (; start < 64; start++) if (binary_form[start]) break;

	ret = mul(op, op);

	for (int i = start + 1; i < 63; i++)
	{
		if (binary_form[i])
		{
			ret = mul(ret, op);
			ret = mul(ret, ret);
		}
		else
			ret = mul(ret, ret);
	}

	if (binary_form[63]) ret = mul(ret, op);

	return ret;
}

std::string ToString::get_string(std::vector<bool> binary_represent)
{
	int true_count = 0;
	for (std::vector<bool>::iterator it =
		binary_represent.begin(); it != binary_represent.end(); ++it)
		if (*it) true_count++;
	if (true_count == 0) return std::string("0");
	std::vector<char> ret = { 0 };
	for (long long i = binary_represent.size() - 1, e = 0; i >= 0; i--, e++)
	{
		if (binary_represent[i])
		{
			ret = add(ret, pow(std::vector<char>(1, 2), e));
		}
	}
	std::string end;
	char buf[4] = { 0 };
	for (int i = 0; i < ret.size(); i++)
	{
		memset(buf, 0, 4);
		_itoa_s(ret[i], buf, 10);
		end.append(buf);
	}
	return end;
}


std::vector<bool> ToString::get_binar_repr(std::string item)
{
	std::vector<bool> bbi;
	bool yes = false;
	std::vector<char> temp;
	char t[2];
	t[1] = '\0';
	for (size_t i = 0; i < item.length(); i++)
	{
		t[0] = item[i];
		temp.push_back(atoi(t));
	}

	std::vector<char> temp2;
	std::vector<bool>().swap(bbi);
	int i = item.size() * 3.5;
	for (; i >= 0; i--)
	{
		temp2 = pow(std::vector<char>(1, 2), i);

		if (is_bigger_or_eq(temp, temp2))
		{
			yes = true;
			temp = sub(temp, temp2);
			bbi.push_back(true);
		}
		else
			if (yes)
				bbi.push_back(false);
	}

	return bbi;
}