#include <iostream>
#include <ctime>
#include <vector>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

// Konstanta
const int MAKS_BUKU = 100;
const int MAKS_PENGGUNA = 50;

// Enum Kategori Buku
enum class KategoriBuku {
    EKONOMI,
    ILMU_KOMPUTER,
    AGAMA,
    SOSIAL,
    PEMERINTAHAN,
    FILSAFAT,
    LAIN_LAIN
};

// Enum Tipe Buku
enum class TipeBuku {
    FISIK,
    ONLINE
};

// Struct Waktu
struct Waktu {
    int hari, bulan, tahun;
    int jam, menit;
    
    Waktu() : hari(0), bulan(0), tahun(0), jam(0), menit(0) {}
    
    void setWaktuSekarang() {
        time_t sekarang = time(0);
        tm* waktuLokal = localtime(&sekarang);
        hari = waktuLokal->tm_mday;
        bulan = waktuLokal->tm_mon + 1;
        tahun = waktuLokal->tm_year + 1900;
        jam = waktuLokal->tm_hour;
        menit = waktuLokal->tm_min;
    }
    
    string toString() const {
        return to_string(hari) + "/" + to_string(bulan) + "/" + to_string(tahun) + 
               " " + to_string(jam) + ":" + (menit < 10 ? "0" : "") + to_string(menit);
    }
};

// Struct Buku
struct Buku {
    int id;
    string judul;
    string penulis;
    KategoriBuku kategori;
    TipeBuku tipe;
    bool dipinjam;
    int idPenggunaPeminjam;
    string tempatBuku;
    Waktu waktuPinjam;
    
    Buku() : id(0), judul(""), penulis(""), kategori(KategoriBuku::LAIN_LAIN), 
             tipe(TipeBuku::FISIK), dipinjam(false), idPenggunaPeminjam(-1), tempatBuku("") {}
};

// Struct Pengguna
struct Pengguna {
    int idPengguna;
    string nama;
    
    Pengguna() : idPengguna(0), nama("") {}
};

// Node untuk Linked List Riwayat
struct NodeRiwayat {
    int idBuku;
    int idPengguna;
    string judulBuku;
    string namaPengguna;
    Waktu waktuPinjam;
    Waktu waktuKembali;
    NodeRiwayat* next;
    
    NodeRiwayat() : idBuku(0), idPengguna(0), judulBuku(""), namaPengguna(""), next(nullptr) {}
};

// Class Riwayat Peminjaman
class RiwayatPeminjaman {
private:
    NodeRiwayat* head;
    
public:
    RiwayatPeminjaman() : head(nullptr) {}
    
    void tambahRiwayat(int idBuku, int idPengguna, const string& judulBuku, 
                      const string& namaPengguna, const Waktu& waktuPinjam) {
        NodeRiwayat* nodeBaru = new NodeRiwayat();
        nodeBaru->idBuku = idBuku;
        nodeBaru->idPengguna = idPengguna;
        nodeBaru->judulBuku = judulBuku;
        nodeBaru->namaPengguna = namaPengguna;
        nodeBaru->waktuPinjam = waktuPinjam;
        nodeBaru->next = head;
        head = nodeBaru;
    }
    
    void selesaiPeminjaman(int idBuku, const Waktu& waktuKembali) {
        NodeRiwayat* current = head;
        while (current != nullptr) {
            if (current->idBuku == idBuku && current->waktuKembali.hari == 0) {
                current->waktuKembali = waktuKembali;
                break;
            }
            current = current->next;
        }
    }
    
    void tampilkanRiwayat() {
        if (head == nullptr) {
            cout << "Tidak ada riwayat peminjaman." << endl;
            return;
        }
        
        cout << "\n--- Riwayat Peminjaman ---" << endl;
        cout << "ID Buku\tJudul\t\tPeminjam\tWaktu Pinjam\t\tWaktu Kembali" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        
        NodeRiwayat* current = head;
        while (current != nullptr) {
            cout << current->idBuku << "\t" << current->judulBuku << "\t\t" 
                 << current->namaPengguna << "\t" << current->waktuPinjam.toString() << "\t";
            if (current->waktuKembali.hari != 0) {
                cout << current->waktuKembali.toString();
            } else {
                cout << "Belum dikembalikan";
            }
            cout << endl;
            current = current->next;
        }
        cout << "--------------------------------------------------------------------------------" << endl;
    }
    
