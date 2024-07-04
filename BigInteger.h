#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include "BigIntToString.h"
#include <cmath>
#include <intrin.h>


#define ELEMENT_SIZE 64

class BigInteger
{
private:
	// ������ ���������
	// ������� �� ��� ������ �����
	static void div2(const BigInteger& origin, BigInteger& left, BigInteger& right);
	// ���� ��������� - ����������� �������, ����� ��������
	static BigInteger karatsuba_mul(BigInteger& X, BigInteger& Y);
	// ��������� ���������� ������� ���� uint32 �� ��������� ������� ������ (� ����� ������� ����� �����)
	static void expansion(BigInteger& X, BigInteger& Y);
	// ������� ��������� (���������� ��������)
	static BigInteger naive_mul(const BigInteger& x, const BigInteger& y);
	// ������� ��� ��������� ������ �������� �� 3 ������
	static void multi_proc_adapter(BigInteger& sol, BigInteger& l, BigInteger& r);
	// ������� ���������
	static BigInteger mul(const BigInteger& X, const BigInteger& Y);

private:
	// ������ �������
	// Q - ������� ��� ��� �����
	// R - �������
	// ��� ��� ������� - ���������� ��������� ������ �������, ������ �������� �� � ��������� �������
	// ������� 128-������� ����� �� 64-������
	static void DivTwoDigitsByOne(uint64_t AH, uint64_t AL, uint64_t B,
		uint64_t q[2], uint64_t& s); 
	// ������� ���� ��������� (96 ���) �� ��� ��������� (64 ����)
	static void DivThreeHalvesByTwo(uint32_t a1, uint32_t a2,
		uint32_t a3, uint32_t b1, uint32_t b2, uint64_t& Q, uint64_t& R);

	
	// ���� ����������� ������� �������
	// ������� �������� ����� ������ 2n �� ����� n �����
	static void D2n_1n(BigInteger& A, BigInteger& B, size_t n, BigInteger& Q, BigInteger& R);
	// ������� 3-� ������ �������� ����� �� ����� n �����
	static void D3n_2n(BigInteger& A1, BigInteger& A2, BigInteger& A3, BigInteger& B1, BigInteger& B2, size_t n, BigInteger& Q, BigInteger& R);
	// ���������� �� ������� ������ n ��������� X
	static void expansion(BigInteger& X, size_t n);
	// ���������� �� ��������� ������� ������
	static uint64_t expansion(BigInteger& X);
	// ���������� �� ��������� ������� ������ � ����������� ��������
	static uint64_t expansion_on_right(BigInteger& X);

public:
	static void Division(BigInteger X, BigInteger Y, BigInteger& Q, BigInteger& R);

private:
	// ������ �������� � ��������� ����������� �����
	static BigInteger add(const BigInteger& X, const BigInteger& Y);
	static BigInteger sub(const BigInteger& X, const BigInteger& Y);

private:
	// ������ ��������� ����������� �����
	static bool smaller(const BigInteger& X, const BigInteger& Y);
	static bool bigger(const BigInteger& X, const BigInteger& Y);
	static bool smaller_eq(const BigInteger& X, const BigInteger& Y);
	static bool bigger_eq(const BigInteger& X, const BigInteger& Y);
	static bool eq(const BigInteger& X, const BigInteger& Y);
	static bool eq_not(const BigInteger& X, const BigInteger& Y);

public: // ������� private
	static enum class endi { LITTLE_ENDIAN = 1, BIG_ENDIAN = 2, UNKNOWN = -1 };
	const static endi BYTE_ORDER = static_cast<char>(32768) == 0 ? endi::LITTLE_ENDIAN : endi::BIG_ENDIAN;
	bool negative;
	std::vector<uint64_t> data;

	void clear()
	{
		negative = false;
		if (this->data.size() != 0)
			std::fill(this->data.begin(), this->data.end(), 0);
		std::vector<uint64_t>().swap(this->data);
	}

