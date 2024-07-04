#include "BigInteger.h"


uint64_t BigInteger::expansion_on_right(BigInteger& X)
{
	size_t s = X.data.size( );
	if (s == 2) return 0;

	uint64_t i = 1;
	while (i < s) i <<= 1;

	uint64_t ret = i - X.data.size( );
	X.data.insert(X.data.begin( ), i - X.data.size( ), 0);
	return ret;
}
uint64_t BigInteger::expansion(BigInteger& X)
{
	size_t s = X.data.size( );
	if (s == 0 || s == 1 || s == 2) return s;

	size_t i = 1;
	while (i < s) i <<= 1;

	X.data.insert(X.data.end( ), i - X.data.size( ), 0);
	return i;
}

void BigInteger::expansion(BigInteger& X, size_t n)
{
	X.data.insert(X.data.end( ), n - X.data.size( ), 0);
}


void BigInteger::DivThreeHalvesByTwo(uint32_t a1, uint32_t a2,
	uint32_t a3, uint32_t b1, uint32_t b2, uint64_t& Q, uint64_t& R)
{
	uint64_t a;
	reinterpret_cast<uint32_t*>(&a)[1] = a1;
	reinterpret_cast<uint32_t*>(&a)[0] = a2;
	uint64_t q = a / b1;

	//std::cout << "a = " << a << "\nb1 = " << b1
	//	<< "\na / b1 = " << a / b1 << "\nq = " << q << "\n";

	uint64_t c = a - q * b1;
	//std::cout << "c   = " << c << "\n";
	uint64_t D = q * b2;
	uint64_t ca3;
	reinterpret_cast<uint32_t*>(&ca3)[1] = static_cast<uint32_t>(c);
	reinterpret_cast<uint32_t*>(&ca3)[0] = a3;
	uint64_t B;
	reinterpret_cast<uint32_t*>(&B)[1] = b1;
	reinterpret_cast<uint32_t*>(&B)[0] = b2;

	//std::cout << "ca3 = " << ca3 << "\n";
	//std::cout << "D   = " << D << "\n";
	//std::cout << "B   = " << B << "\n";
	BigInteger r = BigInteger(ca3) - BigInteger(D);
	while (r < 0)
	{
		q--;
		r += B;
	}
	if (r.data.size( ) > 0) R = r.data[0];
	else					R = 0;
	//std::cout << "q   = " << q << "\n";

	//std::cout << "R   = " << R << "\n";
	Q = q;
}
void BigInteger::DivTwoDigitsByOne(uint64_t AH, uint64_t AL, uint64_t B,
	uint64_t q[2], uint64_t& s)
{
	uint32_t a1, a2;
	a1 = reinterpret_cast<uint32_t*>(&AH)[1];
	a2 = reinterpret_cast<uint32_t*>(&AH)[0];

	uint32_t a3, a4;
	a3 = reinterpret_cast<uint32_t*>(&AL)[1];
	a4 = reinterpret_cast<uint32_t*>(&AL)[0];

	uint32_t b1, b2;
	b1 = reinterpret_cast<uint32_t*>(&B)[1];
	b2 = reinterpret_cast<uint32_t*>(&B)[0];

	uint64_t q12[2];
	uint64_t R, S;
	uint32_t r12[2];

	// compute high part of solution
	DivThreeHalvesByTwo(a1, a2, a3, b1, b2, q12[0], R);
	r12[0] = reinterpret_cast<uint32_t*>(&R)[1];
	r12[1] = reinterpret_cast<uint32_t*>(&R)[0];


	//std::cout << "\n\n";
	//std::cout << a1 << " " << a2 << " " << a3 << std::endl;
	//std::cout << "q0 = " << q12[0] << std::endl;

	// compute low part of solution
	DivThreeHalvesByTwo(r12[0], r12[1], a4, b1, b2, q12[1], S);
	//std::cout << r12[0] << " " << r12[1] << " " << a4 << "\n\n";

	q[1] = 0;
	q[0] = q12[1];
	q[0] += (q12[0] << 32);
	if (__readeflags( ) & 1)
		q[1] += 1;
	q[1] += (q12[0] >> 32);

	s = S;
}
//18765734346198288863 true
//18765734346198288863 false


