#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;
using Clock = chrono::high_resolution_clock;
using Duration = chrono::duration<double>;

struct City { double x, y; };

// ファイルから都市座標を読み込む
bool readCitiesFromFile(const string& filename, vector<City>& cities) {
    ifstream ifs(filename);
    if (!ifs) return false;
    cities.clear();
    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        double x, y;
        if (!(iss >> x >> y)) return false;
        cities.push_back({x, y});
    }
    return true;
}

// 距離計算
double distance(const City& a, const City& b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// 総距離計算
double totalDistance(const vector<City>& cities, const vector<int>& tour) {
    double dist = 0.0;
    for (size_t i = 0; i + 1 < tour.size(); ++i) {
        dist += distance(cities[tour[i]], cities[tour[i+1]]);
    }
    return dist;
}

// 挿入近傍による改善
bool insertionNeighborhood(vector<int>& tour, const vector<City>& cities) {
    int n = static_cast<int>(tour.size()) - 1;
    double bestDist = totalDistance(cities, tour);
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            if (i == j || i == j + 1) continue;
            vector<int> newTour = tour;
            int city = newTour[i];
            newTour.erase(newTour.begin() + i);
            newTour.insert(newTour.begin() + j, city);
            double newDist = totalDistance(cities, newTour);
            if (newDist < bestDist) {
                tour = newTour;
                return true;
            }
        }
    }
    return false;
}

// 巡回路表示
void showTour(const vector<int>& tour) {
    for (int t : tour) cout << t << " ";
}

int main() {
    vector<string> filenames = {"cities_50.txt", "cities_100.txt"};
    // vector<string> filenames = {"cities1.txt", "cities2.txt"};

    for (const auto& fname : filenames) {
        vector<City> cities;
        if (!readCitiesFromFile(fname, cities)) {
            cerr << "ファイル読み込み失敗: " << fname << endl;
            continue;
        }
        int n = static_cast<int>(cities.size());
        double bestDist = numeric_limits<double>::infinity();
        vector<int> bestTour;

        // 計算開始時刻
        auto t_start = Clock::now();

        // スタート地点を 0～n-1 で探索
        for (int start = 0; start < n; ++start) {
            vector<int> tour(n + 1);
            for (int i = 0; i < n; ++i) tour[i] = (start + i) % n;
            tour[n] = start;
            while (insertionNeighborhood(tour, cities));
            double d = totalDistance(cities, tour);
            if (d < bestDist) {
                bestDist = d;
                bestTour = tour;
            }
        }

        // 計算終了時刻
        auto t_end = Clock::now();
        Duration elapsed = t_end - t_start;

        // ファイルごとに出力
        cout << "=== ファイル: " << fname << " ===" << endl;
        cout << "最短経路：";
        showTour(bestTour);
        cout << endl;
        cout << "距離：" << bestDist << endl;
        cout << "計算時間：" << elapsed.count() << " 秒" << endl << endl;
    }
    return 0;
}
