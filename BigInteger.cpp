#include "BigInteger.h"
#include <algorithm>
#include <ctime>
#include <thread>
#include <cstdlib>
#include <intrin.h>

inline constexpr long long now_time( ) {
	return std::chrono::high_resolution_clock::now( ).
		time_since_epoch( ).count( );
}

BigInteger BigInteger::add(const BigInteger& X, const BigInteger& Y)
{
	BigInteger bi = BigInteger( );
	std::vector<uint64_t>& p_op2 =
		const_cast<std::vector<uint64_t>&>(X.data);
	bool carry = false;
	//char carry = 0;
	size_t min;
	if (X.data.size( ) <= Y.data.size( ))
	{
		min = X.data.size( );
		bi.data = Y.data;
		p_op2 = X.data;
	}
	else
	{
		min = Y.data.size( );
		bi.data = X.data;
		p_op2 = Y.data;
	}

	for (size_t i = 0; i < min; i++)
	{
		bi.data[i] += p_op2.at(i);
		carry = __readeflags( ) & static_cast<uint64_t>(1);
		if (carry)
		{
			size_t j = i + 1;
			do
			{
				if (j == bi.data.size( ))
					bi.data.push_back(0);
				bi.data[j]++;
				carry = __readeflags( ) & static_cast<uint64_t>(1);
				j++;
			} while (carry);
		}

	}

	while (bi.data.size( ) > 0 && bi.data[bi.data.size( ) - 1] == 0) bi.data.erase(bi.data.end( ) - 1);
	return bi;
}
BigInteger BigInteger::sub(const BigInteger& X, const BigInteger& Y)
{
	BigInteger bi = 0;
	std::vector<uint64_t> p_op2;
	//uint64_t temp1, temp2;

	uint8_t counter = 0;
	int64_t j = 0;
	uint64_t ttt = 0;
	uint64_t mask = 1;
	bool carry = false;

	if (X.data.size( ) >= Y.data.size( ))
	{
		bi = X;
		p_op2 = Y.data;
	}
	else
	{
		bi = Y;
		p_op2 = X.data;
	}

	for (int64_t i = 0; i < p_op2.size( ); i++)
	{
		//temp1 = bi.data[i];
		//temp2 = p_op2.at(i);
		bi.data[i] -= p_op2.at(i);
		//if (temp1 < temp2)
		carry = __readeflags( ) & static_cast<uint64_t>(1);
		if (carry)
		{
			mask = 1;
			counter = 0;
			// найти четверное слово, в котором можно произвести заём бита
			j = i + 1;
			while (j < bi.data.size( ) && bi.data[j] == 0) j++;
			// сохранить qword, чтобы найти, где самый младший единичный бит
			ttt = bi.data[j];
			// ищем бит
			// смещаем временное значение вправо, пока первый бит != 1
			// вместе с этим counter сохраняет позицию бита
			// mask заполняется единицами в младших разрядах

			while (!(ttt & 0x1))
			{
				ttt >>= 1;
				counter++;
				mask <<= 1;
				mask |= 1;
			}
			mask >>= 1;
			
			// удаляем единицу в том месте, где мы её заняли
			bi.data[j] &= (UINT64_MAX ^ (static_cast<uint64_t>(1) << counter));
			// заполняем единицами все разряды, младше того, из которого единицу достали
			bi.data[j] |= mask;

			// если есть квадро слова, равные 0 и младшие того, из которого заняли единицу, заполняем их битовые поля 1
			for (j -= 1; j > i; j--)
				bi.data[j] = UINT64_MAX;


			//// нашли ближайшее слово, которое больше 0 и привел его в биты
			//bool binary[ELEMENT_SIZE] = { false };
			//BigInteger::uint64_to_binar(bi.data.at(j), binary);
			//// найти, начиная с младшего бита, единичный бит
			//int k = ELEMENT_SIZE - 1;
			//for (; k >= 0; k--) if (binary[k]) break;
			//// нашли и сделали его 0
			//binary[k] = false;
			//// остальные стали 1
			//for (k = k + 1; k < ELEMENT_SIZE; k++)
			//	binary[k] = true;

			//// все слова до того, в котором возникла проблема
			//// получают все биты = 1
			//// с проблематичным словом проблема будет решена 
			//// компьютером
			//bi.data[j] = BigInteger::binar_to_uint64(binary);
			//for (j -= 1; j >= i; j--)
			//{
			//	memset(binary, 1, sizeof(binary));
			//	bi.data[j] = BigInteger::binar_to_uint64(binary);
			//}
		}
		//bi.data[i] = temp1 - temp2;
	}
	while (bi.data.size( ) > 0 && bi.data[bi.data.size( ) - 1] == 0) bi.data.erase(bi.data.end( ) - 1);
	return bi;
}


