#include "BigInteger.h"
#include <algorithm>
#include <thread>

void BigInteger::div2(const BigInteger& origin, BigInteger& left, BigInteger& right)
{
	if (origin.data.size( ) > static_cast<size_t>(1))
	{
		size_t l_c = origin.data.size( ) / static_cast<size_t>(2);

		std::vector<uint64_t> l, r;
		r.insert(r.begin( ), origin.data.begin( ), origin.data.begin( ) + l_c);
		l.insert(l.begin( ), origin.data.begin( ) + l_c, origin.data.end( ));
		left = l;
		right = r;
	}
	else if (origin.data.size( ) == static_cast<size_t>(1))
	{
		left.data.push_back(0);
		right.data.push_back(1);
	}
	else if (origin.data.size( ) == static_cast<size_t>(0))
	{
		left.data.push_back(0);
		right.data.push_back(0);
	}
}

BigInteger BigInteger::karatsuba_mul(BigInteger& X, BigInteger& Y)
{
	expansion(X, Y);
	size_t n = std::max(X.data.size( ), Y.data.size( ));
	if (n <= static_cast<size_t>(256))
	{
		while (X.data.size( ) > 0 && X.data[X.data.size( ) - static_cast<size_t>(1)] == 0) 
			X.data.erase(X.data.end( ) - static_cast<size_t>(1));
		while (Y.data.size( ) > 0 && Y.data[Y.data.size( ) - static_cast<size_t>(1)] == 0) 
			Y.data.erase(Y.data.end( ) - static_cast<size_t>(1));
		return naive_mul(X, Y);
	}
	else
	{
		BigInteger x_l, x_r; div2(X, x_l, x_r);
		BigInteger y_l, y_r; div2(Y, y_l, y_r);
		BigInteger x_lx_r = x_l + x_r;
		BigInteger y_ly_r = y_l + y_r;

		BigInteger prod_1 = karatsuba_mul(x_l, y_l);
		BigInteger prod_2 = karatsuba_mul(x_r, y_r);
		BigInteger prod_3 = karatsuba_mul(x_lx_r, y_ly_r);

		prod_3 = prod_3 - prod_1 - prod_2;
		prod_3.data.insert(prod_3.data.begin( ), n / 2, 0);
		prod_1.data.insert(prod_1.data.begin( ), n, 0);

		return prod_1 + prod_3 + prod_2;
	}

}

void BigInteger::expansion(BigInteger& X, BigInteger& Y)
{
	size_t s = X.data.size( ) >= Y.data.size( ) ? X.data.size( ) : Y.data.size( );
	if (s == 0 || s == 1 || s == 2) return;

	uint64_t i = 1;
	while (i < s) i <<= 1;

	X.data.insert(X.data.end( ), i - X.data.size( ), 0);
	Y.data.insert(Y.data.end( ), i - Y.data.size( ), 0);
}

BigInteger BigInteger::naive_mul(const BigInteger& x, const BigInteger& y)
{
	BigInteger bi;
	bi.data = std::vector<uint64_t>(x.data.size( ) + y.data.size( ), 0);
	uint64_t temp, hi, lo;
	bool carry = false;

#ifdef _M_IX86
	uint64_t t1, t2, t3;
#endif

	//std::cout << this->data.size() << std::endl << other.data.size();
	for (size_t i = 0; i < x.data.size( ); i++)
	{
		for (size_t j = 0; j < y.data.size( ); j++)
		{
			
#ifdef _M_X64
			lo = x.data[i] * y.data[j];
			hi = __umulh(x.data[i], y.data[j]);
#endif _M_X64
#ifdef _M_IX86
			t1 = (x.data[i] >> 32) * (y.data[j] >> 32);
			t2 = (x.data[i] & 0x00000000FFFFFFFF) * (y.data[j] & 0x00000000FFFFFFFF);
			t3 = ((x.data[i] >> 32) + (x.data[i] & 0x00000000FFFFFFFF))*
				((y.data[j] >> 32) + (y.data[j] & 0x00000000FFFFFFFF));
			t3 = t3 - t1 - t2;

			t2 += t3;
			carry = __readeflags( ) & 1;
			if (carry)
				t1++;

			lo = t2;
			hi = t1;
#endif _M_IX86

			
			//lo = _umul128(x.data[i], y.data[j], &hi);
			bi.data[j + i] += lo;
			carry = __readeflags( ) & 1;
			if (carry)
			{
				size_t c = i + j + static_cast<size_t>(1);
				do
				{
					bi.data[c++] += 1;
					carry = __readeflags( ) & 1;
				} while (carry);
			}

			bi.data[j + i + static_cast<size_t>(1)] += hi;
			carry = __readeflags( ) & 1;
			if (carry)
			{
				size_t c = i + j + static_cast<size_t>(2);
				do
				{
					bi.data[c++] += 1;
					carry = __readeflags( ) & 1;
				} while (carry);
			}
		}
	}

	while (bi.data.size( ) > 0 && bi.data[bi.data.size( ) - static_cast<size_t>(1)] == 0) 
		bi.data.erase(bi.data.end( ) - static_cast<size_t>(1));

	return bi;
}

