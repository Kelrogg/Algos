#include <set>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

/*
* 1.9. Дамба (9)
* В рыбохозяйстве "Рыбнастол" принято решение о разведении карасей и щук. К сожалению, эти рыбы 
* не могут быть вместе в одном водоеме, так как щуки предпочитают питаться исключительно карасями. 
* Планируется каждое из k озер разделить дамбой на две не сообщающиеся друг с другом части. 
* На карте каждое i-е озеро представлено в определенном масштабе прямоугольным  участком  mi × ni 
* единиц, разбитым на единичные квадраты. Отдельный квадрат целиком занят водой или сушей. 
* Множество водных квадратов является связным. Это означает, что из любого водного квадрата озера 
* можно попасть в любой другой водный квадрат, последовательно переходя по водным квадратам через 
* их общую сторону. Такие последовательности связанных квадратов будем называть путями.
* Дамба имеет вид непрерывной ломаной, проходящей по сторонам квадратов. Два водных квадрата, 
* общая сторона которых принадлежит ломаной, становятся не сообщающимися напрямую друг с другом. 
* Требуется для каждого озера определить минимальную по количеству сторон длину дамбы, разделяющей 
* озеро на две не сообщающиеся между собой связные части. 
* Чтобы обеспечить доступ рыбаков к воде, каждая из двух полученных частей озера должна сообщаться 
* с берегом. Иными словами, каждая часть должна содержать водный квадрат, который либо сам находится
* на границе исходного прямоугольного участка, либо имеет общую сторону с квадратом суши, связанным
* с границей путем из квадратов суши.
* 
* Ввод. В первой строке содержится число k (1 ≤ k ≤ 10). В следующих строках следуют k блоков данных.
* Каждый блок описывает одно озеро. В первой строке блока содержатся числа mi и ni, разделенные 
* пробелом. В следующих mi строках находится матрица, представляющая озеро, по ni символов в строке.
* Символ '.' обозначает квадрат суши, а символ '#' – квадрат воды. Гарантируется наличие не менее 
* двух водных квадратов. Общая площадь озер s = m1 × n1 + m2 × n2 + … + mk × nk не должна 
* превосходить 10^6.
* 
* Выходные данные. В единственной строке должны выводиться через пробел k значений, определяющих 
* минимальные длины дамб. В результате каждое озеро должно быть разделено на две части так, что 
* водные клетки из разных частей не могут иметь общей стороны, не принадлежащей дамбе. 
* Тем не менее, касание этих клеток углами допускается. Каждая часть должна быть связана с берегом 
* так, как это определялось выше.
* Примеры
* Ввод
* 2
* 3 6
* ......
* .####.
* .####.
* 3 5
* ##.##
* #####
* ##.##
* Вывод
* 2 1
*
* Author: Ivaikov Leonid, "ПС-23"
* IDE: Microsoft Visual Studio 2022
*/

using std::pair;
using std::vector;
using std::string;

// Порядок важен
const int16_t dx[5] = { 0,1,-1,0, 0 };
const int16_t dy[5] = { 1,0,0,-1, 0 };

const char WATER = '#';
const char EARTH = '.';
const char SHORE = '_';

using lakeType = vector<vector<pair<unsigned, unsigned>>>;
using lakeRawType = vector<pair<unsigned, unsigned>>;

unsigned timer = UINT_MAX;
pair<unsigned, unsigned> start{UINT_MAX, UINT_MAX};

void ReadLake(lakeType& lake, std::ifstream& fin)
{
	char c;
	unsigned maxy, maxx;
	if (!fin.is_open())
	{
		std::cerr << "Failed to open ";
		exit(1);
	}

	fin >> maxy >> maxx;
	lake.clear();
	lake.resize(maxy, lakeRawType(maxx));

	for (unsigned y = 0; y < maxy; ++y)
	{
		for (unsigned x = 0; x < maxx; ++x)
		{
			fin >> c;
			lake[y][x].first = static_cast<unsigned>(c);
			lake[y][x].second = 0;

			if (start.first == UINT_MAX && c == WATER)
			{
				start.first = x;
				start.second = y;
			}
		}
	}
}

bool NotValid(lakeType const& lake, unsigned x, unsigned y)
{
	return lake.size() <= y || lake.at(0).size() <= x;
}

