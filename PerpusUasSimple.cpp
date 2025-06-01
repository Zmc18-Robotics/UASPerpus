#include <iostream>
#include <iomanip>
using namespace std;

const int MAX_BOOKS = 9;
const int MAX_USERS = 6;
const int MAX_HISTORY = 10;

bool compareString(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

void copyString(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool containsSubstring(const char* str, const char* sub) {
    for (int i = 0; str[i] != '\0'; i++) {
        int j = 0;
        while (sub[j] != '\0' && str[i + j] == sub[j]) {
            j++;
        }
        if (sub[j] == '\0') return true;
    }
    return false;
}

struct Book {
    int id;
    char title[50];
    char author[30];
    bool isBorrowed;
    int rackRow;
    int rackCol;
    Book* next;
};

struct QueueNode {
    int userId;
    QueueNode* next;
};

struct Queue {
    QueueNode* front;
    QueueNode* rear;
};

struct BorrowHistory {
    int bookId;
};

struct User {
    int id;
    char name[30];
    BorrowHistory history[MAX_HISTORY];
    int historyCount;
};

Book* head = NULL;
User users[MAX_USERS] = {
    {1, "Nafhan", {}, 0},
    {2, "Cakra", {}, 0},
    {3, "Arif", {}, 0},
    {4, "Zidane", {}, 0},
    {5, "Rivaldi", {}, 0},
    {6, "Dafi", {}, 0}
};
Queue bookQueues[MAX_BOOKS];

void initBooks() {
    int idCounter = 101;
    const char titles[3][3][50] = {
        {"C++ Dasar", "Logika Pemrograman", "Struktur Data"},
        {"Alam Semesta", "Dunia Sains", "Kosmos"},
        {"Sejarah Dunia", "Perang Dunia", "Imperium"}
    };
    const char authors[3][3][30] = {
        {"Walter Savitch", "Martin Fowler", "Robert Lafore"},
        {"Bill Bryson", "Brian Cox", "Carl Sagan"},
        {"E.H.Gombrich", "Antony Beevor", "Niall Ferguson"}
    };

    for (int cat = 0; cat < 3; cat++) {
        for (int i = 0; i < 3; i++) {
            Book* newBook = new Book;
            newBook->id = idCounter++;
            copyString(newBook->title, titles[cat][i]);
            copyString(newBook->author, authors[cat][i]);
            newBook->isBorrowed = false;
            newBook->rackRow = cat + 1;
            newBook->rackCol = i + 1;
            newBook->next = head;
            head = newBook;
        }
    }

    for (int i = 0; i < MAX_BOOKS; i++) {
        bookQueues[i].front = bookQueues[i].rear = NULL;
    }
}

Book* findBookById(int id) {
    Book* temp = head;
    while (temp) {
        if (temp->id == id) return temp;
        temp = temp->next;
    }
    return NULL;
}

int authenticateUser(int id, char* name) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].id == id && compareString(users[i].name, name))
            return i;
    }
    return -1;
}

