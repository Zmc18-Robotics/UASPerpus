#include <iostream>
#include <iomanip>
using namespace std;

const int MAX_BOOKS = 9;
const int MAX_USERS = 6;
const int MAX_HISTORY = 10;

bool bandingString(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

void salinString(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool mengandungSubstring(const char* str, const char* sub) {
    for (int i = 0; str[i] != '\0'; i++) {
        int j = 0;
        while (sub[j] != '\0' && str[i + j] == sub[j]) {
            j++;
        }
        if (sub[j] == '\0') return true;
    }
    return false;
}

struct Buku {
    int idBuku;
    char judul[50];
    char penulis[30];
    bool SedangDipinjam;
    int barisRak;
    int kolomRak;
    Buku* next;
};

struct QueueNode {
    int userId;
    QueueNode* next;
};

struct Queue {
    QueueNode* front;
    QueueNode* rear;
};

struct RiwayatPinjam {
    int idBuku;
};

struct Pengguna {
    int idPengguna;
    char nama[30];
    RiwayatPinjam riwayat[MAX_HISTORY];
    int jumlahRiwayat;
};

Buku* head = NULL;
Pengguna daftarPengguna[MAX_USERS] = {
    {1, "Nafhan", {}, 0},
    {2, "Cakra", {}, 0},
    {3, "Arif", {}, 0},
    {4, "Zidane", {}, 0},
    {5, "Rivaldi", {}, 0},
    {6, "Dafi", {}, 0}
};
Queue antreanBuku[MAX_BOOKS];

void inisialisasiBuku() {
    int counterId = 101;
    const char judulBuku[3][3][50] = {
        {"C++ Dasar", "Logika Pemrograman", "Struktur Data"},
        {"Alam Semesta", "Dunia Sains", "Kosmos"},
        {"Sejarah Dunia", "Perang Dunia", "Imperium"}
    };
    const char penulisBuku[3][3][30] = {
        {"Walter Savitch", "Martin Fowler", "Robert Lafore"},
        {"Bill Bryson", "Brian Cox", "Carl Sagan"},
        {"E.H.Gombrich", "Antony Beevor", "Niall Ferguson"}
    };

    for (int cat = 0; cat < 3; cat++) {
        for (int i = 0; i < 3; i++) {
            Buku* bukuBaru = new Buku;
            bukuBaru->idBuku = counterId++;
            salinString(bukuBaru->judul, judulBuku[cat][i]);
            salinString(bukuBaru->penulis, penulisBuku[cat][i]);
            bukuBaru->SedangDipinjam = false;
            bukuBaru->barisRak = cat + 1;
            bukuBaru->kolomRak = i + 1;
            bukuBaru->next = head;
            head = bukuBaru;
        }
    }

    for (int i = 0; i < MAX_BOOKS; i++) {
        antreanBuku[i].front = antreanBuku[i].rear = NULL;
    }
}

Buku* cariBukuBerdasarkanId(int id) {
    Buku* temp = head;
    while (temp) {
        if (temp->idBuku == id) return temp;
        temp = temp->next;
    }
    return NULL;
}

int MengecekPengguna(int id, char* nama) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (daftarPengguna[i].idPengguna == id && bandingString(daftarPengguna[i].nama, nama))
            return i;
    }
    return -1;
}

int CariIndexBuku(int id) {
    return id - 101;
}

void enqueue(Queue &q, int userId) {
    QueueNode* node = new QueueNode{userId, NULL};
    if (!q.rear) q.front = q.rear = node;
    else {
        q.rear->next = node;
        q.rear = node;
    }
}

int dequeue(Queue &q) {
    if (!q.front) return -1;
    QueueNode* temp = q.front;
    int id = temp->userId;
    q.front = q.front->next;
    if (!q.front) q.rear = NULL;
    delete temp;
    return id;
}