    ~RiwayatPeminjaman() {
        while (head != nullptr) {
            NodeRiwayat* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Node untuk Queue Antrian
struct NodeQueue {
    int idPengguna;
    int idBuku;
    string namaPengguna;
    string judulBuku;
    Waktu waktuAntri;
    NodeQueue* next;
    
    NodeQueue() : idPengguna(0), idBuku(0), namaPengguna(""), judulBuku(""), next(nullptr) {}
};

// Class Antrian Peminjaman
class AntrianPeminjaman {
private:
    NodeQueue* front;
    NodeQueue* rear;
    int ukuran;
    
public:
    AntrianPeminjaman() : front(nullptr), rear(nullptr), ukuran(0) {}
    
    void enqueue(int idPengguna, int idBuku, const string& namaPengguna, const string& judulBuku) {
        NodeQueue* nodeBaru = new NodeQueue();
        nodeBaru->idPengguna = idPengguna;
        nodeBaru->idBuku = idBuku;
        nodeBaru->namaPengguna = namaPengguna;
        nodeBaru->judulBuku = judulBuku;
        nodeBaru->waktuAntri.setWaktuSekarang();
        
        if (rear == nullptr) {
            front = rear = nodeBaru;
        } else {
            rear->next = nodeBaru;
            rear = nodeBaru;
        }
        ukuran++;
        cout << "Pengguna " << namaPengguna << " ditambahkan ke antrian untuk buku '" << judulBuku << "'" << endl;
    }
    
    void dequeue() {
        if (front == nullptr) {
            cout << "Antrian kosong." << endl;
            return;
        }
        
        NodeQueue* temp = front;
        cout << "Peminjaman untuk " << temp->namaPengguna << " (buku: " << temp->judulBuku << ") diproses." << endl;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        ukuran--;
    }
    
    void tampilkanAntrian() {
        if (front == nullptr) {
            cout << "Antrian peminjaman kosong." << endl;
            return;
        }
        
        cout << "\n--- Antrian Peminjaman ---" << endl;
        cout << "Posisi\tNama Pengguna\tJudul Buku\t\tWaktu Antri" << endl;
        cout << "---------------------------------------------------------------" << endl;
        
        NodeQueue* current = front;
        int posisi = 1;
        while (current != nullptr) {
            cout << posisi << "\t" << current->namaPengguna << "\t\t" 
                 << current->judulBuku << "\t\t" << current->waktuAntri.toString() << endl;
            current = current->next;
            posisi++;
        }
        cout << "---------------------------------------------------------------" << endl;
    }
    
    bool kosong() {
        return front == nullptr;
    }
    
    ~AntrianPeminjaman() {
        while (front != nullptr) {
            NodeQueue* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

// Node untuk Binary Search Tree
struct NodeBST {
    Buku* buku;
    NodeBST* kiri;
    NodeBST* kanan;
    
    NodeBST(Buku* b) : buku(b), kiri(nullptr), kanan(nullptr) {}
};

// Class Binary Search Tree
class BST {
private:
    NodeBST* root;
    
    NodeBST* insert(NodeBST* node, Buku* buku) {
        if (node == nullptr) {
            return new NodeBST(buku);
        }
        
        if (buku->id < node->buku->id) {
            node->kiri = insert(node->kiri, buku);
        } else if (buku->id > node->buku->id) {
            node->kanan = insert(node->kanan, buku);
        }
        
        return node;
    }
    
    NodeBST* cari(NodeBST* node, int id) {
        if (node == nullptr || node->buku->id == id) {
            return node;
        }
        
        if (id < node->buku->id) {
            return cari(node->kiri, id);
        }
        
        return cari(node->kanan, id);
    }
    
    void inorder(NodeBST* node, vector<Buku*>& hasil) {
        if (node != nullptr) {
            inorder(node->kiri, hasil);
            hasil.push_back(node->buku);
            inorder(node->kanan, hasil);
        }
    }
    
public:
    BST() : root(nullptr) {}
    
    void tambahBuku(Buku* buku) {
        root = insert(root, buku);
    }
    
    Buku* cariBuku(int id) {
        NodeBST* node = cari(root, id);
        return node ? node->buku : nullptr;
    }
    
    vector<Buku*> getDaftarBuku() {
        vector<Buku*> hasil;
        inorder(root, hasil);
        return hasil;
    }
};

// Variabel Global
Buku daftarBuku[MAKS_BUKU];
Pengguna daftarPengguna[MAKS_PENGGUNA];
int jumlahBuku = 0;
int jumlahPengguna = 0;
int idPenggunaBerikutnya = 1;

BST indeksBuku;
RiwayatPeminjaman riwayat;
AntrianPeminjaman antrian;

// Deklarasi Fungsi
void initializeBuku();
void tampilkanMenu();
void tampilkanBukuFisik();
void tampilkanBukuOnline();
void cekDetailBuku();
void pinjamBuku();
void kembalikanBuku();
void daftarPengguna();
void tampilkanSemuaPengguna();
void tampilkanRiwayat();
void tampilkanAntrian();
void prosesAntrian();
void urutkanBuku();
void cariBuku();
int cariIndeksPenggunaBerdasarkanId(int idPengguna);
void bersihkanBufferInput();
string kategoriToString(KategoriBuku kategori);
string tipeToString(TipeBuku tipe);

// Implementasi Fungsi
void initializeBuku() {
    daftarBuku[0] = {1, "Mikroekonomi Dasar", "Robert S. Pindyck", KategoriBuku::EKONOMI, TipeBuku::FISIK, false, -1, "Rak A-1, Lantai 2"};
    daftarBuku[1] = {2, "Algoritma dan Struktur Data", "Thomas H. Cormen", KategoriBuku::ILMU_KOMPUTER, TipeBuku::FISIK, false, -1, "Rak B-3, Lantai 1"};
    daftarBuku[2] = {3, "Tafsir Al-Quran Al-Misbah", "M. Quraish Shihab", KategoriBuku::AGAMA, TipeBuku::FISIK, false, -1, "Rak C-2, Lantai 3"};
    daftarBuku[3] = {4, "Sosiologi Modern", "Anthony Giddens", KategoriBuku::SOSIAL, TipeBuku::FISIK, false, -1, "Rak D-1, Lantai 2"};
    daftarBuku[4] = {5, "Sistem Politik Indonesia", "Miriam Budiardjo", KategoriBuku::PEMERINTAHAN, TipeBuku::FISIK, false, -1, "Rak E-4, Lantai 1"};
    daftarBuku[5] = {6, "Makroekonomi Global", "Paul Krugman", KategoriBuku::EKONOMI, TipeBuku::ONLINE, false, -1, "https://library.digital/macro-economics"};
    daftarBuku[6] = {7, "Machine Learning Fundamentals", "Andrew Ng", KategoriBuku::ILMU_KOMPUTER, TipeBuku::ONLINE, false, -1, "https://library.digital/ml-fundamentals"};
    daftarBuku[7] = {8, "Hadits Shahih Bukhari", "Imam Bukhari", KategoriBuku::AGAMA, TipeBuku::ONLINE, false, -1, "https://library.digital/bukhari-hadits"};
    daftarBuku[8] = {9, "Psikologi Sosial", "David G. Myers", KategoriBuku::SOSIAL, TipeBuku::ONLINE, false, -1, "https://library.digital/social-psychology"};
    daftarBuku[9] = {10, "Filsafat Kontemporer", "Bertrand Russell", KategoriBuku::FILSAFAT, TipeBuku::ONLINE, false, -1, "https://library.digital/contemporary-philosophy"};
    
    for (int i = 0; i < 10; i++) {
        indeksBuku.tambahBuku(&daftarBuku[i]);
    }
    
    jumlahBuku = 10;
}

void tampilkanMenu() {
    cout << "\n=== SISTEM PERPUSTAKAAN DIGITAL ===" << endl;
    cout << "1. Lihat Buku Fisik (Offline)" << endl;
    cout << "2. Lihat Buku Online" << endl;
    cout << "3. Cek Detail Buku" << endl;
    cout << "4. Pinjam Buku" << endl;
    cout << "5. Kembalikan Buku" << endl;
    cout << "6. Daftar Pengguna Baru" << endl;
    cout << "7. Lihat Semua Pengguna" << endl;
    cout << "8. Lihat Riwayat Peminjaman" << endl;
    cout << "9. Lihat Antrian Peminjaman" << endl;
    cout << "10. Proses Antrian" << endl;
    cout << "11. Urutkan Buku (berdasarkan Judul)" << endl;
    cout << "12. Cari Buku" << endl;
    cout << "0. Keluar" << endl;
    cout << "====================================" << endl;
}

void tampilkanBukuFisik() {
    cout << "\n--- DAFTAR BUKU FISIK ---" << endl;
    cout << "ID\tJudul\t\t\t\tPenulis\t\t\tStatus\t\tKategori\t\tTempat Buku" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    
    for (int i = 0; i < jumlahBuku; i++) {
        if (daftarBuku[i].tipe == TipeBuku::FISIK) {
            cout << daftarBuku[i].id << "\t";
            
            // Format judul dengan padding yang tepat
            string judul = daftarBuku[i].judul;
            if (judul.length() > 23) {
                judul = judul.substr(0, 20) + "...";
            }
            cout << judul;
            for (int j = judul.length(); j < 24; j++) cout << " ";
            
            // Format penulis
            string penulis = daftarBuku[i].penulis;
            if (penulis.length() > 19) {
                penulis = penulis.substr(0, 16) + "...";
            }
            cout << penulis;
            for (int j = penulis.length(); j < 20; j++) cout << " ";
            
            // Status buku
            if (daftarBuku[i].dipinjam) {
                cout << "Dipinjam\t";
            } else {
                cout << "Tersedia\t";
            }
            
            cout << kategoriToString(daftarBuku[i].kategori) << "\t\t" << daftarBuku[i].tempatBuku << endl;
        }
    }
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
}

void tampilkanBukuOnline() {
    cout << "\n--- DAFTAR BUKU ONLINE ---" << endl;
    cout << "ID\tJudul\t\t\t\tPenulis\t\t\tStatus\t\tKategori\t\tURL/Link" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    
    for (int i = 0; i < jumlahBuku; i++) {
        if (daftarBuku[i].tipe == TipeBuku::ONLINE) {
            cout << daftarBuku[i].id << "\t";
            
            // Format judul dengan padding yang tepat
            string judul = daftarBuku[i].judul;
            if (judul.length() > 23) {
                judul = judul.substr(0, 20) + "...";
            }
            cout << judul;
            for (int j = judul.length(); j < 24; j++) cout << " ";
            
            // Format penulis
            string penulis = daftarBuku[i].penulis;
            if (penulis.length() > 19) {
                penulis = penulis.substr(0, 16) + "...";
            }
            cout << penulis;
            for (int j = penulis.length(); j < 20; j++) cout << " ";
            
            // Status buku
            if (daftarBuku[i].dipinjam) {
                cout << "Dipinjam\t";
            } else {
                cout << "Tersedia\t";
            }
            
            cout << kategoriToString(daftarBuku[i].kategori) << "\t\t" << daftarBuku[i].tempatBuku << endl;
        }
    }
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
}

void cekDetailBuku() {
    int idBuku;
    cout << "Masukkan ID Buku: ";
    cin >> idBuku;
    bersihkanBufferInput();
    
    Buku* buku = indeksBuku.cariBuku(idBuku);
    
    if (buku == nullptr) {
        cout << "Buku dengan ID " << idBuku << " tidak ditemukan." << endl;
        return;
    }
    
    cout << "\n--- DETAIL BUKU ---" << endl;
    cout << "ID: " << buku->id << endl;
    cout << "Judul: " << buku->judul << endl;
    cout << "Penulis: " << buku->penulis << endl;
    cout << "Kategori: " << kategoriToString(buku->kategori) << endl;
    cout << "Tipe: " << tipeToString(buku->tipe) << endl;
    cout << "Tempat: " << buku->tempatBuku << endl;
    cout << "Status: " << (buku->dipinjam ? "Dipinjam" : "Tersedia") << endl;
    
    if (buku->dipinjam) {
        cout << "Waktu Dipinjam: " << buku->waktuPinjam.toString() << endl;
        cout << "ID Peminjam: " << buku->idPenggunaPeminjam << endl;
    }
    cout << "-------------------" << endl;
}

void pinjamBuku() {
    if (jumlahPengguna == 0) {
        cout << "Belum ada pengguna terdaftar. Silakan daftar terlebih dahulu." << endl;
        return;
    }
    
    int idBuku, idPengguna;
    cout << "Masukkan ID Buku: ";
    cin >> idBuku;
    cout << "Masukkan ID Pengguna: ";
    cin >> idPengguna;
    bersihkanBufferInput();
    
    Buku* buku = indeksBuku.cariBuku(idBuku);
    if (buku == nullptr) {
        cout << "Buku tidak ditemukan." << endl;
        return;
    }
    
    int indeksPengguna = cariIndeksPenggunaBerdasarkanId(idPengguna);
    if (indeksPengguna == -1) {
        cout << "Pengguna tidak ditemukan." << endl;
        return;
    }
    
    if (buku->dipinjam) {
        antrian.enqueue(idPengguna, idBuku, daftarPengguna[indeksPengguna].nama, buku->judul);
    } else {
        buku->dipinjam = true;
        buku->idPenggunaPeminjam = idPengguna;
        buku->waktuPinjam.setWaktuSekarang();
        
        riwayat.tambahRiwayat(idBuku, idPengguna, buku->judul, 
                             daftarPengguna[indeksPengguna].nama, buku->waktuPinjam);
        
        cout << "Buku '" << buku->judul << "' berhasil dipinjam oleh " 
             << daftarPengguna[indeksPengguna].nama << endl;
    }
}

void kembalikanBuku() {
    int idBuku;
    cout << "Masukkan ID Buku yang akan dikembalikan: ";
    cin >> idBuku;
    bersihkanBufferInput();
    
    Buku* buku = indeksBuku.cariBuku(idBuku);
    if (buku == nullptr) {
        cout << "Buku tidak ditemukan." << endl;
        return;
    }
    
    if (!buku->dipinjam) {
        cout << "Buku tidak sedang dipinjam." << endl;
        return;
    }
    
    Waktu waktuKembali;
    waktuKembali.setWaktuSekarang();
    
    riwayat.selesaiPeminjaman(idBuku, waktuKembali);
    
    cout << "Buku '" << buku->judul << "' berhasil dikembalikan." << endl;
    
    buku->dipinjam = false;
    buku->idPenggunaPeminjam = -1;
    
    if (!antrian.kosong()) {
        cout << "Memproses antrian..." << endl;
        antrian.dequeue();
    }
}

void daftarPengguna() {
    if (jumlahPengguna >= MAKS_PENGGUNA) {
        cout << "Batas maksimum pengguna tercapai." << endl;
        return;
    }
    
    cout << "Masukkan nama pengguna: ";
    string nama;
    getline(cin, nama);
    
    daftarPengguna[jumlahPengguna].idPengguna = idPenggunaBerikutnya++;
    daftarPengguna[jumlahPengguna].nama = nama;
    
    cout << "Pengguna '" << nama << "' berhasil didaftarkan dengan ID: " 
         << daftarPengguna[jumlahPengguna].idPengguna << endl;
    
    jumlahPengguna++;
}

void tampilkanSemuaPengguna() {
    if (jumlahPengguna == 0) {
        cout << "Belum ada pengguna terdaftar." << endl;
        return;
    }
    
    cout << "\n--- DAFTAR PENGGUNA ---" << endl;
    cout << "ID\tNama" << endl;
    cout << "----------------------" << endl;
    
    for (int i = 0; i < jumlahPengguna; i++) {
        cout << daftarPengguna[i].idPengguna << "\t" << daftarPengguna[i].nama << endl;
    }
    cout << "----------------------" << endl;
}

void tampilkanRiwayat() {
    riwayat.tampilkanRiwayat();
}

void tampilkanAntrian() {
    antrian.tampilkanAntrian();
}

void prosesAntrian() {
    if (!antrian.kosong()) {
        antrian.dequeue();
    } else {
        cout << "Tidak ada antrian untuk diproses." << endl;
    }
}

void urutkanBuku() {
    // Menggunakan Selection Sort untuk stabilitas
    for (int i = 0; i < jumlahBuku - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < jumlahBuku; j++) {
            if (daftarBuku[j].judul < daftarBuku[minIdx].judul) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(daftarBuku[i], daftarBuku[minIdx]);
        }
    }
    
    // Rebuild BST setelah sorting
    BST newBST;
    for (int i = 0; i < jumlahBuku; i++) {
        newBST.tambahBuku(&daftarBuku[i]);
    }
    indeksBuku = newBST;
    
    cout << "Buku berhasil diurutkan berdasarkan judul secara alfabetis." << endl;
}

void cariBuku() {
    string keyword;
    cout << "Masukkan kata kunci pencarian (judul): ";
    getline(cin, keyword);
    
    // Konversi ke lowercase dengan cara yang aman
    transform(keyword.begin(), keyword.end(), keyword.begin(), 
              [](unsigned char c) { return tolower(c); });
    
    cout << "\n--- HASIL PENCARIAN ---" << endl;
    bool ditemukan = false;
    
    for (int i = 0; i < jumlahBuku; i++) {
        string judulLower = daftarBuku[i].judul;
        // Konversi ke lowercase dengan cara yang aman
        transform(judulLower.begin(), judulLower.end(), judulLower.begin(),
                  [](unsigned char c) { return tolower(c); });
        
        if (judulLower.find(keyword) != string::npos) {
            cout << "ID: " << daftarBuku[i].id << " | " 
                 << daftarBuku[i].judul << " (" << tipeToString(daftarBuku[i].tipe) << ")" 
                 << " - " << daftarBuku[i].penulis << endl;
            ditemukan = true;
        }
    }
    
    if (!ditemukan) {
        cout << "Tidak ada buku yang ditemukan dengan kata kunci '" << keyword << "'." << endl;
    }
    cout << "----------------------" << endl;
}

int cariIndeksPenggunaBerdasarkanId(int idPengguna) {
    for (int i = 0; i < jumlahPengguna; i++) {
        if (daftarPengguna[i].idPengguna == idPengguna) {
            return i;
        }
    }
    return -1;
}

void bersihkanBufferInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string kategoriToString(KategoriBuku kategori) {
    switch (kategori) {
        case KategoriBuku::EKONOMI: return "Ekonomi";
        case KategoriBuku::ILMU_KOMPUTER: return "Ilmu Komputer";
        case KategoriBuku::AGAMA: return "Agama";
        case KategoriBuku::SOSIAL: return "Sosial";
        case KategoriBuku::PEMERINTAHAN: return "Pemerintahan";
        case KategoriBuku::FILSAFAT: return "Filsafat";
        case KategoriBuku::LAIN_LAIN: return "Lain-lain";
        default: return "Tidak Diketahui";
    }
}

string tipeToString(TipeBuku tipe) {
    return (tipe == TipeBuku::FISIK) ? "Fisik" : "Online";
}

// Fungsi Utama
int main() {
    initializeBuku();
    
    int pilihan;
    do {
        tampilkanMenu();
        cout << "Pilih menu: ";
        cin >> pilihan;
        bersihkanBufferInput();
        
        switch (pilihan) {
            case 1: tampilkanBukuFisik(); break;
            case 2: tampilkanBukuOnline(); break;
            case 3: cekDetailBuku(); break;
            case 4: pinjamBuku(); break;
            case 5: kembalikanBuku(); break;
            case 6: daftarPengguna(); break;
            case 7: tampilkanSemuaPengguna(); break;
            case 8: tampilkanRiwayat(); break;
            case 9: tampilkanAntrian(); break;
            case 10: prosesAntrian(); break;
            case 11: urutkanBuku(); break;
            case 12: cariBuku(); break;
            case 0: cout << "Terima kasih telah menggunakan sistem perpustakaan!" << endl; break;
            default: cout << "Pilihan tidak valid." << endl;
        }
        
        if (pilihan != 0) {
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.get();
        }
        
    } while (pilihan != 0);
    
    return 0;
}
