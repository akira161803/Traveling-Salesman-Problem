#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>

using namespace std;

void generateAndSave(const string& filename, int n) {
    ofstream ofs(filename);
    if (!ofs) {
        cerr << "ファイルオープン失敗: " << filename << endl;
        return;
    }
    

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, 100); 

    for (int i = 0; i < n; ++i)
        ofs << dist(mt) << " " << dist(mt) << endl; 

    cout << filename << " を生成しました。" << endl;
}

int main() {
    int n = 20;
    generateAndSave("cities1.txt", n);
    generateAndSave("cities2.txt", n);
    generateAndSave("cities3.txt", n);
    return 0;
}