void tampilkanBuku() {
    int pilihan;
    const char* kategoriLabels[3] = {"Informatika", "Sains", "Sejarah"};

    cout << "=== PILIH KATEGORI BUKU ===\n";
    for (int i = 0; i < 3; i++) {
        cout << i + 1 << ". " << kategoriLabels[i] << "\n";
    }
    cout << "Pilihan Anda: ";
    cin >> pilihan;
    system("cls");

    if (pilihan < 1 || pilihan > 3) {
        cout << "Pilihan tidak valid.\n";
        system("pause");
        system("cls");
        return;
    }

    cout << "=== DAFTAR BUKU KATEGORI: " << kategoriLabels[pilihan - 1] << " ===\n\n";

    cout << left
         << setw(5) << "ID"
         << setw(30) << "Judul"
         << setw(20) << "Penulis"
         << setw(12) << "Status"
         << setw(10) << "Rak"
         << "\n";
    cout << string(77, '-') << "\n";

    Buku* temp = head;
    while (temp) {
        if (temp->barisRak == pilihan) {
            cout << left
                 << setw(5) << temp->idBuku
                 << setw(30) << temp->judul
                 << setw(20) << temp->penulis
                 << setw(12) << (temp->SedangDipinjam ? "Dipinjam" : "Tersedia")
                 << temp->barisRak << "-" << temp->kolomRak << "\n";
        }
        temp = temp->next;
    }

    cout << string(77, '-') << "\n";
    system("pause");
    system("cls");
}

void urutkanBuku() {
    for (Buku* i = head; i != NULL; i = i->next) {
        for (Buku* j = i->next; j != NULL; j = j->next) {
            int k = 0;
            while (i->judul[k] == j->judul[k] && i->judul[k] != '\0') k++;
            if (i->judul[k] > j->judul[k]) {
                swap(i->idBuku, j->idBuku);
                swap(i->barisRak, j->barisRak);
                swap(i->kolomRak, j->kolomRak);
                swap(i->SedangDipinjam, j->SedangDipinjam);

                char tempJudul[50], tempPenulis[30];
                salinString(tempJudul, i->judul);
                salinString(i->judul, j->judul);
                salinString(j->judul, tempJudul);

                salinString(tempPenulis, i->penulis);
                salinString(i->penulis, j->penulis);
                salinString(j->penulis, tempPenulis);
            }
        }
    }
}

void tampilkanSemuaBukuTerurut() {
    cout << "=== DAFTAR BUKU (URUTAN ABJAD) ===\n\n";

    cout << left
         << setw(5) << "ID"
         << setw(30) << "Judul"
         << setw(20) << "Penulis"
         << setw(12) << "Status"
         << setw(10) << "Rak"
         << "\n";
    cout << string(77, '-') << "\n";

    Buku* temp = head;
    while (temp) {
        cout << left
             << setw(5) << temp->idBuku
             << setw(30) << temp->judul
             << setw(20) << temp->penulis
             << setw(12) << (temp->SedangDipinjam ? "Dipinjam" : "Tersedia")
             << temp->barisRak << "-" << temp->kolomRak << "\n";
        temp = temp->next;
    }

    cout << string(77, '-') << "\n";
    system("pause");
    system("cls");
}

void cariBukuBerdasarkanJudul(const char* kataKunci) {
    cout << "=== HASIL PENCARIAN ===\n";
    bool ditemukan = false;
    Buku* temp = head;
    while (temp) {
        if (mengandungSubstring(temp->judul, kataKunci)) {
            cout << "ID: " << temp->idBuku << "\n"
                 << "Judul : " << temp->judul << "\n"
                 << "Penulis : " << temp->penulis << "\n"
                 << "Status : " << (temp->SedangDipinjam ? "Dipinjam" : "Tersedia") << "\n"
                 << "Posisi Rak : " << temp->barisRak << "-" << temp->kolomRak << "\n\n";
            ditemukan = true;
        }
        temp = temp->next;
    }
    if (!ditemukan) cout << "Buku tidak ditemukan.\n";
    system("pause");
    system("cls");
}

void pinjamBuku() {
    int idPengguna, idBuku;
    char namaPengguna[30];
    cout << "Masukkan ID Pengguna: "; cin >> idPengguna;
    cout << "Masukkan Nama Pengguna: "; cin.ignore(); cin.getline(namaPengguna,30);
    int idx = MengecekPengguna(idPengguna, namaPengguna);
    if (idx == -1) {
        cout << "Akun tidak ditemukan.\n";
        system("pause");
        system("cls");
        return;
    }
    cout << "Masukkan ID Buku: "; cin >> idBuku;
    Buku* buku = cariBukuBerdasarkanId(idBuku);
    if (!buku) {
        cout << "Buku tidak ditemukan.\n";
        system("pause");
        system("cls");
        return;
    }

    int indeksBuku = CariIndexBuku(idBuku);
    if (!buku->SedangDipinjam) {
        buku->SedangDipinjam = true;
        if(daftarPengguna[idx].jumlahRiwayat < MAX_HISTORY)
            daftarPengguna[idx].riwayat[daftarPengguna[idx].jumlahRiwayat++].idBuku = idBuku;
        cout << "Berhasil meminjam buku: " << buku->judul << endl;
    } else {
        enqueue(antreanBuku[indeksBuku], idPengguna);
        cout << "Buku sedang dipinjam. Anda masuk dalam antrean.\n";
    }
    system("pause");
    system("cls");
}