std::vector<bool> BigInteger::uint64_to_bool_vector(uint64_t x)
{
	bool ret[ELEMENT_SIZE] = { 0 };
	for (int8_t i = ELEMENT_SIZE - 1, j = 0; i >= 0; i--, j++)
		ret[i] = (x >> j)& static_cast<uint64_t>(1);

	uint8_t start = 0;
	while (start < ELEMENT_SIZE && !ret[start]) { start++; }
	std::vector<bool> r;
	for (; start < ELEMENT_SIZE; start++)
		r.push_back(ret[start]);
	return r;

}

BigInteger pow(const BigInteger& bi, uint32_t e)
{
	if (e == 0) return BigInteger(1);
	else if (e == 1) return bi;

	BigInteger ret = 1;
	bool exp[ELEMENT_SIZE] = { false };

	for (int i = ELEMENT_SIZE - 1, j = 0; i >= 0; i--, j++)
		exp[i] = e >> j & 1;

	size_t start = 0;
	for (; start < ELEMENT_SIZE; start++) if (exp[start]) break;

	ret = bi * bi;

	for (size_t i = start + 1; i < ELEMENT_SIZE - 1; i++)
	{
		if (exp[i])
		{
			ret = ret * bi;
			ret = ret * ret;
		}
		else
			ret = ret * ret;
	}

	if (exp[ELEMENT_SIZE - 1]) ret = ret * bi;

	if (bi.negative)
	{
		if (bi.negative && e % 2 == 1) ret.negative = false;
		else ret.negative = true;
	}

	return ret;
}
BigInteger pow(const BigInteger& bi, const BigInteger& e)
{
	if (e == 0) return BigInteger(1);
	else if (e == 1) return bi;

	BigInteger ret = 1;
	std::vector<bool> exp = const_cast<BigInteger*>(&e)->get_binar_represent( );
	size_t start = 0;
	for (; start < exp.size( ); start++) if (exp[start]) break;

	ret = bi * bi;

	for (size_t i = start + 1; i < exp.size( ) - 1; i++)
	{
		if (exp[i])
		{
			ret = ret * bi;
			ret = ret * ret;
		}
		else
			ret = ret * ret;
	}

	if (exp[exp.size( ) - 1]) ret = ret * bi;

	if (bi.negative)
	{
		if (bi.negative && e % 2 == 1) ret.negative = true;
		else ret.negative = false;
	}
	return ret;
}

BigInteger gcd(BigInteger v1, BigInteger v2)
{
	BigInteger g;
	if (v1 < 0)
		v1 = v1 * (-1);
	if (v2 < 0)
		v2 = v2 * (-1);
	if (v1 + v2 == 0)
		return BigInteger(0);
	g = v2;
	while (v1 > 0)
	{
		g = v1;
		v1 = v2 % v1;
		v2 = g;
	}
	return g;
}
void swap(BigInteger& a, BigInteger& b)
{
	a ^= b;
	b ^= a;
	a ^= b;
}

BigInteger BigInteger::random(uint64_t bits)
{
	//	std::cout << "random start\n";
	BigInteger ret;
	srand(now_time( ));
	std::vector<bool> bit_rep;
	bit_rep.push_back(true);
	for (size_t i = 0; i < bits - 1; i++)
		bit_rep.push_back(rand( ) % 2);
	ret = bit_rep;
	//for (int i = 0; i < bits; i++)
	//	std::cout << bit_rep[i];
	//std::cout << "\nrandom:\n";
	//std::cout << ret;
	//std::cout << "\nrandom end\n";

	return ret;
}

BigInteger BigInteger::fact(const BigInteger& bi)
{
	BigInteger ret = 1;
	for (BigInteger i = 2; i <= bi; i += 1)
		ret *= i;
	return ret;
}

