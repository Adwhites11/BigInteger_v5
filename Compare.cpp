#include "BigInteger.h"

bool BigInteger::smaller(const BigInteger& X, const BigInteger& Y)
{
	if ((X.data.size( ) == 0) && (Y.data.size( ) == 0))
		return false;
	if (X.data.size( ) < Y.data.size( ))
		return true;
	else if (X.data.size( ) > Y.data.size( ))
		return false;
	else
	{
		//std::cout << "dbg operator < 2\n";
		//for (int64_t i = X.data.size( ) - 1; i >= 0; i--)
		for (size_t i = X.data.size( ) - 1; (__readeflags( ) & 0x1) == false; --i)
		{
			if (X.data[i] < Y.data[i]) return true;
			else if (X.data[i] > Y.data[i]) return false;
			else continue;
		}
	}
	return false;
}
bool BigInteger::bigger(const BigInteger& X, const BigInteger& Y)
{
	//std::cout << X.data.size( ) << ", " << Y.data.size( ) << "\n";
	if ((X.data.size( ) == 0) && (Y.data.size( ) == 0))
		return false;
	else if (X.data.size( ) > Y.data.size( ))
		return true;
	else if (X.data.size( ) < Y.data.size( ))
		return false;
	else
	{
		//std::cout << Y.data.size( ) << " " << X.data.size( ) << std::endl;
		//for (int64_t i = X.data.size( ) - 1; i >= 0; i--)
		for (size_t i = X.data.size( ) - 1; (__readeflags( ) & 0x1) == false; --i)
		{
			if (X.data[i] > Y.data[i]) return true;
			else if (X.data[i] < Y.data[i]) return false;
			else continue;
		}
	}
	return false;
}
bool BigInteger::smaller_eq(const BigInteger& X, const BigInteger& Y)
{
	if ((X.data.size( ) == 0) && (Y.data.size( ) == 0))
		return true;
	if (X.data.size( ) < Y.data.size( ))
		return true;
	else if (X.data.size( ) > Y.data.size( ))
		return false;
	else
	{
		//for (int64_t i = X.data.size( ) - 1; i >= 0; i--)
		for (size_t i = X.data.size( ) - 1; (__readeflags( ) & 0x1) == false; --i)
		{
			if (X.data[i] < Y.data[i]) return true;
			else if (X.data[i] > Y.data[i]) return false;
			else continue;
		}
	}
	return true;
}
bool BigInteger::bigger_eq(const BigInteger& X, const BigInteger& Y)
{
	if ((X.data.size( ) == 0) && (Y.data.size( ) == 0))
		return true;
	if (X.data.size( ) > Y.data.size( ))
		return true;
	else if (X.data.size( ) < Y.data.size( ))
		return false;
	else
	{
		//for (int64_t i = X.data.size( ) - 1; i >= 0; i--)
		for (size_t i = X.data.size( ) - 1; (__readeflags( ) & 0x1) == false; --i)
		{
			if (X.data[i] > Y.data[i]) return true;
			else if (X.data[i] < Y.data[i]) return false;
			else continue;
		}
	}
	return true;
}
bool BigInteger::eq(const BigInteger& X, const BigInteger& Y)
{
	if (X.data.size( ) > 0 && Y.data.size( ) <= 0) return false;
	if (X.data.size( ) <= 0 && Y.data.size( ) > 0) return false;
	if (X.data.size( ) == 0 && Y.data.size( ) == 0) return true;
	if (X.data.size( ) != Y.data.size( )) return false;
	else
	{
		for (size_t i = 0; i < X.data.size( ); i++)
			if (X.data[i] != Y.data[i]) return false;
	}
	return true;
}
bool BigInteger::eq_not(const BigInteger& X, const BigInteger& Y)
{
	if (X.data.size( ) != Y.data.size( )) return true;
	else
	{
		bool* control = new bool[X.data.size( )];
		//for (int64_t i = X.data.size( ) - 1; i >= 0; i--)
		for (size_t i = X.data.size( ) - 1; (__readeflags( ) & 0x1) == false; --i)
		{
			if (X.data[i] == Y.data[i]) control[i] = true;
			else control[i] = false;
		}

		int count = 0;
		//for (int64_t i = X.data.size( ) - 1; i >= 0; i--)
		for (size_t i = X.data.size( ) - 1; (__readeflags( ) & 0x1) == false; --i)
			if (control[i]) count++;
		if (count == X.data.size( ))
			return false;
		else
			return true;
	}
}