void kembalikanBuku() {
    int idPengguna, idBuku;
    char namaPengguna[30];
    cout << "Masukkan ID Pengguna: "; cin >> idPengguna;
    cout << "Masukkan Nama Pengguna: "; cin.ignore(); cin.getline(namaPengguna,30);
    int idx = MengecekPengguna(idPengguna, namaPengguna);
    if (idx == -1) {
        cout << "Akun tidak ditemukan.\n";
        system("pause");
        system("cls");
        return;
    }
    cout << "Masukkan ID Buku: "; cin >> idBuku;
    Buku* buku = cariBukuBerdasarkanId(idBuku);
    if (!buku || !buku->SedangDipinjam) {
        cout << "Buku tidak dipinjam.\n";
        system("pause");
        system("cls");
        return;
    }

    int indeksBuku = CariIndexBuku(idBuku);
    int penggunaSelanjutnya = dequeue(antreanBuku[indeksBuku]);
    if (penggunaSelanjutnya != -1) {
        buku->SedangDipinjam = true;
        for (int i = 0; i < MAX_USERS; i++) {
            if (daftarPengguna[i].idPengguna == penggunaSelanjutnya) {
                if(daftarPengguna[i].jumlahRiwayat < MAX_HISTORY)
                    daftarPengguna[i].riwayat[daftarPengguna[i].jumlahRiwayat++].idBuku = idBuku;
                break;
            }
        }
        cout << "Buku langsung diberikan ke pengguna dalam antrean (ID: " << penggunaSelanjutnya << ")\n";
    } else {
        buku->SedangDipinjam = false;
        cout << "Buku berhasil dikembalikan.\n";
    }
    system("pause");
    system("cls");
}

void lihatRiwayat() {
    int idPengguna;
    char namaPengguna[30];
    cout << "Masukkan ID Pengguna: "; cin >> idPengguna;
    cout << "Masukkan Nama Pengguna: "; cin.ignore(); cin.getline(namaPengguna,30);
    int idx = MengecekPengguna(idPengguna, namaPengguna);
    if (idx == -1) {
        cout << "Akun tidak ditemukan.\n";
        system("pause");
        system("cls");
        return;
    }
    cout << "=== Riwayat Pinjaman Pengguna " << daftarPengguna[idx].nama << " ===\n";
    if(daftarPengguna[idx].jumlahRiwayat == 0) {
        cout << "Belum ada riwayat pinjaman.\n";
    }
    for (int i = 0; i < daftarPengguna[idx].jumlahRiwayat; i++) {
        Buku* buku = cariBukuBerdasarkanId(daftarPengguna[idx].riwayat[i].idBuku);
        if (buku)
            cout << "- " << buku->judul << " (ID: " << buku->idBuku << ")\n";
    }
    system("pause");
    system("cls");
}

int main() {
    inisialisasiBuku();
    int choice;
    do {
        cout << "=== MENU PERPUSTAKAAN ===\n";
        cout << "1. Lihat Semua Buku (Kategori)\n";
        cout << "2. Pinjam Buku\n";
        cout << "3. Kembalikan Buku\n";
        cout << "4. Lihat Riwayat Pinjaman\n";
        cout << "5. Urutkan Buku Berdasarkan Judul\n";
        cout << "6. Cari Buku Berdasarkan Judul\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: "; cin >> choice;
        system("cls");

        switch (choice) {
            case 1: tampilkanBuku(); break;
            case 2: pinjamBuku(); break;
            case 3: kembalikanBuku(); break;
            case 4: lihatRiwayat(); break;
            case 5:
                urutkanBuku();
                tampilkanSemuaBukuTerurut();
                break;
            case 6: {
                char kataKunci[50];
                cout << "Masukkan Judul yang Dicari: "; cin.ignore(); cin.getline(kataKunci, 50);
                cariBukuBerdasarkanJudul(kataKunci);
                break;
            }
            case 0:
                cout << "Terima kasih telah menggunakan program perpustakaan!\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
                system("pause");
                system("cls");
        }
    } while (choice != 0);

    while(head) {
        Buku* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}