void BigInteger::uint64_to_binar(uint64_t x, bool* ret)
{
	memset(ret, 0, sizeof(ret));
	for (int8_t i = ELEMENT_SIZE - 1, j = 0; i >= 0; i--, j++)
		ret[i] = (x >> j)& static_cast<uint64_t>(1);
}

uint64_t BigInteger::binar_to_uint64(bool* x)
{
	uint64_t ret = 0;
	for (uint8_t i = 0; i < ELEMENT_SIZE; i++)
	{
		x[i] == true ? ret |= static_cast<uint64_t>(1) : NULL;
		if (i == ELEMENT_SIZE - 1) break;
		ret <<= 1;
	}
	return ret;
}


BigInteger::BigInteger( )
{
	negative = false;
	clear( );
}
BigInteger::BigInteger(const BigInteger& other)
{
	clear( );
	this->negative = other.negative;
	this->data = other.data;
}
BigInteger::BigInteger(const uint64_t item)
{
	clear( );
	*this = item;
}

BigInteger::BigInteger(std::vector<bool> binary_representation) // big-endian bits order (from bigger to smaller)
{
	clear( );
	*this = binary_representation;
}

BigInteger::BigInteger(std::vector<uint64_t> qwords) // little-endian byte order
{
	clear( );
	*this = qwords;
}
BigInteger::BigInteger(const std::string& item)
{
	clear( );
	*this = item;
}

BigInteger BigInteger::operator+(const BigInteger& other) const
{
	BigInteger bi;
	if (this->negative && other.negative)
	{
		bi = add(*this, other);
		bi.negative = true;
	}
	else if (this->negative && !other.negative)
	{
		if (bigger(*this, other))
		{
			bi = sub(*this, other);
			bi.negative = true;
		}
		else if (eq(*this, other))
		{
			return BigInteger(0);
		}
		else
		{
			bi = sub(other, *this);
			bi.negative = false;
		}
	}
	else if (!this->negative && other.negative)
	{
		if (bigger(*this, other))
		{
			bi = sub(*this, other);
			bi.negative = false;
		}
		else if (eq(*this, other))
		{
			return BigInteger(0);
		}
		else
		{
			bi = sub(other, *this);
			bi.negative = true;
		}
	}
	else
	{
		bi = add(*this, other);
		bi.negative = false;
	}
	return bi;
}
BigInteger BigInteger::operator-(const BigInteger& other) const
{
	BigInteger bi;
	if (this->negative && other.negative)
	{
		if (bigger(*this, other))
		{
			bi = sub(*this, other);
			bi.negative = true;
		}
		else if (eq(*this, other))
		{
			return BigInteger(0);
		}
		else
		{
			bi = sub(other, *this);
			bi.negative = false;
		}
	}
	else if (this->negative && !other.negative)
	{
		bi = add(*this, other);
		bi.negative = true;
	}
	else if (!this->negative && other.negative)
	{
		bi = add(*this, other);
		bi.negative = false;
	}
	else
	{
		if (bigger(*this, other))
		{
			bi = sub(*this, other);
			bi.negative = false;
		}
		else if (eq(*this, other))
		{
			return BigInteger(0);
		}
		else
		{
			bi = sub(other, *this);
			bi.negative = true;
		}
	}
	return bi;
}
BigInteger BigInteger::operator*(const BigInteger& other) const
{
	BigInteger ret = mul(*this, other);
	ret.negative = this->negative ^ other.negative;
	return ret;
}
BigInteger BigInteger::operator/(const BigInteger& other) const
{
	if (smaller(*this, other))
		return BigInteger(0);
	if (other == 0)
		return *this;
	if (*this == other)
		return BigInteger(1);

	if (this->data.size( ) == 1 && other.data.size( ) == 1)
	{
		BigInteger ret = this->data[0] / other.data[0];
		ret.negative = this->negative ^ other.negative;
		return ret;
	}


	BigInteger Q, R;
	Division(*this, other, Q, R);
	Q.negative = this->negative ^ other.negative;
	return Q;
}

