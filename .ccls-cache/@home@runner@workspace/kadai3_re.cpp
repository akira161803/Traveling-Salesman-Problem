#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

using Clock = chrono::high_resolution_clock;
using Duration = chrono::duration<double>;

struct City {
  double x, y;
};

// テキストファイルから都市データを読み込む
vector<City> readCitiesFromFile(const string &filename) {
  vector<City> cities;
  ifstream infile(filename);
  if (!infile) {
    cerr << "ファイルを開けません: " << filename << endl;
    return cities;
  }
  double x, y;
  while (infile >> x >> y) {
    cities.push_back({x, y});
  }
  return cities;
}

// 都市間の距離を計算
inline double distance(const City &a, const City &b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// 巡回路の総距離を計算（tour の末尾はスタート地点）
double totalDistance(const vector<City> &cities, const vector<int> &tour) {
  double dist = 0;
  for (size_t i = 0; i + 1 < tour.size(); ++i) {
    dist += distance(cities[tour[i]], cities[tour[i + 1]]);
  }
  return dist;
}

// 巡回路を標準出力
void showTour(const vector<int> &tour) {
  for (int v : tour)
    cout << v << " ";
  cout << endl;
}

// 挿入近傍による改善（Best-improvement）
bool insertionNeighborhood(vector<int> &tour, const vector<City> &cities) {
  int n = static_cast<int>(tour.size()) - 1; // 最後の要素はスタート地点
  double currentDist = totalDistance(cities, tour);
  double bestDist = currentDist;
  int best_i = -1, best_j = -1;
  vector<int> bestTour;

  // すべての挿入移動を試して最良を探す
  for (int i = 1; i < n; ++i) {
    for (int j = 1; j < n; ++j) {
      if (i == j || i == j + 1)
        continue;
      vector<int> newTour = tour;
      int city = newTour[i];
      newTour.erase(newTour.begin() + i);
      newTour.insert(newTour.begin() + j, city);
      double newDist = totalDistance(cities, newTour);
      if (newDist < bestDist) {
        bestDist = newDist;
        best_i = i;
        best_j = j;
        bestTour = newTour;
      }
    }
  }

  // 改善があれば最良の移動を適用
  if (best_i != -1) {
    tour = move(bestTour);
    return true;
  }
  return false;
}

// 指定された start から Nearest Neighbor で初期巡回路を作成
void NearestNeighbor(int n, const vector<City> &cities, vector<int> &tour,
                     int start) {
  vector<bool> visited(n, false);
  tour.resize(n + 1);
  tour[0] = start;
  visited[start] = true;
  for (int i = 0; i < n - 1; ++i) {
    int cur = tour[i];
    double minD = numeric_limits<double>::infinity();
    int nextCity = -1;
    for (int j = 0; j < n; ++j) {
      if (visited[j])
        continue;
      double d = distance(cities[cur], cities[j]);
      if (d < minD) {
        minD = d;
        nextCity = j;
      }
    }
    tour[i + 1] = nextCity;
    visited[nextCity] = true;
  }
  tour[n] = tour[0]; // 巡回を閉じる
}

int main() {
  vector<string> filenames = {"cities1.txt", "cities2.txt", "cities3.txt","50-2.txt"};

  for (const auto &fname : filenames) {
    auto cities = readCitiesFromFile(fname);
    if (cities.empty())
      continue;
    int n = static_cast<int>(cities.size());

    double bestDist = numeric_limits<double>::infinity();
    vector<int> bestTour;

    auto t_start = Clock::now();

    // Multiple Start: 0～n-1 の各 start で NearestNeighbor + 挿入近傍
    for (int start = 0; start < n; ++start) {
      vector<int> tour;
      NearestNeighbor(n, cities, tour, start);
      // 局所探索
      while (insertionNeighborhood(tour, cities))
        ;
      double d = totalDistance(cities, tour);
      if (d < bestDist) {
        bestDist = d;
        bestTour = tour;
      }
    }

    auto t_end = Clock::now();
    Duration elapsed = t_end - t_start;

    // 結果出力
    cout << "=== ファイル: " << fname << " ===" << endl;
    cout << "最短巡回路: ";
    showTour(bestTour);
    cout << "距離: " << bestDist << endl;
    cout << "計算時間：" << elapsed.count() << " 秒" << endl << endl;
  }

  return 0;
}
