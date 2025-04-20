#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <climits>
using namespace std;

typedef struct City
{
    int x;
    int y;
} City;

double d(const City &a, const City &b)
{
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

double tsp(int n, const vector<vector<double>> &dist)
{
    const double INF = 1e18; 
    int N = 1 << n;
    vector<vector<double>> dp(N, vector<double>(n, INF));
    dp[1][0] = 0; // スタートは都市0

    for (int S = 1; S < N; S++)
    {
        for (int u = 0; u < n; u++)
        {
            if (!(S & (1 << u)))
                continue; // 都市uが集合Sに含まれていなければスキップ

            for (int v = 0; v < n; v++)
            {
                if (S & (1 << v))
                    continue; // すでに訪問済みならスキップ

                int next = S | (1 << v);
                dp[next][v] = min(dp[next][v], dp[S][u] + dist[u][v]);
            }
        }
    }

    double res = INF;
    for (int i = 1; i < n; i++)
    {
        res = min(res, dp[(1 << n) - 1][i] + dist[i][0]);
    }

    return res;
}

int main()
{
    
    // ファイル読み込み
    string filename;
    cin >> filename;
    ifstream inputFile(filename);

    if (!inputFile)
    {
        cerr << "ファイルが開けません。" << endl;
        return 1;
    }

    // // ファイルからxとyを読み込む
    // int x, y;
    // vector<pair<int, int>> cities;
    // while (inputFile >> x >> y)
    // {
    //     cities.emplace_back(x, y);
    // }

    City c;
    vector<City> cities;
    while (inputFile >> c.x >> c.y)
    {
        cities.emplace_back(c);
    }

    inputFile.close();

    int n = cities.size();

    vector<vector<double>> dist(n, vector<double>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            dist[i][j] = d(cities[i], cities[j]);
        }
    }

    int result = tsp(n, dist);

    cout << result << endl;

    return 0;
}

