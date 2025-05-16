#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <string>
#include <sstream>
#include <time.h>

using namespace std;

struct City {
    double x, y;
};


// テキストファイルから都市データを読み込む関数
vector<City> readCitiesFromFile(const string& filename) {
    vector<City> cities;
    ifstream infile(filename);

    if (!infile) {
        cerr << "ファイルを開けません: " << filename << endl;
        return cities;
    }

    double x1, y1;
    while (infile >> x1 >> y1) {
        cities.push_back({x1, y1});
    }

    return cities;
}

void NearestNeighbor(int n, int x[], int y[], int z[])
{
  float r = 0;
  srand((unsigned int)time(NULL));
  int start = rand()%n;  //スタート地点
  z[0] = start;  //スタート地点を格納
  z[n] = start;
  for(int i=0; i<n-1; i++)
    {
      int min = 100000000;  //最小距離
      int min_num = 0;  //最小距離の都市番号
      for(int j=0; j<n; j++)  //全都市との距離を測る
        {
          r = sqrt(pow(x[z[i]]-x[j],2)+pow(y[z[i]]-y[j],2));  //距離を計算

          for(int k=0; k<i; k++)
            {
              if(j == z[k])  //すでに通った都市は除外
              {
                r = 0;
              }
            }

          if(r<min && r != 0)  //最小距離を更新
            {
              min = r;
              min_num = j;
            }
        }
      z[i+1] = min_num;  //最小距離の都市番号を格納
    }
}

// 都市間の距離を計算
double distance(const City& a, const City& b) {
    double d = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return d;
}

void show(vector<int> tour) {
    for (int t : tour) cout << t << " ";
    cout << endl;
}

// 巡回路の総距離を計算（tour の末尾が 0 を含むこと前提）
double totalDistance(const vector<City>& cities, const vector<int>& tour) {
    double dist = 0;
    for (int i = 0; i < tour.size() - 1; ++i) {
        dist += distance(cities[tour[i]], cities[tour[i + 1]]);
    }
    return dist;
}

// 挿入近傍の改善
bool insertionNeighborhood(vector<int>& tour, const vector<City>& cities) {
    int n = tour.size() - 1; // 最後の 0 は固定
    bool improved = false;
    double bestDist = totalDistance(cities, tour);

    //show(tour);

    for (int i = 1; i < n; ++i) { // 0 は動かさない
        for (int j = 1; j < n; ++j) {
            if (i == j || i == j + 1) continue;

            vector<int> newTour = tour;
            int city = newTour[i];
            newTour.erase(newTour.begin() + i);
            newTour.insert(newTour.begin() + j, city);
            //show(newTour);
            

            double newDist = totalDistance(cities, newTour);
            if (newDist < bestDist) {
                bestDist = newDist;
                tour = newTour;
                improved = true;
                return true;
            }
        }
    }
    return improved;
}

int main() {

    clock_t start = clock();

    int n = 20;
    int x[n];
    int y[n];
    int z[n+1];
    
    vector<City> cities;

    string filename = "50-2.txt";
    cities = readCitiesFromFile(filename);

    for(int i=0; i<n; i++){
        x[i] = cities[i].x;
        y[i] = cities[i].y;
    }


    // 初期巡回路（0→1→2→3→4→0）
     vector<int> tour;
    // for (int i = 0; i < cities.size(); ++i) tour.push_back(i);
    //tour.push_back(0);

    //show(tour);

    NearestNeighbor(n,  x,  y,  z);
    for(int i; i<n; i++){
      cities[i].x = x[i];
      cities[i].y = y[i];
      tour.push_back(z[i]);
    }
    tour.push_back(z[0]);

    cout << filename << "の結果" << endl;

/*
    for (int i=0; i<20; i++ ) {
        cout << "x: " << cities[i].x << ", y: " << cities[i].y << endl;
    }
    
    cout << "Initial tour: " ;
    for (int t : tour) cout << t << " ";
    cout << endl;
    cout << "Initial distance: " << totalDistance(cities, tour) << endl;
*/
    
    while (insertionNeighborhood(tour, cities)) {
        //cout << "Improved distance: " << totalDistance(cities, tour) << endl;
    }

    cout << "Final tour  : ";
    for (int t : tour) cout << t << " ";
    cout << "\nFinal distance: " << totalDistance(cities, tour) << endl;

clock_t end = clock();

const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "time:" << time << "秒" << endl;

    return 0;
}