void BigInteger::multi_proc_adapter(BigInteger& sol, BigInteger& l, BigInteger& r)
{
	sol = karatsuba_mul(l, r);
}


BigInteger BigInteger::mul(const BigInteger& X, const BigInteger& Y)
{
	//BigInteger x = X;
	//BigInteger y = Y;

	//expansion(x, y);

	//uint32_t n = std::max(x.data.size( ), y.data.size( ));

	//BigInteger x_l, x_r; div2(x, x_l, x_r);
	//BigInteger y_l, y_r; div2(y, y_l, y_r);
	//BigInteger x_l_x_r = x_l + x_r;
	//BigInteger y_l_y_r = y_l + y_r;

	//BigInteger prod_1;
	//BigInteger prod_2;


	//std::thread th1 = std::thread(&multi_proc_adapter, std::ref(prod_1),
	//	std::ref(x_l), std::ref(y_l));
	//std::thread th2 = std::thread(&multi_proc_adapter, std::ref(prod_2),
	//	std::ref(x_r), std::ref(y_r));

	//BigInteger prod_3 = karatsuba_mul(x_l_x_r, y_l_y_r);

	//th1.join( ); th2.join( );

	//prod_3 = prod_3 - prod_1 - prod_2;
	//prod_3.data.insert(prod_3.data.begin( ), n / 2, 0);
	//prod_1.data.insert(prod_1.data.begin( ), n, 0);

	//prod_3 = prod_1 + prod_3 + prod_2;
	//return prod_3;
	//4100
	if (X.data.size( ) <= 1 && Y.data.size( ) <= 1)
		return naive_mul(X, Y);
	else
	{
		BigInteger x = X;
		BigInteger y = Y;
		BigInteger res = karatsuba_mul(x, y);
		while (res.data.size( ) > 0 && res.data[res.data.size( ) - 1] == 0) 
			res.data.erase(res.data.end( ) - 1);
		return res;
		/*BigInteger x = X;
		BigInteger y = Y;

		expansion(x, y);

		uint32_t n = std::max(x.data.size( ), y.data.size( ));

		BigInteger x_l, x_r; div2(x, x_l, x_r);
		BigInteger y_l, y_r; div2(y, y_l, y_r);
		BigInteger x_l_x_r = x_l + x_r;
		BigInteger y_l_y_r = y_l + y_r;

		BigInteger prod_1;
		BigInteger prod_2;


		std::thread th1 = std::thread(&multi_proc_adapter, std::ref(prod_1),
			std::ref(x_l), std::ref(y_l));
		std::thread th2 = std::thread(&multi_proc_adapter, std::ref(prod_2),
			std::ref(x_r), std::ref(y_r));

		BigInteger prod_3 = karatsuba_mul(x_l_x_r, y_l_y_r);

		th1.join( ); th2.join( );

		prod_3 = prod_3 - prod_1 - prod_2;
		prod_3.data.insert(prod_3.data.begin( ), n / 2, 0);
		prod_1.data.insert(prod_1.data.begin( ), n, 0);

		prod_3 = prod_1 + prod_3 + prod_2;
		return prod_3;*/
	}

}