void BigInteger::D2n_1n(BigInteger& A, BigInteger& B, size_t n, BigInteger& Q, BigInteger& R)
{
	//std::cout << "d2n_1n, n = " << n << "\n";
	if (n == 1)
	{
		uint64_t r = 0;
		uint64_t q[2] = { 0 };
		////std::cout << "_udiv128 test\n";
		////std::cout << "a[1] = " << A.data[1] << std::endl;
		////std::cout << "a[0] = " << A.data[0] << std::endl;
		////std::cout << "b    = " << B.data[0] << std::endl;

		////std::cout << "bits in A: " << A.bit_count( ) << std::endl;
		////std::cout << "bits in B: " << B.bit_count( ) << std::endl;
		uint64_t ah = A.data[1];
		uint64_t al = A.data[0];
		uint64_t b = B.data[0];
		DivTwoDigitsByOne(ah, al, b, q, r);
		//uint64_t q, r, b;
		//b = B.data[0];
		//uint8_t counter = 0;
		//while ((b & 0x8000'0000'0000'0000) != 0x8000'0000'0000'0000) 
		//{
		//	b <<= 1;
		//	counter++;
		//}
		//q = _udiv128(A.data[1], A.data[0], b, &r);
		//q <<= counter;
		//r >>= counter;
		//Q = q;
		//Q <<= counter;
		//std::cout << "_udiv128 test end\n";
		Q = std::vector<uint64_t>{ q[0], q[1] };
		R = std::vector<uint64_t>{ r };

		
		return;
	}
	else
	{
		uint64_t nor_count = B.normalize( );
		A <<= nor_count;

		if (A.data.size( ) != n * 2)
			A.data.insert(A.data.end( ), n * 2 - A.data.size( ), 0);


		size_t step = A.data.size( ) / 4;
		BigInteger A1, A2, A3, A4;
		for (size_t i = 0; i < step; i++)
			A4.data.push_back(A.data[i]);
		for (size_t i = step; i < step * 2; i++)
			A3.data.push_back(A.data[i]);
		for (size_t i = step * 2; i < step * 3; i++)
			A2.data.push_back(A.data[i]);
		for (size_t i = step * 3; i < step * 4; i++)
			A1.data.push_back(A.data[i]);

		step = B.data.size( ) / 2;
		BigInteger B1, B2;
		for (size_t i = 0; i < step; i++)
			B2.data.push_back(B.data[i]);
		for (size_t i = step; i < step * 2; i++)
			B1.data.push_back(B.data[i]);

		BigInteger Q1, Q2;
		BigInteger r, S;

		D3n_2n(A1, A2, A3, B1, B2, n / 2, Q1, r);

		if (r.data.size( ) < n)
			expansion(r, n);
		step = r.data.size( ) / 2;
		BigInteger R1, R2;
		for (size_t i = 0; i < step; i++)
			R2.data.push_back(r.data[i]);
		for (size_t i = step; i < step * 2; i++)
			R1.data.push_back(r.data[i]);

		D3n_2n(R1, R2, A4, B1, B2, n / 2, Q2, S);
		if (Q1.data.size( ) == 0)
			expansion(Q1, n / 2);
		if (Q2.data.size( ) == 0)
			expansion(Q2, n / 2);

		std::vector<uint64_t>( ).swap(Q.data);
		Q.data.insert(Q.data.begin( ), Q2.data.begin( ), Q2.data.end( ));
		Q.data.insert(Q.data.end( ), Q1.data.begin( ), Q1.data.end( ));
		R = S;
		B >>= nor_count;
		A >>= nor_count;
		R >>= nor_count;

		
	}

}
void BigInteger::D3n_2n(BigInteger& A1, BigInteger& A2, BigInteger& A3, BigInteger& B1, BigInteger& B2, size_t n, BigInteger& Q, BigInteger& R)
{
	BigInteger a;
	a.data.insert(a.data.begin( ), A2.data.begin( ), A2.data.end( ));
	a.data.insert(a.data.end( ), A1.data.begin( ), A1.data.end( ));
	BigInteger q, c;
	//std::cout << "D3n_2n enter in D2n_1n\n";
	D2n_1n(a, B1, n, q, c);
	BigInteger D = q * B2;
	BigInteger ca3;
	ca3.data.insert(ca3.data.begin( ), c.data.begin( ), c.data.end( ));
	ca3.data.insert(ca3.data.begin( ), A3.data.begin( ), A3.data.end( ));
	R = ca3 - D;
	//std::cout << "D3n_2n 1\n";
	BigInteger B;
	B.data.insert(B.data.begin( ), B2.data.begin( ), B2.data.end( ));
	B.data.insert(B.data.end( ), B1.data.begin( ), B1.data.end( ));
	//std::cout << "D3n_2n 2\n";
	while (R < 0)
	{
		//std::cout << "D3n_2n circle start\n";
		q--;
		R += B;
		//std::cout << "D3n_2n circle end\n";
	}
	//std::cout << "D3n_2n 3\n";
	Q = q;
}

