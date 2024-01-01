#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

/*5.4. Число делителей (8)
Будем называть нормальным набор из k чисел (a1, a2, ..., ak), если выполнены следующие условия:
каждое из чисел ai является делителем числа N;
выполняется неравенство a1 < a2 < ... < ak;
числа ai и ai+1 для всех i от 1 до k – 1 являются взаимно простыми;
произведение a1 a2...ak не превышает N.
Например, набор (2, 9, 10) является нормальным набором из 3 делителей числа 360.
Требуется написать программу, которая по заданным значениям N и k определяет количество нормальных наборов из k делителей числа N.
Ввод из файла INPUT.TXT. Первая строка входного файла содержит два целых числа: N и k (2 <= N <= 108, 2 <= k <= 10).
Вывод в файл OUTPUT.TXT. В выходном файле должно содержаться одно число — количество нормальных наборов из k делителей числа N.
Примеры
Ввод  1       Ввод  2
90 3          10 2
Вывод  1      Вывод  2
16            4
* Author: Ivaikov Leonid, "ПС-23"
* IDE: Microsoft Visual Studio 2022
*/

int gcd(int a, int b)
{
	while (b) {
		int t = a % b;
		a = b;
		b = t;
	}
	return a;
}

std::vector<int> GetDivisors(int N) {
	std::vector<int> divisors;
	for (int i = 1; i <= std::sqrt(N); ++i)
	{
		if (N % i == 0)
		{
			divisors.push_back(i);
		}
	}
	for (int i = divisors.size() - 1; i >= 0; --i)
	{
		if (divisors[i] * divisors[i] != N)
		{
			divisors.push_back(N / divisors[i]);
		}
	}
	return divisors;
}

std::vector<std::vector<bool>> GetCoprimeTable(std::vector<int> const& divisors) {
	int divisorsQty = divisors.size();

	std::vector<std::vector<bool>> coprime(divisorsQty, std::vector<bool>(divisorsQty));
	for (int i = 0; i < divisorsQty; ++i) {
		for (int j = i; j < divisorsQty; ++j) {
			coprime[i][j] = (gcd(divisors[i], divisors[j]) == 1);
			coprime[j][i] = coprime[i][j];
		}
	}
	return coprime;
}

std::vector<std::vector<double>> GetMinProduct(std::vector<int> const& divisors, int bound, int multiplierQty) {

	int divisorsQty = divisors.size();
	std::vector<std::vector<double>> minProduct(multiplierQty + 1, std::vector<double>(divisorsQty + 1));

	for (int i = 1; i <= multiplierQty; ++i)
	{
		for (int d = 0; d <= divisorsQty; ++d)
		{
			if (d == divisorsQty)
			{
				minProduct[i][d] = bound + 1;
			}
			else if (i == 1)
			{
				minProduct[i][d] = divisors[d];
			}
			else
			{
				minProduct[i][d] = minProduct[i - 1][d + 1] * divisors[d];
			}
		}
	}
	return minProduct;
}

int main()
{
	int N, k;
	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");

	fin >> N >> k;

	std::vector<int> divisors = GetDivisors(N);

	std::vector<std::vector<bool>> coprime = GetCoprimeTable(divisors);

	std::vector<std::vector<double>> minProduct = GetMinProduct(divisors, N, k);

	int total = 0;
	std::function<void(int, int, int)> count = [&](int lastDivisor, int p, int remaining)
	{
		if (remaining == 0)
		{
			++total;
			return;
		}
		for (int currDivisor = lastDivisor + 1; currDivisor < divisors.size() && p * minProduct[remaining][currDivisor] <= N; ++currDivisor)
		{
			if (coprime[lastDivisor][currDivisor])
			{
				count(currDivisor, p * divisors[currDivisor], remaining - 1);
			}
		}
	};

	for (int d = 0; d < divisors.size(); ++d)
	{
		count(d, divisors[d], k - 1);
	}

	fout << total;
	return 0;
}


