#include <iostream>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

const int MAKS_BUKU = 100;
const int MAKS_PENGGUNA = 50;
const int PANJANG_MAKS_STRING = 100;

struct Buku {
    int id;
    string judul;
    string penulis;
    bool dipinjam;
    int idPenggunaPeminjam;
};

struct Pengguna {
    int idPengguna;
    string nama;
};

Buku perpustakaan[MAKS_BUKU];
Pengguna pengguna[MAKS_PENGGUNA];
int jumlahBuku = 0;
int jumlahPengguna = 0;
int idBukuBerikutnya = 1;
int idPenggunaBerikutnya = 1;

void tampilkanMenu();
void tambahBuku();
void tampilkanSemuaBuku();
void cariBuku();
void pinjamBuku();
void kembalikanBuku();
void daftarPengguna();
void tampilkanSemuaPengguna();
int cariIndeksBukuBerdasarkanId(int idBuku);
int cariIndeksPenggunaBerdasarkanId(int idPengguna);
void bersihkanBufferInput();
void urutkanBukuBerdasarkanJudul();

int main() {
    int pilihan = 0;

    do {
        tampilkanMenu();
        cout << "Masukkan pilihan Anda: ";
        cin >> pilihan;
        bersihkanBufferInput();

        switch (pilihan) {
            case 1:
                tambahBuku();
                break;
            case 2:
                tampilkanSemuaBuku();
                break;
            case 3:
                cariBuku();
                break;
            case 4:
                pinjamBuku();
                break;
            case 5:
                kembalikanBuku();
                break;
            case 6:
                daftarPengguna();
                break;
            case 7:
                tampilkanSemuaPengguna();
                break;
            case 8:
                urutkanBukuBerdasarkanJudul();
                break;
            case 0:
                cout << "Keluar dari sistem. Sampai jumpa!" << endl;
                break;
            default:
                if (pilihan != 0) {
                    cout << "Pilihan tidak valid atau input tidak sesuai. Silakan coba lagi." << endl;
                }
        }
        cout << endl;
    } while (pilihan != 0);

    return 0;
}

void bersihkanBufferInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void tampilkanMenu() {
    cout << "--- Sistem Manajemen Perpustakaan Digital ---" << endl;
    cout << "1. Tambah Buku Baru" << endl;
    cout << "2. Tampilkan Semua Buku" << endl;
    cout << "3. Cari Buku (berdasarkan Judul)" << endl;
    cout << "4. Pinjam Buku" << endl;
    cout << "5. Kembalikan Buku" << endl;
    cout << "6. Daftar Pengguna Baru" << endl;
    cout << "7. Tampilkan Semua Pengguna" << endl;
    cout << "8. Urutkan Buku Berdasarkan Judul (Alfabetis)" << endl;
    cout << "0. Keluar" << endl;
    cout << "---------------------------------------------" << endl;
}

void urutkanBukuBerdasarkanJudul() {
    if (jumlahBuku < 2) {
        cout << "Tidak cukup buku untuk diurutkan." << endl;
        return;
    }

    for (int i = 0; i < jumlahBuku - 1; ++i) {
        for (int j = 0; j < jumlahBuku - i - 1; ++j) {
            if (perpustakaan[j].judul > perpustakaan[j + 1].judul) {
                Buku temp = perpustakaan[j];
                perpustakaan[j] = perpustakaan[j + 1];
                perpustakaan[j + 1] = temp;
            }
        }
    }
    cout << "Perpustakaan berhasil diurutkan berdasarkan judul (alfabetis)." << endl;
}

void tambahBuku() {
    if (jumlahBuku >= MAKS_BUKU) {
        cout << "Perpustakaan penuh. Tidak dapat menambahkan lebih banyak buku." << endl;
        return;
    }

    Buku& bukuBaru = perpustakaan[jumlahBuku];
    bukuBaru.id = idBukuBerikutnya++;
    cout << "Masukkan judul buku: ";
    getline(cin, bukuBaru.judul);
    cout << "Masukkan nama penulis: ";
    getline(cin, bukuBaru.penulis);
    bukuBaru.dipinjam = false;
    bukuBaru.idPenggunaPeminjam = -1;

    jumlahBuku++;
    cout << "Buku berhasil ditambahkan dengan ID: " << bukuBaru.id << endl;
}

void tampilkanSemuaBuku() {
    if (jumlahBuku == 0) {
        cout << "Tidak ada buku di perpustakaan." << endl;
        return;
    }

    cout << "--- Daftar Semua Buku ---" << endl;
    cout << "ID\tJudul\t\tPenulis\t\tStatus" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    for (int i = 0; i < jumlahBuku; ++i) {
        cout << perpustakaan[i].id << "\t"
             << perpustakaan[i].judul << "\t\t"
             << perpustakaan[i].penulis << "\t\t";
        if (perpustakaan[i].dipinjam) {
            cout << "Dipinjam (ID Pengguna: " << perpustakaan[i].idPenggunaPeminjam << ")" << endl;
        } else {
            cout << "Tersedia" << endl;
        }
    }
    cout << "---------------------------------------------------------------------" << endl;
}

void cariBuku() {
    if (jumlahBuku == 0) {
        cout << "Tidak ada buku di perpustakaan untuk dicari." << endl;
        return;
    }

    string kataCari;
    cout << "Masukkan judul buku yang ingin dicari: ";
    getline(cin, kataCari);

    bool ditemukan = false;
    cout << "--- Hasil Pencarian (Judul) ---" << endl;
    cout << "ID\tJudul\t\tPenulis\t\tStatus" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    for (int i = 0; i < jumlahBuku; ++i) {
        if (perpustakaan[i].judul.find(kataCari) != string::npos) {
            cout << perpustakaan[i].id << "\t"
                 << perpustakaan[i].judul << "\t\t"
                 << perpustakaan[i].penulis << "\t\t";
            if (perpustakaan[i].dipinjam) {
                cout << "Dipinjam (ID Pengguna: " << perpustakaan[i].idPenggunaPeminjam << ")" << endl;
            } else {
                cout << "Tersedia" << endl;
            }
            ditemukan = true;
        }
    }
    cout << "---------------------------------------------------------------------" << endl;

    if (!ditemukan) {
        cout << "Tidak ada buku yang ditemukan sesuai dengan kata kunci pencarian Anda." << endl;
    }
}