BigInteger BigInteger::operator%(const BigInteger& other) const
{
	if (other == 0)
		return *this;
	if (*this == other)
		return BigInteger(1);

	if (this->data.size( ) == 1 && other.data.size( ) == 1)
	{
		BigInteger ret = this->data[0] % other.data[0];
		if (this->negative)
			ret.negative = true;
		return ret;
	}


	BigInteger Q, R;
	//std::cout << "Division\n";
	Division(*this, other, Q, R);
	if (this->negative)
		R.negative = true;
	//R.negative = this->negative ^ other.negative;
	return R;
}
BigInteger BigInteger::operator^(const BigInteger& other) const
{
	BigInteger bi;
	std::vector<uint64_t>* p_op2;
	if (*this >= other)
	{
		bi.data = other.data;
		size_t rest = this->data.size( ) - other.data.size( );
		bi.data.insert(bi.data.end( ), rest, 0);
		p_op2 = const_cast<std::vector<uint64_t>*>(&this->data);
	}
	else
	{
		bi.data = this->data;
		size_t rest = other.data.size( ) - this->data.size( );
		bi.data.insert(bi.data.end( ), rest, 0);
		p_op2 = const_cast<std::vector<uint64_t>*>(&other.data);
	}

	for (size_t i = 0; i < bi.data.size( ); i++)
		bi.data[i] ^= p_op2->at(i);

	while (bi.data.size( ) > 0 && bi.data[bi.data.size( ) - 1] == 0)
		bi.data.erase(bi.data.end( ) - 1);
	bi.negative = this->negative ^ other.negative;
	return bi;
}
BigInteger BigInteger::operator&(const BigInteger& other) const
{
	BigInteger bi;
	std::vector<uint64_t>* p_op2;
	if (*this >= other)
	{
		bi.data = other.data;
		size_t rest = this->data.size( ) - other.data.size( );
		bi.data.insert(bi.data.end( ), rest, 0);
		p_op2 = const_cast<std::vector<uint64_t>*>(&this->data);
	}
	else
	{
		bi.data = this->data;
		size_t rest = other.data.size( ) - this->data.size( );
		bi.data.insert(bi.data.end( ), rest, 0);
		p_op2 = const_cast<std::vector<uint64_t>*>(&other.data);
	}

	for (size_t i = 0; i < bi.data.size( ); i++)
		bi.data[i] &= p_op2->at(i);

	while (bi.data.size( ) > 0 && bi.data[bi.data.size( ) - 1] == 0)
		bi.data.erase(bi.data.end( ) - 1);
	bi.negative = this->negative & other.negative;
	return bi;
}
BigInteger BigInteger::operator|(const BigInteger& other) const
{
	BigInteger bi;
	std::vector<uint64_t>* p_op2;
	if (*this >= other)
	{
		bi.data = other.data;
		int rest = this->data.size( ) - other.data.size( );
		bi.data.insert(bi.data.end( ), rest, 0);
		p_op2 = const_cast<std::vector<uint64_t>*>(&this->data);
	}
	else
	{
		bi.data = this->data;
		int rest = other.data.size( ) - this->data.size( );
		bi.data.insert(bi.data.end( ), rest, 0);
		p_op2 = const_cast<std::vector<uint64_t>*>(&other.data);
	}

	for (size_t i = 0; i < bi.data.size( ); i++)
		bi.data[i] |= p_op2->at(i);

	while (bi.data.size( ) > 0 && bi.data[bi.data.size( ) - 1] == 0)
		bi.data.erase(bi.data.end( ) - 1);
	bi.negative = this->negative | other.negative;
	return bi;
}


