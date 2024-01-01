/* 7.5. Волки и овцы (8)
* Волки и овцы. На координатной плоскости заданы отрезками N волков и M овец. 
* Пастух с ружьем находится в начале координат. 
* Выстрел поражает всех животных по направлению полета пули. 
* Найти наименьшее число выстрелов для того, чтобы убить всех волков и не тронуть овец.
* 
* Ввод из файла INPUT.TXT. В первой строке  задаются  через  пробел  значения N и M (1 <= N, M <= 10^3). 
* В следующих N строках - целочисленные координаты начала (X1, Y1) и конца (X2, Y2) отрезка, 
* соответствующего волку (-1000 <= X1, X2 <= 1000; 1 <=Y1, Y2 <= 1000). 
* Аналогично в следующих M строках указывается положение овец.
* Вывод в файл OUTPUT.TXT единственного целого числа либо сообщения “No solution”.
* Пример
* Ввод
* 2 1
* 1 1 2 3 
* -5 4 2 2
* 999 1000 1000 999
* Вывод
* 1
* 
* Author: Ivaikov Leonid, "ПС-23"
* IDE: Microsoft Visual Studio 2022
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

const bool WOLF = 1;
const bool SHEEP = 0;
const bool LEFT = 0;
const bool RIGHT = 1;
const unsigned DEAD = UINT_MAX;

struct Point {
	float x;
	bool Vid = LEFT;
	bool creature = SHEEP;
	unsigned pairIndex;
	Point(): x(), Vid(), creature(), pairIndex() {};
	Point(float x, char Vid, char creature, int pair) :
		x(x)
		, pairIndex(pair) 
	{
		if (Vid == '[')
			this->Vid = LEFT;
		if (Vid == ']')
			this->Vid = RIGHT;

		if (creature == 's')
			this->creature = SHEEP;
		if (creature == 'w')
			this->creature = WOLF;
	}

	bool operator<(Point const& other) const {
		bool tmp = Vid ^ creature;
		bool otherTmp = other.Vid ^ other.creature;
		return 
			std::tie(x, Vid, tmp) < 
			std::tie(other.x, other.Vid, otherTmp);
	}
	bool operator>(Point const& other) const {
		bool tmp = Vid ^ creature;
		bool otherTmp = other.Vid ^ other.creature;
		return
			std::tie(x, Vid, tmp) >
			std::tie(other.x, other.Vid, otherTmp);
	}
	void operator=(Point const& other) {
		std::tie(x, Vid, creature, pairIndex) =
        std::tie(other.x, other.Vid, other.creature, other.pairIndex);
	}
};

void swap(Point& lhs, Point& rhs) {
	float temp = lhs.x;
	lhs.x = rhs.x;
	rhs.x = temp;
}

float ctg(float adjacent, float opposite);

void Read(std::vector<Point>& v, std::istream& in);

void Qsort(std::vector<Point>& mas, int low = 0, int high = 0);

unsigned CountShots(std::vector<Point>& v);

int main() {
	std::vector<Point> scanMachine;

	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");

	unsigned shots;
	Read(scanMachine, fin);
	Qsort(scanMachine);
	shots = CountShots(scanMachine);
	if (shots == UINT_MAX)
		fout << "No solution";
	else
	    fout << shots;
	return 0;
}

//implementation

float ctg(float adjacent, float opposite) {
	return adjacent / opposite;
}

void Qsort(std::vector<Point>& mas, int low, int high) {

	if (!high)
		high = mas.size() - 1;

	int i = low;
	int j = high;

	Point mid = mas[(i + j) / 2];

	do {
		while (mas[i] < mid)
			++i;

		while (mas[j] > mid)
			--j;

		if (i <= j) {
			// change pointers of paired elements
			mas[mas[i].pairIndex].pairIndex = j;
			mas[mas[j].pairIndex].pairIndex = i;

			// swap elements
			Point tmp = mas[i];
			mas[i] = mas[j];
			mas[j] = tmp;

			++i;
			--j;
		}
	} while (i <= j);


	if (j > low) {
		Qsort(mas, low, j);
	}
	if (i < high)
		Qsort(mas, i, high);
}

void Read(std::vector<Point>& v, std::istream& in) {

	using size_t = unsigned int;

	unsigned N, M; // N - wolf, M - sheep
	int lhsx, lhsy, rhsx, rhsy; // left-hand side

	in >> N >> M;
	v.reserve((N + M) * 2);

	for (size_t i = 0; i < N; ++i) {
		in >> lhsx >> lhsy >> rhsx >> rhsy;
		Point lhsend(ctg(lhsx, lhsy), '[', 'w', v.size() + 1);
		Point rhsend(ctg(rhsx, rhsy), ']', 'w', v.size());

		if (lhsend > rhsend)
			swap(lhsend, rhsend);
		v.push_back(lhsend);
		v.push_back(rhsend);
	}

	for (size_t i = 0; i < M; ++i) {
		in >> lhsx >> lhsy >> rhsx >> rhsy;
		Point lhsend(ctg(lhsx, lhsy), '[', 's', v.size() + 1);
		Point rhsend(ctg(rhsx, rhsy), ']', 's', v.size());

		if (lhsend > rhsend)
			swap(lhsend, rhsend);
		v.push_back(lhsend);
		v.push_back(rhsend);
	}
}

void KillWolves(std::vector<Point>& v, unsigned from) {
	while (from) {
		if (v[from].creature == WOLF && 
			v[from].Vid == LEFT) {

			v[v[from].pairIndex].pairIndex = DEAD;
		}
		--from;
	}
}

unsigned CountShots(std::vector<Point>& v) {
	unsigned shotQty{}, sheepQty{}; // Qty - quantity
	unsigned leftSheep{};
	for (Point& p : v) {
		if (p.creature == SHEEP) {

			if (p.Vid == LEFT) {
				if (!sheepQty)
					leftSheep = &p - &v[0];
				++sheepQty;
			}

			if (p.Vid == RIGHT)
				--sheepQty;
		}
		if (p.creature == WOLF) {
			if (p.pairIndex == DEAD)
				continue;

			if (p.Vid == RIGHT) {

				if (!sheepQty)
					KillWolves(v, &p - &v[0]);

				if (sheepQty) {
					if (leftSheep <= (&v[p.pairIndex] - &v[0]))
						return UINT_MAX;
					KillWolves(v, leftSheep);
				}

				++shotQty;
			}
		}
	}
	return shotQty;
}