int cariIndeksBukuBerdasarkanId(int idBuku) {
    for (int i = 0; i < jumlahBuku; ++i) {
        if (perpustakaan[i].id == idBuku) {
            return i;
        }
    }
    return -1;
}

int cariIndeksPenggunaBerdasarkanId(int idPengguna) {
    for (int i = 0; i < jumlahPengguna; ++i) {
        if (pengguna[i].idPengguna == idPengguna) {
            return i;
        }
    }
    return -1;
}

void pinjamBuku() {
    if (jumlahBuku == 0) {
        cout << "Tidak ada buku yang tersedia di perpustakaan untuk dipinjam." << endl;
        return;
    }
    if (jumlahPengguna == 0) {
        cout << "Tidak ada pengguna yang terdaftar. Silakan daftarkan pengguna terlebih dahulu." << endl;
        return;
    }

    int idBuku = 0, idPengguna = 0;
    cout << "Masukkan ID Buku yang ingin dipinjam: ";
    cin >> idBuku;
    bersihkanBufferInput();

    cout << "Masukkan ID Pengguna Anda: ";
    cin >> idPengguna;
    bersihkanBufferInput();

    if (idBuku <= 0) {
        cout << "ID Buku yang dimasukkan tidak valid." << endl;
        return;
    }
     if (idPengguna <= 0) {
        cout << "ID Pengguna yang dimasukkan tidak valid." << endl;
        return;
    }

    int indeksBuku = cariIndeksBukuBerdasarkanId(idBuku);
    int indeksPengguna = cariIndeksPenggunaBerdasarkanId(idPengguna);

    if (indeksBuku == -1) {
        cout << "Buku dengan ID " << idBuku << " tidak ditemukan." << endl;
        return;
    }
    if (indeksPengguna == -1) {
        cout << "Pengguna dengan ID " << idPengguna << " tidak ditemukan. Silakan daftar terlebih dahulu." << endl;
        return;
    }

    if (perpustakaan[indeksBuku].dipinjam) {
        cout << "Buku '" << perpustakaan[indeksBuku].judul << "' sedang dipinjam oleh Pengguna ID: " << perpustakaan[indeksBuku].idPenggunaPeminjam << "." << endl;
    } else {
        perpustakaan[indeksBuku].dipinjam = true;
        perpustakaan[indeksBuku].idPenggunaPeminjam = idPengguna;
        cout << "Buku '" << perpustakaan[indeksBuku].judul << "' berhasil dipinjam oleh " << pengguna[indeksPengguna].nama << " (ID Pengguna: " << idPengguna << ")." << endl;
    }
}

void kembalikanBuku() {
    if (jumlahBuku == 0) {
        cout << "Tidak ada buku di perpustakaan untuk dikembalikan." << endl;
        return;
    }
    int idBuku = 0;
    cout << "Masukkan ID Buku yang ingin dikembalikan: ";
    cin >> idBuku;
    bersihkanBufferInput();

    if (idBuku <= 0) {
        cout << "ID Buku yang dimasukkan tidak valid." << endl;
        return;
    }

    int indeksBuku = cariIndeksBukuBerdasarkanId(idBuku);

    if (indeksBuku == -1) {
        cout << "Buku dengan ID " << idBuku << " tidak ditemukan." << endl;
        return;
    }

    if (!perpustakaan[indeksBuku].dipinjam) {
        cout << "Buku '" << perpustakaan[indeksBuku].judul << "' saat ini tidak sedang dipinjam." << endl;
    } else {
        cout << "Buku '" << perpustakaan[indeksBuku].judul << "' berhasil dikembalikan. (Sebelumnya dipinjam oleh ID Pengguna: " << perpustakaan[indeksBuku].idPenggunaPeminjam << ")" << endl;
        perpustakaan[indeksBuku].dipinjam = false;
        perpustakaan[indeksBuku].idPenggunaPeminjam = -1;
    }
}

void daftarPengguna() {
    if (jumlahPengguna >= MAKS_PENGGUNA) {
        cout << "Batas maksimum pengguna telah tercapai. Tidak dapat mendaftarkan lebih banyak pengguna." << endl;
        return;
    }

    Pengguna& penggunaBaru = pengguna[jumlahPengguna];
    penggunaBaru.idPengguna = idPenggunaBerikutnya++;
    cout << "Masukkan nama pengguna: ";
    getline(cin, penggunaBaru.nama);

    jumlahPengguna++;
    cout << "Pengguna '" << penggunaBaru.nama << "' berhasil didaftarkan dengan ID Pengguna: " << penggunaBaru.idPengguna << endl;
}

void tampilkanSemuaPengguna() {
    if (jumlahPengguna == 0) {
        cout << "Tidak ada pengguna yang terdaftar dalam sistem." << endl;
        return;
    }

    cout << "--- Daftar Semua Pengguna ---" << endl;
    cout << "ID Pengguna\tNama" << endl;
    cout << "-------------------------" << endl;
    for (int i = 0; i < jumlahPengguna; ++i) {
        cout << pengguna[i].idPengguna << "\t\t" << pengguna[i].nama << endl;
    }
    cout << "-------------------------" << endl;
}
