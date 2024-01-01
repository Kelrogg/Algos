/*  Шахматная доска состоит из N строк и M столбцов(1 ≤ N, M ≤ 15).Имеется единственный белый король.У черных в распоряжении кони, слоны и ладьи.По заданному расположению фигур требуется взять белым королем все черные фигуры за минимальное число ходов, не находясь при этом под боем.
*  Ввод.В первой строке находятся числа M и N.В следующих N строках по M символов задано расположение фигур.Имеются  следующие обозначения :
*  «.» – пустая клетка;
*  «* » – белый король;
*  «K» – черный конь;
*  «B» – черный слон;
*  «R» – черная ладья.
*  Белый король в начальной позиции не атакован.Общее число черных фигур не более 14.
*  Вывод.В единственной строке выводится минимальное число ходов, необходимое для взятия черных фигур.Вывести 0, если черных фигур нет, и - 1, если взять все черные фигуры невозможно.
*  Ограничения.На одном тесте время счета не должно превышать 6 сек.Максимально допустимая память 256 мгб.
*  Пример
*  Ввод
*  7 9
*  .........
*  .........
*  .........
*  ..R.K.R..
*  .........
*  .........
*  *........
*  Вывод
*  9
*/  

/*
* Выполнил студент группы ПС-23 Ивайков Леонид
* В среде Visual Studio 2022
*/

#include<algorithm>
#include<iostream>
#include<cstring>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<vector>
#include<string>
#include<cstdio>
#include<bitset>
#include<queue>
#include<cmath>
#include<array>
#include<map>
#include<set>

#define CLR(a, b) memset(a, b, sizeof(a))

using std::array;
using std::queue;
using std::ifstream;

const int ENEMY = 14;
const int SET = (1 << ENEMY);
const int MAXN = 15;

using Board = array<array<char, MAXN>, MAXN>;
using BoardId = array<array<unsigned char, MAXN>, MAXN>;
int n, m, kingX = 0, kingY = 0, total = 0;

Board g;
BoardId id;

//can[y][x][state] = 1 : Поле находится под боем
//visited[y][x][state] = 1 ： Посещённая клетка ( +BONUS: великолепная отладочная информация)
array<array<array<bool, SET>, MAXN>, MAXN> can;
array<array<array<bool, SET>, MAXN>, MAXN> visited;

inline bool outside(int maxX, int maxY, int x, int y)
{
    return x < 0 || x >= maxX || y < 0 || y >= maxY;
}
//Находится ли чёрная фигура в текущем состоянии в точке <x, y> ?
inline bool has(int state, int x, int y)
{
    return isalpha(g[y][x]) && (state & (1 << id[y][x]));
}

// s - state
struct Node
{
    int x{}, y{}, s{}, steps{};
    Node() {}
    Node(int a, int b, int c, int d) :x(a), y(b), s(c), steps(d) {}
};

//Инициализировать во всех состояниях все точки, которые будут атакованы
void init()
{
    short int dkx[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };    // Knight, deltaKx
    short int dky[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };    // смещения - возможные результаты хода коня (рыцаря)

    for (int state = 1; state < total + 1; ++state)
    {
        //Если чёрная фигура в состоянии s содержит g[i][j], то обновить диапазон атаки g[i][j]
        for (int y = 0; y < n; ++y) for (int x = 0; x < m; ++x) 
            if (isalpha(g[y][x]) && (state & (1 << id[y][x]))) 
            {
                    if (g[y][x] == 'K')
                    {
                        for (int k = 0; k < 8; ++k)
                        {
                            int newX = x + dkx[k], newY = y + dky[k];
                            if (outside(m, n, newX, newY) || has(state, newX, newY)) continue;
                            can[newY][newX][state] = 1;
                        }
                    }
                    else if (g[y][x] == 'B')
                    {
                        int newX = x, newY = y;
                        while (true) 
                        {
                            ++newX, ++newY;
                            if (outside(m, n, newX, newY) || has(state, newX, newY)) break;
                            can[newY][newX][state] = 1;
                        }
                        newX = x, newY = y;
                        while (true)
                        {
                            --newX, --newY;
                            if (outside(m, n, newX, newY) || has(state, newX, newY)) break;
                            can[newY][newX][state] = 1;
                        }
                        newX = x, newY = y;
                        while (true)
                        {
                            ++newX, --newY;
                            if (outside(m, n, newX, newY) || has(state, newX, newY)) break;
                            can[newY][newX][state] = 1;
                        }
                        newX = x, newY = y;
                        while (true)
                        {
                            --newX, ++newY;
                            if (outside(m, n, newX, newY) || has(state, newX, newY)) break;
                            can[newY][newX][state] = 1;
                        }
                    }
                    else
                    {
                        int newX = x, newY = y;
                        while (true)
                        {
                            ++newX;
                            if (newX >= m || has(state, newX, newY)) break;
                            can[newY][newX][state] = 1;
                        }
                        newX = x, newY = y;
                        while (true)
                        {
                            --newX;
                            if (newX < 0 || has(state, newX, newY)) break;
                            can[newY][newX][state] = 1;
                        }
                        newX = x, newY = y;
                        while (true)
                        {
                            ++newY;
                            if (newY >= n || has(state, newX, newY)) break;
                            can[newY][newX][state] = 1;
                        }
                        newX = x, newY = y;
                        while (true)
                        {
                            --newY;
                            if (newY < 0 || has(state, newX, newY)) break;
                            can[newY][newX][state] = 1;
                        }
                    }
            }
    }
}

void bfs()
{
    int dwx[8] = { 0, 0, 1, -1, 1, 1, -1, -1 }; // croWn, deltaWx
    int dwy[8] = { 1, -1, 0, 0, 1, -1, 1, -1 }; // смещения - возможные ходы короля (короны)
    
    queue<Node> q; q.push(Node(kingX, kingY, total, 0));
    visited[kingY][kingX][total] = 1;
    while (!q.empty())
    {
        Node kingField = q.front(); q.pop();
        if (kingField.s == 0)
        {
            std::cout << kingField.steps << std::endl;
            return;
        }
        for (int i = 0; i < 8; ++i)
        {
            int newX = kingField.x + dwx[i], newY = kingField.y + dwy[i];
            if (outside(m, n, newX, newY)) continue;

            //Если <newX, newY> содержит чёрную фигуру
            if (has(kingField.s, newX, newY))
            {
                int state = kingField.s & (total - (1 << id[newY][newX]));
                
                //если можно съесть
                if (!visited[newY][newX][state] && !can[newY][newX][state])
                {
                    q.push(Node(newX, newY, state, kingField.steps + 1));
                    visited[newY][newX][state] = 1;
                }
            }
            else if (!can[newY][newX][kingField.s])
            {
                int state = kingField.s;
                if (!visited[newY][newX][state])
                {
                    q.push(Node(newX, newY, state, kingField.steps + 1));
                    visited[newY][newX][state] = 1;
                }
            }
        }
    }
    puts("-1");
    return;
}

int main()
{    
    ifstream fin("input.txt");
    fin >> n >> m;
    for (int y = 0; y < n; ++y)
    {
        for (int x = 0; x < m; ++x)
        {
            fin >> g[y][x];
            if (g[y][x] == '*') kingY = y, kingX = x;
            else if (isalpha(g[y][x])) id[y][x] = total++;
        }
    }

    if (total == 0)
    {
        puts("0");
        return 0;
    }
    total = (1 << total) - 1;
    init();

    bfs();

    return 0;
}

