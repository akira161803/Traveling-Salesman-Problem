#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    const int xMin = 0, xMax = 100;
    const int yMin = 0, yMax = 100;

    cout << "都市の数を入力してください：";
    int numCities = 20; cin >> numCities;

    string fileName = "cities_" + to_string(numCities) + ".txt";
    ofstream outFile(fileName);
    if (!outFile) {
        cerr << "ファイルを開けませんでした。\n";
        return 1;
    }

    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < numCities; ++i) {
        int x = xMin + rand() % (xMax - xMin + 1);
        int y = yMin + rand() % (yMax - yMin + 1);
        outFile << x << " " << y << endl;
    }

    outFile.close();
    cout << numCities << "個の都市座標を" << fileName << "に出力しました。\n";

    return 0;
}