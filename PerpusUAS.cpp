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
    
    // Mencegah shallow copy
    RiwayatPeminjaman(const RiwayatPeminjaman&) = delete;
    RiwayatPeminjaman& operator=(const RiwayatPeminjaman&) = delete;
    
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
    
    // Mencegah shallow copy
    AntrianPeminjaman(const AntrianPeminjaman&) = delete;
    AntrianPeminjaman& operator=(const AntrianPeminjaman&) = delete;
    
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
    
    NodeQueue* dequeue() {
        if (front == nullptr) {
            cout << "Antrian kosong." << endl;
            return nullptr;
        }
        
        NodeQueue* temp = front;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        ukuran--;
        temp->next = nullptr; // Memutuskan hubungan sebelum return
        return temp;
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
    
    bool kosong() const {
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
    
    ~NodeBST() {
        // Tidak menghapus buku karena buku dimiliki oleh array daftarBuku
    }
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
    
    NodeBST* cari(NodeBST* node, int id) const {
        if (node == nullptr || node->buku->id == id) {
            return node;
        }
        
        if (id < node->buku->id) {
            return cari(node->kiri, id);
        }
        
        return cari(node->kanan, id);
    }
    
    void inorder(NodeBST* node, vector<Buku*>& hasil) const {
        if (node != nullptr) {
            inorder(node->kiri, hasil);
            hasil.push_back(node->buku);
            inorder(node->kanan, hasil);
        }
    }
    
    void hapusSemua(NodeBST* node) {
        if (node != nullptr) {
            hapusSemua(node->kiri);
            hapusSemua(node->kanan);
            delete node;
        }
    }
    
public:
    BST() : root(nullptr) {}
    
    // Copy constructor
    BST(const BST& other) : root(nullptr) {
        // Implementasi copy constructor jika diperlukan
    }
    
    // Assignment operator
    BST& operator=(const BST& other) {
        if (this != &other) {
            hapusSemua(root);
            root = nullptr;
            
            vector<Buku*> bukuBuku;
            other.getDaftarBuku(bukuBuku);
            for (Buku* buku : bukuBuku) {
                tambahBuku(buku);
            }
        }
        return *this;
    }
    
    ~BST() {
        hapusSemua(root);
    }
    
    void tambahBuku(Buku* buku) {
        root = insert(root, buku);
    }
    
    Buku* cariBuku(int id) const {
        NodeBST* node = cari(root, id);
        return node ? node->buku : nullptr;
    }
    
    void getDaftarBuku(vector<Buku*>& hasil) const {
        inorder(root, hasil);
    }
    
    void clear() {
        hapusSemua(root);
        root = nullptr;
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
void daftarPenggunaBaru();
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
    // Inisialisasi buku fisik
    daftarBuku[0].id = 1;
    daftarBuku[0].judul = "Mikroekonomi Dasar";
    daftarBuku[0].penulis = "Robert S. Pindyck";
    daftarBuku[0].kategori = KategoriBuku::EKONOMI;
    daftarBuku[0].tipe = TipeBuku::FISIK;
    daftarBuku[0].dipinjam = false;
    daftarBuku[0].idPenggunaPeminjam = -1;
    daftarBuku[0].tempatBuku = "Rak A-1, Lantai 2";

    daftarBuku[1].id = 2;
    daftarBuku[1].judul = "Algoritma dan Struktur Data";
    daftarBuku[1].penulis = "Thomas H. Cormen";
    daftarBuku[1].kategori = KategoriBuku::ILMU_KOMPUTER;
    daftarBuku[1].tipe = TipeBuku::FISIK;
    daftarBuku[1].dipinjam = false;
    daftarBuku[1].idPenggunaPeminjam = -1;
    daftarBuku[1].tempatBuku = "Rak B-3, Lantai 1";

    daftarBuku[2].id = 3;
    daftarBuku[2].judul = "Tafsir Al-Quran Al-Misbah";
    daftarBuku[2].penulis = "M. Quraish Shihab";
    daftarBuku[2].kategori = KategoriBuku::AGAMA;
    daftarBuku[2].tipe = TipeBuku::FISIK;
    daftarBuku[2].dipinjam = false;
    daftarBuku[2].idPenggunaPeminjam = -1;
    daftarBuku[2].tempatBuku = "Rak C-2, Lantai 3";

    daftarBuku[3].id = 4;
    daftarBuku[3].judul = "Sosiologi Modern";
    daftarBuku[3].penulis = "Anthony Giddens";
    daftarBuku[3].kategori = KategoriBuku::SOSIAL;
    daftarBuku[3].tipe = TipeBuku::FISIK;
    daftarBuku[3].dipinjam = false;
    daftarBuku[3].idPenggunaPeminjam = -1;
    daftarBuku[3].tempatBuku = "Rak D-1, Lantai 2";

    daftarBuku[4].id = 5;
    daftarBuku[4].judul = "Sistem Politik Indonesia";
    daftarBuku[4].penulis = "Miriam Budiardjo";
    daftarBuku[4].kategori = KategoriBuku::PEMERINTAHAN;
    daftarBuku[4].tipe = TipeBuku::FISIK;
    daftarBuku[4].dipinjam = false;
    daftarBuku[4].idPenggunaPeminjam = -1;
    daftarBuku[4].tempatBuku = "Rak E-4, Lantai 1";

    // Inisialisasi buku online
    daftarBuku[5].id = 6;
    daftarBuku[5].judul = "Makroekonomi Global";
    daftarBuku[5].penulis = "Paul Krugman";
    daftarBuku[5].kategori = KategoriBuku::EKONOMI;
    daftarBuku[5].tipe = TipeBuku::ONLINE;
    daftarBuku[5].dipinjam = false;
    daftarBuku[5].idPenggunaPeminjam = -1;
    daftarBuku[5].tempatBuku = "https://library.digital/macro-economics";

    daftarBuku[6].id = 7;
    daftarBuku[6].judul = "Machine Learning Fundamentals";
    daftarBuku[6].penulis = "Andrew Ng";
    daftarBuku[6].kategori = KategoriBuku::ILMU_KOMPUTER;
    daftarBuku[6].tipe = TipeBuku::ONLINE;
    daftarBuku[6].dipinjam = false;
    daftarBuku[6].idPenggunaPeminjam = -1;
    daftarBuku[6].tempatBuku = "https://library.digital/ml-fundamentals";

    daftarBuku[7].id = 8;
    daftarBuku[7].judul = "Hadits Shahih Bukhari";
    daftarBuku[7].penulis = "Imam Bukhari";
    daftarBuku[7].kategori = KategoriBuku::AGAMA;
    daftarBuku[7].tipe = TipeBuku::ONLINE;
    daftarBuku[7].dipinjam = false;
    daftarBuku[7].idPenggunaPeminjam = -1;
    daftarBuku[7].tempatBuku = "https://library.digital/bukhari-hadits";

    daftarBuku[8].id = 9;
    daftarBuku[8].judul = "Psikologi Sosial";
    daftarBuku[8].penulis = "David G. Myers";
    daftarBuku[8].kategori = KategoriBuku::SOSIAL;
    daftarBuku[8].tipe = TipeBuku::ONLINE;
    daftarBuku[8].dipinjam = false;
    daftarBuku[8].idPenggunaPeminjam = -1;
    daftarBuku[8].tempatBuku = "https://library.digital/social-psychology";

    daftarBuku[9].id = 10;
    daftarBuku[9].judul = "Filsafat Kontemporer";
    daftarBuku[9].penulis = "Bertrand Russell";
    daftarBuku[9].kategori = KategoriBuku::FILSAFAT;
    daftarBuku[9].tipe = TipeBuku::ONLINE;
    daftarBuku[9].dipinjam = false;
    daftarBuku[9].idPenggunaPeminjam = -1;
    daftarBuku[9].tempatBuku = "https://library.digital/contemporary-philosophy";

    jumlahBuku = 10;
    
    // Tambahkan semua buku ke indeks BST
    for (int i = 0; i < jumlahBuku; i++) {
        indeksBuku.tambahBuku(&daftarBuku[i]);
    }
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
    cout << "ID   Judul                         Penulis                 Status        Kategori            Tempat Buku" << endl;
    cout << "------------------------------------------------------------------------------------------------------" << endl;
    
    vector<Buku*> semuaBuku;
    indeksBuku.getDaftarBuku(semuaBuku);
    for (Buku* buku : semuaBuku) {
        if (buku->tipe == TipeBuku::FISIK) {
            // Format ID (4 karakter)
            printf("%-4d", buku->id);
            
            // Format judul (30 karakter)
            string judul = buku->judul;
            if (judul.length() > 28) {
                judul = judul.substr(0, 25) + "...";
            }
            printf("%-30s", judul.c_str());
            
            // Format penulis (25 karakter)
            string penulis = buku->penulis;
            if (penulis.length() > 23) {
                penulis = penulis.substr(0, 20) + "...";
            }
            printf("%-25s", penulis.c_str());
            
            // Format status (15 karakter)
            string status = buku->dipinjam ? "Dipinjam" : "Tersedia";
            printf("%-15s", status.c_str());
            
            // Format kategori (20 karakter)
            string kategori = kategoriToString(buku->kategori);
            printf("%-20s", kategori.c_str());
            
            // Tempat buku (tanpa batas)
            cout << buku->tempatBuku;
            
            cout << endl;
        }
    }
    cout << "------------------------------------------------------------------------------------------------------" << endl;
}

void tampilkanBukuOnline() {
    cout << "\n--- DAFTAR BUKU ONLINE ---" << endl;
    printf("%-3s %-25s %-20s %-10s %-15s %s\n", 
          "ID", "Judul", "Penulis", "Status", "Kategori", "URL/Link");
    cout << "----------------------------------------------------------------------------------------" << endl;
    
    vector<Buku*> semuaBuku;
    indeksBuku.getDaftarBuku(semuaBuku);
    for (Buku* buku : semuaBuku) {
        if (buku->tipe == TipeBuku::ONLINE) {
            // Format judul
            string judul = buku->judul;
            if (judul.length() > 25) {
                judul = judul.substr(0, 22) + "...";
            }
            
            // Format penulis
            string penulis = buku->penulis;
            if (penulis.length() > 20) {
                penulis = penulis.substr(0, 17) + "...";
            }
            
            // Format URL
            string url = buku->tempatBuku;
            if (url.length() > 30) {
                url = url.substr(0, 27) + "...";
            }
            
            printf("%-3d %-25s %-20s %-10s %-15s %s\n", 
                  buku->id,
                  judul.c_str(),
                  penulis.c_str(),
                  (buku->dipinjam ? "Dipinjam" : "Tersedia"),
                  kategoriToString(buku->kategori).c_str(),
                  url.c_str());
        }
    }
    cout << "----------------------------------------------------------------------------------------" << endl;
}

void cekDetailBuku() {
    int idBuku;
    cout << "Masukkan ID Buku: ";
    if (!(cin >> idBuku)) {
        cout << "Input tidak valid. Harap masukkan angka." << endl;
        bersihkanBufferInput();
        return;
    }
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
    if (!(cin >> idBuku)) {
        cout << "Input ID Buku tidak valid." << endl;
        bersihkanBufferInput();
        return;
    }
    
    cout << "Masukkan ID Pengguna: ";
    if (!(cin >> idPengguna)) {
        cout << "Input ID Pengguna tidak valid." << endl;
        bersihkanBufferInput();
        return;
    }
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
    if (!(cin >> idBuku)) {
        cout << "Input ID Buku tidak valid." << endl;
        bersihkanBufferInput();
        return;
    }
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
    
    // Proses antrian jika ada yang menunggu
    if (!antrian.kosong()) {
        prosesAntrian();
    }
}

void daftarPenggunaBaru() {
    if (jumlahPengguna >= MAKS_PENGGUNA) {
        cout << "Batas maksimum pengguna tercapai." << endl;
        return;
    }
    
    cout << "Masukkan nama pengguna: ";
    string nama;
    getline(cin, nama);
    
    if (nama.empty()) {
        cout << "Nama pengguna tidak boleh kosong." << endl;
        return;
    }
    
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
    if (antrian.kosong()) {
        cout << "Tidak ada antrian untuk diproses." << endl;
        return;
    }

    NodeQueue* antrianDepan = antrian.dequeue();
    if (antrianDepan == nullptr) {
        return;
    }

    Buku* buku = indeksBuku.cariBuku(antrianDepan->idBuku);
    if (buku == nullptr) {
        cout << "Buku tidak ditemukan." << endl;
        delete antrianDepan;
        return;
    }

    if (buku->dipinjam) {
        // Buku masih dipinjam, masukkan kembali ke antrian
        antrian.enqueue(antrianDepan->idPengguna, antrianDepan->idBuku, 
                       antrianDepan->namaPengguna, antrianDepan->judulBuku);
        cout << "Buku masih dipinjam. Pengguna " << antrianDepan->namaPengguna 
             << " dikembalikan ke antrian." << endl;
    } else {
        // Buku tersedia, proses peminjaman
        buku->dipinjam = true;
        buku->idPenggunaPeminjam = antrianDepan->idPengguna;
        buku->waktuPinjam.setWaktuSekarang();
        
        riwayat.tambahRiwayat(antrianDepan->idBuku, antrianDepan->idPengguna, 
                             buku->judul, antrianDepan->namaPengguna, buku->waktuPinjam);
        
        cout << "Buku '" << buku->judul << "' berhasil dipinjam oleh " 
             << antrianDepan->namaPengguna << " dari antrian." << endl;
    }

    delete antrianDepan;
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
    indeksBuku.clear();
    for (int i = 0; i < jumlahBuku; i++) {
        indeksBuku.tambahBuku(&daftarBuku[i]);
    }
    
    cout << "Buku berhasil diurutkan berdasarkan judul secara alfabetis." << endl;
}

void cariBuku() {
    string keyword;
    cout << "Masukkan kata kunci pencarian (judul): ";
    getline(cin, keyword);
    
    if (keyword.empty()) {
        cout << "Kata kunci tidak boleh kosong." << endl;
        return;
    }
    
    // Konversi ke lowercase dengan cara yang aman
    transform(keyword.begin(), keyword.end(), keyword.begin(), 
              [](unsigned char c) { return tolower(c); });
    
    cout << "\n--- HASIL PENCARIAN ---" << endl;
    bool ditemukan = false;
    
    vector<Buku*> semuaBuku;
    indeksBuku.getDaftarBuku(semuaBuku);
    for (Buku* buku : semuaBuku) {
        string judulLower = buku->judul;
        // Konversi ke lowercase dengan cara yang aman
        transform(judulLower.begin(), judulLower.end(), judulLower.begin(),
                  [](unsigned char c) { return tolower(c); });
        
        if (judulLower.find(keyword) != string::npos) {
            cout << "ID: " << buku->id << " | " 
                 << buku->judul << " (" << tipeToString(buku->tipe) << ")" 
                 << " - " << buku->penulis << endl;
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
        if (!(cin >> pilihan)) {
            cout << "Input tidak valid. Harap masukkan angka." << endl;
            bersihkanBufferInput();
            continue;
        }
        bersihkanBufferInput();
        
        switch (pilihan) {
            case 1: tampilkanBukuFisik(); break;
            case 2: tampilkanBukuOnline(); break;
            case 3: cekDetailBuku(); break;
            case 4: pinjamBuku(); break;
            case 5: kembalikanBuku(); break;
            case 6: daftarPenggunaBaru(); break;
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