void JoinEarth(lakeType& lake, unsigned x, unsigned y) {

	lake[y][x].first = SHORE;
	unsigned xx, yy;

	for (uint8_t _ = 0; _ < 4; ++_) {
		xx = x + dx[_];
		yy = y + dy[_];

		if (NotValid(lake, xx, yy)) continue;

		if (lake[yy][xx].first == EARTH)
			JoinEarth(lake, xx, yy);
	}
}

void ExpandShore(lakeType& lake) {
	if (lake.empty())
		return;

	unsigned tempX{}, tempY{};
	// →→→
	while (tempX < lake[0].size()) {
		if (lake[0][tempX].first == EARTH)
			JoinEarth(lake, tempX, 0);
		++tempX;
	}
	--tempX;

	// ↓↓↓
	while (tempY < lake.size()) {
		if (lake[tempY][tempX].first == EARTH)
			JoinEarth(lake, tempX, tempY);
		++tempY;
	}
	--tempY;

	// From top-left
	tempX = 0;
	tempY = 1;
	// ↓↓↓
	while (tempY < lake.size()) {
		if (lake[tempY][0].first == EARTH)
			JoinEarth(lake, 0, tempY);
		++tempY;
	}
	--tempY;

	// →→→
	while (tempX < lake[0].size()) {
		if (lake[tempY][tempX].first == EARTH)
			JoinEarth(lake, tempX, tempY);
		++tempX;
	}
}

bool dfs(lakeType const& lake, unsigned x, unsigned y, std::set<pair<unsigned, unsigned>>& traversal) {

	unsigned xx, yy;
	traversal.emplace(x, y);
	for (uint8_t _ = 0; _ < 4; ++_) {
		xx = x + dx[_];
		yy = y + dy[_];

		if (NotValid(lake, xx, yy) ||
			traversal.find({ xx, yy }) != traversal.end()
		) continue;

		if (lake[yy][xx].first == SHORE) return true;
		if (lake[yy][xx].first > UCHAR_MAX || lake[yy][xx].first == WATER) 
			return dfs(lake, xx, yy, traversal);
	}
	return false;
}

bool ShoreSearch(lakeType const& lake, unsigned x, unsigned y, unsigned xx, unsigned yy) {
	std::set<pair<unsigned, unsigned>> traversal;

	traversal.emplace(xx, yy);
	if (!dfs(lake, x, y, traversal))
		return false;
	traversal.clear();

	traversal.emplace(x, y);
	if (!dfs(lake, xx, yy, traversal))
		return false;

	return true;
}

bool BridgeSearch(lakeType& lake, unsigned x, unsigned y, uint8_t p = -1) {
	unsigned xx, yy;
	lake[y][x].first = 
	lake[y][x].second = timer;
	--timer;
	uint8_t backStep = 3 - p;
	for (uint8_t _ = 0; _ < 4; ++_) {
		xx = x + dx[_];
		yy = y + dy[_];
		if (backStep == _ || NotValid(lake, xx, yy)) continue;

		unsigned to = lake[yy][xx].first;

		if (to == EARTH) continue;
		if (to > UCHAR_MAX && lake[y][x].second < to)
			lake[y][x].second = to;	// Обновить максиммальную для возврата вершину

		if (to == WATER)
		{
			if (BridgeSearch(lake, xx, yy, _))
				return true;
			lake[y][x].second = std::max(
				lake[y][x].second, 
				lake[yy][xx].second
			);

			if (lake[yy][xx].second < lake[y][x].first)
				if (ShoreSearch(lake, x, y, xx, yy))
					return true;
		}
	}
	return false;
}

void Print(std::ofstream& fout, vector<bool> const& answers)
{
	for (auto exist = answers.crbegin(); exist < answers.crend(); ++exist)
	{
		if (*exist)
			fout << 1;
		if (!*exist)
			fout << 2;
		fout << ' ';
	}
}

int main()
{
	// Ускоряем "stdin -> stdout"
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);	// связанность потоков

	uint16_t k;
	lakeType lake;

	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");

	if (!fout.is_open())
	{
		std::cerr << "Failed to open ";
		return 1;
	}
	if (!fin.is_open())
	{
		std::cerr << "Failed to open";
		return 1;
	}
	fin >> k;
	vector<bool> answers(k);

	for (; k;)
	{
		--k;
		ReadLake(lake, fin);
		ExpandShore(lake);
		answers[k] = BridgeSearch(lake, start.first, start.second);
		start = { UINT_MAX, UINT_MAX };
		timer = UINT_MAX;
	}

	Print(fout, answers);
	fout << std::endl;	// fout.flush() inside
    return 0;
}