	static void uint64_to_binar(uint64_t x, bool* ret);
	static uint64_t binar_to_uint64(bool* x);
	static std::vector<bool> uint64_to_bool_vector(uint64_t x);
	
public:
	BigInteger();
	BigInteger(const BigInteger& other);
	BigInteger(const uint64_t item);
	// big-endian bits order (from bigger to smaller)
	BigInteger(std::vector<bool> binary_representation); 
	// little-endian byte order
	BigInteger(std::vector<uint64_t> qwords); 
	BigInteger(const std::string& item);
	~BigInteger() { clear(); }
	
	BigInteger operator+(const BigInteger& other) const;
	BigInteger operator-(const BigInteger& other) const;
	BigInteger operator*(const BigInteger& other) const;
	BigInteger operator/(const BigInteger& other) const;
	BigInteger operator%(const BigInteger& other) const;
	BigInteger operator^(const BigInteger& other) const;
	BigInteger operator&(const BigInteger& other) const;
	BigInteger operator|(const BigInteger& other) const;

	BigInteger operator+=(const BigInteger& other);
	BigInteger operator-=(const BigInteger& other);
	BigInteger operator*=(const BigInteger& other);
	BigInteger operator/=(const BigInteger& other);
	BigInteger operator%=(const BigInteger& other);
	BigInteger operator^=(const BigInteger& other);
	BigInteger operator&=(const BigInteger& other);
	BigInteger operator|=(const BigInteger& other);

	BigInteger& operator++( );
	BigInteger operator++(int x);
	BigInteger& operator--( );
	BigInteger operator--(int x);

	BigInteger operator+(uint64_t item) const;
	BigInteger operator-(uint64_t item) const;
	BigInteger operator*(uint64_t item) const;
	BigInteger operator/(uint64_t item) const;
	BigInteger operator%(uint64_t item) const;
	BigInteger operator^(uint64_t item) const;
	BigInteger operator&(uint64_t item) const;
	BigInteger operator|(uint64_t item) const;

	BigInteger operator+=(const uint64_t item);
	BigInteger operator-=(const uint64_t item);
	BigInteger operator*=(const uint64_t item);
	BigInteger operator/=(const uint64_t item);
	BigInteger operator%=(const uint64_t item);
	BigInteger operator^=(const uint64_t item);
	BigInteger operator&=(const uint64_t item);
	BigInteger operator|=(const uint64_t item);

	BigInteger operator<< (uint64_t r) const;
	BigInteger operator>> (uint64_t r) const;
	BigInteger operator<<=(uint64_t r);
	BigInteger operator>>=(uint64_t r);

	// big-endian bits order (from bigger to smaller)
	BigInteger& operator=(std::vector<bool> binary_representation);
	// little-endian byte order
	BigInteger& operator=(std::vector<uint64_t> qwords);
	BigInteger& operator=(const BigInteger& other);
	BigInteger& operator=(uint64_t item);
	BigInteger& operator=(const std::string& item);


	bool operator<(const BigInteger& other) const;
	bool operator>(const BigInteger& other) const;
	bool operator<=(const BigInteger& other) const;
	bool operator>=(const BigInteger& other) const;
	bool operator==(const BigInteger& other) const;
	bool operator!=(const BigInteger& other) const;

	bool operator==(const uint64_t item) const;
	bool operator<(const uint64_t item) const;
	bool operator>(const uint64_t item) const;
	bool operator<=(const uint64_t item) const;
	bool operator>=(const uint64_t item) const;

	friend std::ostream& operator<<(std::ostream& os, BigInteger bi);

	std::vector<bool>		get_binar_represent();
	std::vector<uint64_t>	get_qword_represent();
	std::string				get_text_represent();
	uint64_t				bit_count( );
	uint64_t				normalize( );
	void set_negative( ) { negative = true; }
	void set_not_negative( ) { negative = false; }

	friend BigInteger pow(const BigInteger& bi, uint32_t e);
	friend BigInteger pow(const BigInteger& bi, const BigInteger& e);

	friend BigInteger gcd(BigInteger v1, BigInteger v2);
	friend		void swap(BigInteger& a, BigInteger& b);
	static BigInteger random(uint64_t bits);
	static BigInteger fact(const BigInteger& bi);
};