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

int main() {
    vector<string> bahanTersedia;
    inputBahanDariUser(bahanTersedia);

    vector<Resep> semuaResep = {
        {"Nasi Goreng", {"nasi", "bawang putih", "telur", "kecap manis", "minyak"}, 15, 80},
        {"Ayam Bakar", {"ayam", "bawang putih", "kecap manis", "cabai", "garam"}, 30, 90},
        {"Tumis Sayur", {"wortel", "bawang putih", "kol", "minyak", "garam"}, 10, 70},
        {"Telur Dadar", {"telur", "bawang merah", "garam", "minyak"}, 5, 50},
        {"Sup Ayam", {"ayam", "wortel", "bawang putih", "seledri", "garam"}, 25, 85},
        {"Capcay", {"wortel", "kol", "sawi", "bawang putih", "minyak"}, 20, 75},
        {"Soto Ayam", {"ayam", "bawang putih", "serai", "daun salam", "garam"}, 40, 90},
        {"Bakwan Jagung", {"jagung", "tepung terigu", "bawang merah", "seledri", "air"}, 15, 65},
        {"Nasi Uduk", {"beras", "santan", "daun salam", "serai", "garam"}, 30, 80},
        {"Sayur Lodeh", {"terong", "labu siam", "santan", "bawang merah", "cabai"}, 25, 70},
        {"Perkedel Kentang", {"kentang", "telur", "bawang merah", "garam", "minyak"}, 20, 65},
        {"Tempe Orek", {"tempe", "kecap manis", "bawang putih", "cabai", "gula"}, 15, 60},
        {"Mie Goreng", {"mie", "bawang putih", "kecap manis", "sayur", "telur"}, 20, 75},
        {"Ayam Goreng", {"ayam", "bawang putih", "kunyit", "garam", "minyak"}, 25, 85},
        {"Tahu Isi", {"tahu", "wortel", "bawang putih", "tepung terigu", "air"}, 20, 60},
        {"Gado-Gado", {"kentang", "tahu", "telur", "sayur", "saus kacang"}, 30, 85},
        {"Nasi Kuning", {"beras", "santan", "kunyit", "daun salam", "serai"}, 35, 90},
        {"Sayur Asem", {"labu siam", "melinjo", "jagung", "asam jawa", "bawang merah"}, 30, 70},
        {"Sate Ayam", {"ayam", "kecap manis", "saus kacang", "bawang putih", "tusuk sate"}, 35, 95},
        {"Semur Daging", {"daging", "bawang merah", "kecap manis", "kentang", "lada"}, 50, 100}
    };

    vector<Resep> resepValid;
    for (Resep& r : semuaResep) {
        int cocok = jumlahBahanCocok(bahanTersedia, r.bahan);
        if (cocok >= 3) {
            r.skorKecocokan = static_cast<double>(cocok) / r.bahan.size();
            resepValid.push_back(r);
        }
    }

    if (resepValid.empty()) {
        cout << "\n❌ Masukkan minimal 3 resep!\n";
        return 0;
    }

mergeSort(resepValid, 0, resepValid.size() - 1);

    int waktuMax;
    cout << "\nMasukkan batas waktu memasak maksimal (menit): ";
    cin >> waktuMax;
    cin.ignore();

    vector<Resep> hasil;
    int totalGizi = 0;

    vector<Resep> resepCocokPenuh;
    for (Resep& r : resepValid) {
        if (r.skorKecocokan == 1.0 && r.waktuMasak <= waktuMax) {
            resepCocokPenuh.push_back(r);
            totalGizi += r.nilaiGizi;
            waktuMax -= r.waktuMasak;
        }
    }

    if (waktuMax > 0) {
        vector<Resep> sisaResep;
        for (Resep& r : resepValid) {
            if (r.skorKecocokan < 1.0 && r.waktuMasak <= waktuMax) {
                sisaResep.push_back(r);
            }
        }
        vector<Resep> tambahan;
        totalGizi += knapsack(sisaResep, waktuMax, tambahan);
        hasil.insert(hasil.end(), tambahan.begin(), tambahan.end());
    }

    hasil.insert(hasil.begin(), resepCocokPenuh.begin(), resepCocokPenuh.end());

    cout << "\nRekomendasi Resep:\n";
    for (Resep& r : hasil) {
        cout << "- " << r.nama << " (Waktu: " << r.waktuMasak << " menit, Gizi: " << r.nilaiGizi << ")\n";
    }
    cout << "Total Gizi: " << totalGizi << endl;

    simpanKeFile(hasil, totalGizi);

    return 0;
}mergeSort(resepValid, 0, resepValid.size() - 1);

    int waktuMax;
    cout << "\nMasukkan batas waktu memasak maksimal (menit): ";
    cin >> waktuMax;
    cin.ignore();

    vector<Resep> hasil;
    int totalGizi = 0;

    vector<Resep> resepCocokPenuh;
    for (Resep& r : resepValid) {
        if (r.skorKecocokan == 1.0 && r.waktuMasak <= waktuMax) {
            resepCocokPenuh.push_back(r);
            totalGizi += r.nilaiGizi;
            waktuMax -= r.waktuMasak;
        }
    }

    if (waktuMax > 0) {
        vector<Resep> sisaResep;
        for (Resep& r : resepValid) {
            if (r.skorKecocokan < 1.0 && r.waktuMasak <= waktuMax) {
                sisaResep.push_back(r);
            }
        }
        vector<Resep> tambahan;
        totalGizi += knapsack(sisaResep, waktuMax, tambahan);
        hasil.insert(hasil.end(), tambahan.begin(), tambahan.end());
    }

    hasil.insert(hasil.begin(), resepCocokPenuh.begin(), resepCocokPenuh.end());

    cout << "\nRekomendasi Resep:\n";
    for (Resep& r : hasil) {
        cout << "- " << r.nama << " (Waktu: " << r.waktuMasak << " menit, Gizi: " << r.nilaiGizi << ")\n";
    }
    cout << "Total Gizi: " << totalGizi << endl;

    simpanKeFile(hasil, totalGizi);

    return 0;
}