BigInteger& BigInteger::operator++( )
{
	bool carry = 0;
	this->data[0]++;
	carry = __readeflags( ) & static_cast<uint64_t>(1);
	if (carry)
	{
		size_t j = 1;
		do
		{
			if (j == this->data.size( ))
				this->data.push_back(0);
			this->data[j]++;
			carry = __readeflags( ) & static_cast<uint64_t>(1);
			j++;
		} while (carry);
	}
	return *this;
}
BigInteger BigInteger::operator++(int x)
{
	BigInteger bi = *this;
	bool carry = 0;
	bi.data[0]++;
	carry = __readeflags( ) & static_cast<uint64_t>(1);
	if (carry)
	{
		size_t j = 1;
		do
		{
			if (j == bi.data.size( ))
				bi.data.push_back(0);
			bi.data[j]++;
			carry = __readeflags( ) & static_cast<uint64_t>(1);
			j++;
		} while (carry);
	}
	return bi;
}
BigInteger& BigInteger::operator--( )
{
	bool carry = 0;
	uint8_t counter = 0;
	int64_t j = 0;
	uint64_t ttt = 0;
	uint64_t mask = 1;
	this->data[0]--;
	//if (temp1 < temp2)
	carry = __readeflags( ) & static_cast<uint64_t>(1);
	if (carry)
	{
		mask = 1;
		counter = 0;
		// найти четверное слово, в котором можно произвести заём бита
		j = 1;
		while (j < this->data.size( ) && this->data[j] == 0) j++;
		// сохранить qword, чтобы найти, где самый младший единичный бит
		ttt = this->data[j];
		// ищем бит
		// смещаем временное значение вправо, пока первый бит != 1
		// вместе с этим counter сохраняет позицию бита
		// mask заполняется единицами в младших разрядах
		while (!(ttt & 0x1))
		{
			ttt >>= 1;
			counter++;
			mask <<= 1;
			mask |= 1;
		}
		counter++;
		// удаляем единицу в том месте, где мы её заняли
		this->data[j] &= (UINT64_MAX ^ (static_cast<uint64_t>(1) << counter));
		// заполняем единицами все разряды, младше того, из которого единицу достали
		this->data[j] |= mask;

		// если есть квадро слова, равные 0 и младшие того, из которого заняли единицу, заполняем их битовые поля 1
		for (j -= 1; j >= 0; j--)
			this->data[j] = UINT64_MAX;
	}
	return *this;
}
BigInteger BigInteger::operator--(int x)
{
	BigInteger bi = *this;
	bool carry = 0;

	uint8_t counter = 0;
	int64_t j = 0;
	uint64_t ttt = 0;
	uint64_t mask = 1;
	bi.data[0]--;
	//if (temp1 < temp2)
	carry = __readeflags( ) & static_cast<uint64_t>(1);
	if (carry)
	{
		mask = 1;
		counter = 0;
		// найти четверное слово, в котором можно произвести заём бита
		j = 1;
		while (j < bi.data.size( ) && bi.data[j] == 0) j++;
		// сохранить qword, чтобы найти, где самый младший единичный бит
		ttt = bi.data[j];
		// ищем бит
		// смещаем временное значение вправо, пока первый бит != 1
		// вместе с этим counter сохраняет позицию бита
		// mask заполняется единицами в младших разрядах
		while (!(ttt & 0x1))
		{
			ttt >>= 1;
			counter++;
			mask <<= 1;
			mask |= 1;
		}
		counter++;
		// удаляем единицу в том месте, где мы её заняли
		bi.data[j] &= (UINT64_MAX ^ (static_cast<uint64_t>(1) << counter));
		// заполняем единицами все разряды, младше того, из которого единицу достали
		bi.data[j] |= mask;

		// если есть квадро слова, равные 0 и младшие того, из которого заняли единицу, заполняем их битовые поля 1
		for (j -= 1; j >= 0; j--)
			bi.data[j] = UINT64_MAX;
	}
	return bi;
}


BigInteger BigInteger::operator+=(const BigInteger& other)
{
	*this = *this + other;
	return *this;
}
BigInteger BigInteger::operator-=(const BigInteger& other)
{
	*this = *this - other;
	return *this;
}
BigInteger BigInteger::operator*=(const BigInteger& other)
{
	*this = *this * other;
	return *this;
}
BigInteger BigInteger::operator/=(const BigInteger& other)
{
	*this = *this / other;
	return *this;
}
BigInteger BigInteger::operator%=(const BigInteger& other)
{
	*this = *this % other;
	return *this;
}
BigInteger BigInteger::operator^=(const BigInteger& other)
{
	*this = *this ^ other;
	return *this;
}
BigInteger BigInteger::operator&=(const BigInteger& other)
{
	*this = *this & other;
	return *this;
}
BigInteger BigInteger::operator|=(const BigInteger& other)
{
	*this = *this & other;
	return *this;
}