void BigInteger::Division(BigInteger X, BigInteger Y, BigInteger& Q, BigInteger& R)
{
	if ((X.data.size( ) == 0) && (Y.data.size( ) == 0))
	{
		Q = 0;
		R = 0;
		return;
	}
	//std::cout << "div\n";
	if (X < Y)
	{
		Q = 0;
		R = X;
		return;
	}
	
	//std::cout << 1 << "\n";

	//std::cout << 2 << "\n";

	std::vector<uint64_t>( ).swap(Q.data);
	std::vector<uint64_t>( ).swap(R.data);

	if ((X.data.size( ) == 1) && (Y.data.size( ) == 1))
	{
		Q.data.push_back(X.data[0] / Y.data[0]);
		R.data.push_back(X.data[0] % Y.data[0]);
		return;
	}

	BigInteger q, r;
	//BigInteger X = x;
	//BigInteger Y = y;

	//std::cout << 3 << "\n";

	// расширить до ближайшей степени 2 делитель
	uint64_t m = expansion_on_right(Y);
	// нормализовать делитель (1 в самом старшем бите)
	uint64_t nor_count = Y.normalize( );

	// сместить делимое на кол-во бит, необходимых для нормализации
	// делителя
	X <<= nor_count;
	// расширить делитель как делимое
	X.data.insert(X.data.begin( ), m, 0);

	//std::cout << "nor_count = " << nor_count << std::endl;
	//std::cout << 4 << "\n";

	// добить делимое слева до размера, кратного 
	// размеру делителя
	uint_fast64_t delim_expans = 0;
	while (((X.data.size( ) % Y.data.size( )) != 0) /*&& X.data.size( ) == Y.data.size( )*/)
	{
		delim_expans++;
		X.data.insert(X.data.end( ), 1, 0);
	}


	//std::cout << 5 << "\n";

	// определить количество итераций
	size_t t = X.data.size( ) / Y.data.size( );
	// определить размер делимого
	size_t n = Y.data.size( );
	BigInteger Z;
	//std::cout << 6 << "\n";
	//std::cout << "n = " <<  n << "\n";
	//std::cout << "X.data.size( ) = " << X.data.size( ) << "\n";
	//std::cout << "Y.data.size( ) = " << Y.data.size( ) << "\n";
	Z.data.insert(Z.data.begin( ), X.data.begin( ) + X.data.size( ) - n * 2,
		X.data.end( ));
	
	//std::cout << 7 << "\n";

	for (int64_t i = static_cast<int64_t>(t) - 2; i >= 0; i--)
	{
		//std::cout << "circle\n";
		D2n_1n(Z, Y, n, q, r);
		
		Q.data.insert(Q.data.begin( ), q.data.begin( ), q.data.end( ));
		if (i > 0)
		{
			std::vector<uint64_t>( ).swap(Z.data);
			Z.data.insert(Z.data.begin( ), r.data.begin( ), r.data.end( ));
			std::vector<uint64_t>( ).swap(r.data);
			Z.data.insert(Z.data.begin( ), X.data.begin( ) + (static_cast<uint64_t>(i) - 1) * n,
				X.data.begin( ) + (static_cast<uint64_t>(i) * n));
			
		}
	}

	R = r;
	R >>= nor_count;

	while (Q.data.size( ) > 0 && Q.data[Q.data.size( ) - 1] == 0) Q.data.erase(Q.data.end( ) - 1);
	while (R.data.size( ) > 0 && R.data[R.data.size( ) - 1] == 0) R.data.erase(R.data.end( ) - 1);
}