int getBookIndex(int id) {
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

void showBooks() {
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

    Book* temp = head;
    while (temp) {
        if (temp->rackRow == pilihan) {
            cout << left
                 << setw(5) << temp->id
                 << setw(30) << temp->title
                 << setw(20) << temp->author
                 << setw(12) << (temp->isBorrowed ? "Dipinjam" : "Tersedia")
                 << temp->rackRow << "-" << temp->rackCol << "\n";
        }
        temp = temp->next;
    }

    cout << string(77, '-') << "\n";
    system("pause");
    system("cls");
}

void sortBooks() {
    for (Book* i = head; i != NULL; i = i->next) {
        for (Book* j = i->next; j != NULL; j = j->next) {
            int k = 0;
            while (i->title[k] == j->title[k] && i->title[k] != '\0') k++;
            if (i->title[k] > j->title[k]) {
                swap(i->id, j->id);
                swap(i->rackRow, j->rackRow);
                swap(i->rackCol, j->rackCol);
                swap(i->isBorrowed, j->isBorrowed);

                char tempTitle[50], tempAuthor[30];
                copyString(tempTitle, i->title);
                copyString(i->title, j->title);
                copyString(j->title, tempTitle);

                copyString(tempAuthor, i->author);
                copyString(i->author, j->author);
                copyString(j->author, tempAuthor);
            }
        }
    }
}

void showAllBooksSorted() {
    cout << "=== DAFTAR BUKU (URUTAN ABJAD) ===\n\n";

    cout << left
         << setw(5) << "ID"
         << setw(30) << "Judul"
         << setw(20) << "Penulis"
         << setw(12) << "Status"
         << setw(10) << "Rak"
         << "\n";
    cout << string(77, '-') << "\n";

    Book* temp = head;
    while (temp) {
        cout << left
             << setw(5) << temp->id
             << setw(30) << temp->title
             << setw(20) << temp->author
             << setw(12) << (temp->isBorrowed ? "Dipinjam" : "Tersedia")
             << temp->rackRow << "-" << temp->rackCol << "\n";
        temp = temp->next;
    }

    cout << string(77, '-') << "\n";
    system("pause");
    system("cls");
}


void searchBookByTitle(const char* keyword) {
    cout << "=== HASIL PENCARIAN ===\n";
    bool found = false;
    Book* temp = head;
    while (temp) {
        if (containsSubstring(temp->title, keyword)) {
            cout << "ID: " << temp->id << "\n"
                 << "Judul : " << temp->title << "\n"
                 << "Penulis : " << temp->author << "\n"
                 << "Status : " << (temp->isBorrowed ? "Dipinjam" : "Tersedia") << "\n"
                 << "Posisi Rak : " << temp->rackRow << "-" << temp->rackCol << "\n\n";
            found = true;
        }
        temp = temp->next;
    }
    if (!found) cout << "Buku tidak ditemukan.\n";
    system("pause");
    system("cls");
}

void borrowBook() {
    int uid, bid;
    char name[30];
    cout << "Masukkan ID Pengguna: "; cin >> uid;
    cout << "Masukkan Nama Pengguna: "; cin.ignore(); cin.getline(name,30);
    int idx = authenticateUser(uid, name);
    if (idx == -1) {
        cout << "Akun tidak ditemukan.\n";
        system("pause");
        system("cls");
        return;
    }
    cout << "Masukkan ID Buku: "; cin >> bid;
    Book* book = findBookById(bid);
    if (!book) {
        cout << "Buku tidak ditemukan.\n";
        system("pause");
        system("cls");
        return;
    }

    int bookIdx = getBookIndex(bid);
    if (!book->isBorrowed) {
        book->isBorrowed = true;
        if(users[idx].historyCount < MAX_HISTORY)
            users[idx].history[users[idx].historyCount++].bookId = bid;
        cout << "Berhasil meminjam buku: " << book->title << endl;
    } else {
        enqueue(bookQueues[bookIdx], uid);
        cout << "Buku sedang dipinjam. Anda masuk dalam antrean.\n";
    }
    system("pause");
    system("cls");
}

void returnBook() {
    int uid, bid;
    char name[30];
    cout << "Masukkan ID Pengguna: "; cin >> uid;
    cout << "Masukkan Nama Pengguna: "; cin.ignore(); cin.getline(name,30);
    int idx = authenticateUser(uid, name);
    if (idx == -1) {
        cout << "Akun tidak ditemukan.\n";
        system("pause");
        system("cls");
        return;
    }
    cout << "Masukkan ID Buku: "; cin >> bid;
    Book* book = findBookById(bid);
    if (!book || !book->isBorrowed) {
        cout << "Buku tidak dipinjam.\n";
        system("pause");
        system("cls");
        return;
    }

    int bookIdx = getBookIndex(bid);
    int nextUser = dequeue(bookQueues[bookIdx]);
    if (nextUser != -1) {
        book->isBorrowed = true;
        for (int i = 0; i < MAX_USERS; i++) {
            if (users[i].id == nextUser) {
                if(users[i].historyCount < MAX_HISTORY)
                    users[i].history[users[i].historyCount++].bookId = bid;
                break;
            }
        }
        cout << "Buku langsung diberikan ke pengguna dalam antrean (ID: " << nextUser << ")\n";
    } else {
        book->isBorrowed = false;
        cout << "Buku berhasil dikembalikan.\n";
    }
    system("pause");
    system("cls");
}

void viewHistory() {
    int uid;
    char name[30];
    cout << "Masukkan ID Pengguna: "; cin >> uid;
    cout << "Masukkan Nama Pengguna: "; cin.ignore(); cin.getline(name,30);
    int idx = authenticateUser(uid, name);
    if (idx == -1) {
        cout << "Akun tidak ditemukan.\n";
        system("pause");
        system("cls");
        return;
    }
    cout << "=== Riwayat Pinjaman Pengguna " << users[idx].name << " ===\n";
    if(users[idx].historyCount == 0) {
        cout << "Belum ada riwayat pinjaman.\n";
    }
    for (int i = 0; i < users[idx].historyCount; i++) {
        Book* book = findBookById(users[idx].history[i].bookId);
        if (book)
            cout << "- " << book->title << " (ID: " << book->id << ")\n";
    }
    system("pause");
    system("cls");
}

int main() {
    initBooks();
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
            case 1: showBooks(); break;
            case 2: borrowBook(); break;
            case 3: returnBook(); break;
            case 4: viewHistory(); break;
            case 5: 
                sortBooks(); 
                showAllBooksSorted(); 
                break;
            case 6: {
                char keyword[50];
                cout << "Masukkan Judul yang Dicari: "; cin.ignore(); cin.getline(keyword, 50);
                searchBookByTitle(keyword);
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
        Book* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}