BigInteger BigInteger::operator+(uint64_t item) const
{
	return *this + BigInteger(item);
}
BigInteger BigInteger::operator-(uint64_t item) const
{
	return *this - BigInteger(item);
}
BigInteger BigInteger::operator*(uint64_t item) const
{
	return *this * BigInteger(item);
}
BigInteger BigInteger::operator/(uint64_t item) const
{
	return *this / BigInteger(item);
}
BigInteger BigInteger::operator%(uint64_t item) const
{
	return *this % BigInteger(item);
}
BigInteger BigInteger::operator^(uint64_t item) const
{
	return *this ^ BigInteger(item);
}
BigInteger BigInteger::operator&(uint64_t item) const
{
	return *this & BigInteger(item);
}
BigInteger BigInteger::operator|(uint64_t item) const
{
	return *this & BigInteger(item);
}



BigInteger BigInteger::operator+=(const uint64_t item)
{
	*this = *this + item;
	return *this;
}
BigInteger BigInteger::operator-=(const uint64_t item)
{
	*this = *this - item;
	return *this;
}
BigInteger BigInteger::operator*=(const uint64_t item)
{
	*this = *this * item;
	return *this;
}
BigInteger BigInteger::operator/=(const uint64_t item)
{
	*this = *this / item;
	return *this;
}
BigInteger BigInteger::operator%=(const uint64_t item)
{
	*this = *this % item;
	return *this;
}
BigInteger BigInteger::operator^=(const uint64_t item)
{
	*this = *this ^ item;
	return *this;
}
BigInteger BigInteger::operator&=(const uint64_t item)
{
	*this = *this & item;
	return *this;
}
BigInteger BigInteger::operator|=(const uint64_t item)
{
	*this = *this & item;
	return *this;
}


BigInteger BigInteger::operator<<(uint64_t r) const
{
	if (this->data.size( ) == 0) return *this;
	BigInteger bi = *this;
	//std::cout << "1\n";
	if (r >= ELEMENT_SIZE)
	{
		bi.data.insert(bi.data.begin( ), r / ELEMENT_SIZE, 0);
		r = r % ELEMENT_SIZE;
	}
	if (r == 0)
		return bi;
	uint64_t temp[2] = { 0 };
	temp[0] = bi.data[0];
	bi.data[0] <<= r;
	if (bi.data.size( ) == 1)
		bi.data.insert(bi.data.end( ), 1, 0);
	temp[1] = bi.data[1];
	//bi.data[1] |= (temp[0] >> 32 - r);
	char temp_n = 1;
	for (size_t i = 1; i < bi.data.size( ); i++)
	{
		temp_n = i % 2 == 1 ? 0 : 1;
		bi.data[i] <<= r;
		bi.data[i] |= (temp[temp_n] >> (ELEMENT_SIZE - r));
		if (i < bi.data.size( ) - static_cast<size_t>(1))
			temp[temp_n] = bi.data[i + static_cast<size_t>(1)];
	}
	if (temp_n == 0)
	{
		if (temp[1] != 0) bi.data.push_back(temp[1] >> (ELEMENT_SIZE - r));
	}
	else
	{
		if (temp[0] != 0) bi.data.push_back(temp[0] >> (ELEMENT_SIZE - r));
	}

	return bi;
	//BigInteger bi = *this;
	//std::vector<bool> bbi = bi.get_binar_represent();
	//bbi.insert(bbi.end(), r, 0);
	//bi = bbi;
	//return bi;
}
BigInteger BigInteger::operator>>(uint64_t r) const
{
	BigInteger bi = *this;
	//std::cout << "1\n";
	if (r >= ELEMENT_SIZE)
	{
		bi.data.erase(bi.data.begin( ), bi.data.begin( ) + (r / ELEMENT_SIZE));
		r = r % ELEMENT_SIZE;
	}
	if (r == 0)
		return bi;
	if (bi.data.size( ) == 0) return bi;
	bi.data[0] >>= r;
	uint64_t temp;
	for (size_t i = 1; i < bi.data.size( ); i++)
	{
		temp = bi.data[i] << (ELEMENT_SIZE - r);
		bi.data[i - 1] |= temp;
		bi.data[i] >>= r;
	}

	return bi;
}
BigInteger BigInteger::operator<<=(uint64_t r)
{
	*this = *this << r;
	return *this;
}
BigInteger BigInteger::operator>>=(uint64_t r)
{
	*this = *this >> r;
	return *this;
}


