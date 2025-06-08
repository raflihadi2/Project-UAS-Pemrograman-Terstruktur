#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

struct Resep {
    string nama;
    vector<string> bahan;
    int waktuMasak;
    int nilaiGizi;
    double skorKecocokan;
};

// Fungsi input dari user
void inputBahanDariUser(vector<string>& bahanTersedia) {
    cout << "Masukkan bahan makanan yang tersedia (ketik 'selesai' untuk berhenti):\n";
    string bahan;
    while (true) {
        cout << "- ";
        getline(cin, bahan);
        if (bahan == "selesai") break;
        if (!bahan.empty()) bahanTersedia.push_back(bahan);
    }
}

// Hitung jumlah bahan cocok
int jumlahBahanCocok(const vector<string>& tersedia, const vector<string>& bahanResep) {
    int cocok = 0;
    for (const string& b : bahanResep) {
        if (find(tersedia.begin(), tersedia.end(), b) != tersedia.end()) {
            cocok++;
        }
    }
    return cocok;
}
