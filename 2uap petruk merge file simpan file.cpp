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