// big-endian bits order (from bigger to smaller)
BigInteger& BigInteger::operator=(std::vector<bool> binary_representation)
{
	clear( );
	if (binary_representation.size( ) == 0)
	{
		*this = 0;
		return *this;
	}

	size_t rest = (binary_representation.size( ) % ELEMENT_SIZE);
	if (rest > 0)
	{
		rest = ELEMENT_SIZE - rest;
		binary_representation.insert(binary_representation.begin( ), rest, false);
	}

	bool binar[ELEMENT_SIZE] = { false };
	std::vector<bool>::reverse_iterator it = binary_representation.rbegin( );
	for (int8_t i = ELEMENT_SIZE - 1; it != binary_representation.rend( ); ++it, i--)
	{
		binar[i] = *it;
		if (i == 0)
		{

			this->data.push_back(binar_to_uint64(binar));
			i = ELEMENT_SIZE;
		}

	}

	this->negative = false;
	while (data.size( ) > 0 && data[data.size( ) - 1] == 0) data.erase(data.end( ) - 1);
	return *this;
}
// little-endian byte order
BigInteger& BigInteger::operator=(std::vector<uint64_t> qwords)
{
	clear( );
	size_t end = qwords.size( );
	std::vector<uint64_t>::reverse_iterator it = qwords.rbegin( );
	for (; it != qwords.rend( ); ++it)
		if (*it == 0) end--;
		else if (*it != 0) break;
	qwords.erase(qwords.begin( ) + end, qwords.end( ));
	this->negative = false;
	this->data = qwords;
	while (data.size( ) > 0 && data[data.size( ) - 1] == 0) data.erase(data.end( ) - 1);
	return *this;
}
BigInteger& BigInteger::operator=(const BigInteger& other)
{
	clear( );
	this->negative = other.negative;
	this->data = other.data;
	while (data.size( ) > 0 && data[data.size( ) - 1] == 0) data.erase(data.end( ) - 1);
	return *this;
}
BigInteger& BigInteger::operator=(const uint64_t item)
{
	clear( );

	data.push_back(item);
	if (data[0] == 0) std::vector<uint64_t>( ).swap(this->data);
	//std::cout << "operator=: " << data[0] << std::endl;
	//while (data.size() > 0 && data[data.size() - 1] == 0) data.erase(data.end() - 1);

	return *this;
}
BigInteger& BigInteger::operator=(const std::string& item)
{
	clear( );
	if (item == "0")
	{
		*this = 0;
		return *this;
	}

	std::vector<bool> b = ToString::get_binar_repr(item);
	*this = b;

	return *this;
}


bool BigInteger::operator<(const BigInteger& other) const
{
	if (this->negative && other.negative)
		return !smaller(*this, other);
	else if (this->negative && !other.negative)
		return true;
	else if (!this->negative && other.negative)
		return false;
	else
		return smaller(*this, other);
}
bool BigInteger::operator>(const BigInteger& other) const
{
	//std::cout << "operator>: " << *this << ", " << other.data[0] << std::endl;
	//std::cout << "1\n";
	if (this->negative && other.negative)
		return !bigger(*this, other);
	else if (this->negative && !other.negative)
		return false;
	else if (!this->negative && other.negative)
		return true;
	else
		return bigger(*this, other);
}
bool BigInteger::operator<=(const BigInteger& other) const
{
	if (this->negative && other.negative)
		return bigger_eq(*this, other);
	else if (this->negative && !other.negative)
		return true;
	else if (!this->negative && other.negative)
		return false;
	else
		return smaller_eq(*this, other);
}
bool BigInteger::operator>=(const BigInteger& other) const
{
	if (this->negative && other.negative)
		return smaller_eq(*this, other);
	else if (this->negative && !other.negative)
		return false;
	else if (!this->negative && other.negative)
		return true;
	else
		return bigger_eq(*this, other);
}
bool BigInteger::operator==(const BigInteger& other) const
{
	if (this->negative && other.negative)
		return eq(*this, other);
	else if (this->negative && !other.negative)
		return false;
	else if (!this->negative && other.negative)
		return false;
	else
		return eq(*this, other);
}
bool BigInteger::operator!=(const BigInteger& other) const
{
	if (this->negative && other.negative)
		return eq_not(*this, other);
	else if (this->negative && !other.negative)
		return true;
	else if (!this->negative && other.negative)
		return true;
	else
		return eq_not(*this, other);
}


