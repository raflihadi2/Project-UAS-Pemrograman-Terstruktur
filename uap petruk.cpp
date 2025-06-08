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

// Merge Sort berdasarkan skor kecocokan
void merge(vector<Resep>& resep, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    vector<Resep> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = resep[l + i];
    for (int i = 0; i < n2; i++) R[i] = resep[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].skorKecocokan > R[j].skorKecocokan) {
            resep[k++] = L[i++];
        } else {
            resep[k++] = R[j++];
        }
    }
    while (i < n1) resep[k++] = L[i++];
    while (j < n2) resep[k++] = R[j++];
}

void mergeSort(vector<Resep>& resep, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(resep, l, m);
        mergeSort(resep, m + 1, r);
        merge(resep, l, m, r);
    }
}

// Knapsack berdasarkan waktu masak dan nilai gizi
int knapsack(vector<Resep>& resep, int waktuMax, vector<Resep>& hasil) {
    int n = resep.size();
    vector<vector<int>> dp(n + 1, vector<int>(waktuMax + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= waktuMax; w++) {
            if (resep[i - 1].waktuMasak <= w) {
                dp[i][w] = max(
                    resep[i - 1].nilaiGizi + dp[i - 1][w - resep[i - 1].waktuMasak],
                    dp[i - 1][w]
                );
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int w = waktuMax;
    for (int i = n; i > 0 && w > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            hasil.push_back(resep[i - 1]);
            w -= resep[i - 1].waktuMasak;
        }
    }

    return dp[n][waktuMax];
}

// Simpan hasil ke file
void simpanKeFile(const vector<Resep>& hasil, int totalGizi) {
    ofstream out("rekomendasi.txt");
    out << "Resep Terpilih:\n";
    for (const Resep& r : hasil) {
        out << "- " << r.nama << " (Waktu: " << r.waktuMasak << " menit, Gizi: " << r.nilaiGizi << ")\n";
    }
    out << "Total Gizi: " << totalGizi << "\n";
    out.close();
}