bool BigInteger::operator==(const uint64_t item) const
{
	BigInteger r = item;
	return *this == r;
}
bool BigInteger::operator<(const uint64_t item) const
{
	if (item >= 0 && this->negative) return true;
	else return false;
	BigInteger r = item;
	return *this < r;
}
bool BigInteger::operator>(const uint64_t item) const
{
	//std::cout << "item = " << item << std::endl;
	BigInteger r = item;
	//std::cout << "r = " << r << std::endl;
	return *this > r;
}
bool BigInteger::operator<=(const uint64_t item) const
{
	BigInteger r = item;
	return *this <= r;
}
bool BigInteger::operator>=(const uint64_t item) const
{
	BigInteger r = item;
	return *this >= r;
}



std::vector<bool> BigInteger::get_binar_represent( )
{
	std::vector<bool> ret;
	bool binar[ELEMENT_SIZE] = { false };
	for (int64_t i = static_cast<int64_t>(this->data.size( ) - 1); i >= 0; i--)
	{
		uint64_to_binar(this->data[i], binar);
		for (uint8_t j = 0; j < ELEMENT_SIZE; j++)
			ret.push_back(binar[j]);
	}
	while (ret.size( ) > 0 && *ret.begin( ) == false) ret.erase(ret.begin( ));
	return ret;
}
std::vector<uint64_t> BigInteger::get_qword_represent( )
{
	return this->data;
}
std::string BigInteger::get_text_represent( )
{
	//std::cout << "11\n";
	BigInteger _10 = 10;
	//std::cout << "_10 = " << ToString::get_string(_10.get_binar_represent( )) << std::endl;
	BigInteger copy = *this;
	std::vector<uint8_t> dig;
	BigInteger temp;
	if (copy.negative) copy.negative = false;
	//std::cout << "12\n";
	while (copy > _10)
	{
		//::cout << copy << ", "
		Division(copy, _10, copy, temp);
		//temp = copy % _10;
		if (temp.data.size( ) == 0) dig.insert(dig.begin( ), 0);
		else dig.insert(dig.begin( ), temp.data[0]);
		//copy = copy / _10;
		//std::cout << "copy = " << ToString::get_string(copy.get_binar_represent( )) << std::endl;
		//std::cout << "temp = " << ToString::get_string(temp.get_binar_represent( )) << std::endl;
	}
	//std::cout << "13\n";
	if (copy.data.size() > 0)
		dig.insert(dig.begin( ), copy.data[0]);
	std::string ret;
	if (this->negative) ret.push_back('-');
	//std::cout << "14\n";
	char buf[10];
	memset(buf, 0, 10);
	for (int i = 0; i < dig.size( ); i++)
	{
		_itoa_s(dig[i], buf, 10);
		ret.push_back(buf[0]);
	}
	//std::cout << "15\n";
	return ret;
}

uint64_t BigInteger::bit_count( )
{
	uint64_t r = 1;
	r <<= 63;
	if (this->data.size( ) == 0) return 0;
	uint64_t last = this->data[this->data.size( ) - 1];
	size_t i = static_cast<int64_t>(this->data.size( ) - 2);
	while (last == 0 && i >= 0)
		last = this->data[i--];
	uint64_t count = 0;

	while ((last & r) != r)
	{
		count++;
		last <<= 1;
	}
	uint64_t bits = (i + static_cast<int64_t>(2)) * ELEMENT_SIZE;
	return bits - count;
}

uint64_t BigInteger::normalize( )
{
	//std::cout << "this.data.size() = " << this->data.size( ) << std::endl;
	uint64_t r = 1;
	r <<= 63;
	if (this->data.size( ) == 0) return 0;
	uint64_t counter = 0;
	while ((this->data[this->data.size( ) - 1] & r)
		!= r)
	{
		*this <<= 1;
		counter++;
	}
	return counter;
}

std::ostream& operator<<(std::ostream& os, BigInteger bi)
{
	//if (bi.negative) os << "-";
	os << bi.get_text_represent( ); return os;

	//os << ToString::get_string(bi.get_binar_represent( )); return os;
}