#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct servis {
    string model, merek, kendala, montir, tanggal;
    string nama_p, nomor_p;
    servis* next;
};

struct pelanggan {
    string nama, gender, alamat, nomor, tanggal;
    int umur;
    servis* riwayatservis;
    servis* antrianservis; 
    pelanggan *next, *prev;
};

pelanggan* headpel = NULL;
pelanggan* tailpel = NULL;
servis* front = NULL;
servis* rear = NULL;


pelanggan* caripelanggan(string nama) {
    pelanggan* temp = headpel;
    while (temp != NULL) {
        if (temp->nama == nama) return temp;
        temp = temp->next;
    }
    return NULL;
}

pelanggan* tambahpelanggan(string nama, string tanggal = "", string nomor = "", string alamat = "", int umur = 0, string gender = "") {
    pelanggan* baru = new pelanggan;
    baru->nama = nama;
    baru->tanggal = tanggal;
    baru->nomor = nomor;
    baru->alamat = alamat;
    baru->umur = umur;
    baru->gender = gender;
    baru->riwayatservis = NULL;
    baru->next = NULL;
    baru->prev = tailpel;

    if (headpel == NULL) {
        headpel = tailpel = baru;
    } else {
        tailpel->next = baru;
        tailpel = baru;
    }
    return baru;
}

void simpanantrian() {
    ofstream file("antrian.csv");
        servis* s = front;
        while (s != NULL) {
            file << s->nama_p << "," << s->nomor_p << "," 
                    << s->model << "," << s->merek << "," 
                    << s->kendala << "," << s->montir << "," 
                    << s->tanggal << "\n";
            s = s->next;
    }
    file.close();
}


void loadingantrian() {
    ifstream file("antrian.csv");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string nama, nomor, mod, mer, ken, mon, tanggal;

        getline(ss, nama, ',');
        getline(ss, nomor, ',');
        getline(ss, mod, ',');
        getline(ss, mer, ',');
        getline(ss, ken, ',');
        getline(ss, mon, ',');
        getline(ss, tanggal, ',');

        if (nama == "") continue;
        servis* baru = new servis;
        baru->model = mod;
        baru->merek = mer;
        baru->kendala = ken;
        baru->montir = mon;
        baru->tanggal = tanggal;
        baru->nama_p = nama;
        baru->nomor_p = nomor;
        baru->next = NULL;
       
        if (rear == NULL) front = rear = baru;
        else { rear->next = baru; rear = baru; }

        if (caripelanggan(nama) == NULL){}
    }
    file.close();
}

void simpan() {
    ofstream file("datalengkap.csv");
    pelanggan* p = headpel;
    while (p != NULL) {
        servis* s = p->riwayatservis;
        while (s != NULL) {
            file << p->nama << "," << p->nomor << "," << p->alamat << "," 
                 << p->umur << "," << p->gender << "," << s->model << "," 
                 << s->merek << "," << s->kendala << "," << s->montir << "," << s->tanggal << "\n";
            s = s->next;
        }
        p = p->next;
    }
    file.close();
}

void loading() {
    ifstream file("datalengkap.csv");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string nama, nomor, alamat, umurStr, gender, mod, mer, ken, mon, tanggal;

        getline(ss, nama, ',');
        getline(ss, nomor, ',');
        getline(ss, alamat, ',');
        getline(ss, umurStr, ',');
        getline(ss, gender, ',');
        getline(ss, mod, ',');
        getline(ss, mer, ',');
        getline(ss, ken, ',');
        getline(ss, mon, ',');
        getline(ss, tanggal, ',');

        if (nama == "") continue;

        pelanggan* pel = caripelanggan(nama);
        if (pel == NULL) {
            pel = tambahpelanggan(nama, tanggal, nomor, alamat, stoi(umurStr), gender);
        }

        servis* baru = new servis;
        baru->model = mod;
        baru->merek = mer;
        baru->kendala = ken;
        baru->montir = mon;
        baru->tanggal = tanggal;
        baru->nama_p = nama;
        baru->nomor_p = nomor;
        baru->next = pel->riwayatservis;
        pel->riwayatservis = baru;
    }
    file.close();
}

void enqueue(servis* baru) {
    if (rear == NULL) {
        front = rear = baru;
    } else {
        rear->next = baru;
        rear = baru;
    }
}

servis* dequeue() {
    if (front == NULL) return NULL;

    servis* temp = front;
    front = front->next;

    if (front == NULL) {
        rear = NULL;
    }
    
    temp->next = NULL;
    return temp;
}

void tambahservis() {
    string nama, mod, mer, ken, mon, tanggal;
    cout << "Nama Pelanggan: "; getline(cin, nama);
    pelanggan* pel = caripelanggan(nama);
    
    if (pel == NULL) {
        string nomor, alamat; int umur; string gender;
        cout << "No Telp Pelanggan: "; getline(cin, nomor);
        cout << "Alamat: "; getline(cin, alamat);
        cout << "Umur: "; cin >> umur; cin.ignore();
        cout << "Gender: "; getline(cin, gender);
        pel = tambahpelanggan(nama, tanggal, nomor, alamat, umur, gender);
    }

    cout << "Model Mobil: "; getline(cin, mod);
    cout << "Merek Mobil: "; getline(cin, mer);
    cout << "Kendala: "; getline(cin, ken);
    cout << "Montir: "; getline(cin, mon);
    cout << "Tanggal Masuk: "; getline(cin, tanggal);

    servis* baru = new servis;
    baru->model = mod;
    baru->merek = mer;
    baru->kendala = ken;
    baru->montir = mon;
    baru->tanggal = tanggal;
    baru->nama_p = pel->nama;
    baru->nomor_p = pel->nomor;
    baru->next = NULL;

    enqueue(baru);

    simpanantrian();
    cout << "\nBerhasil masuk antrian" << endl;
    cout << "Tekan Enter untuk kembali..."; cin.get();
}


void selesaikan() {
    if (front == NULL) { cout << "Antrian kosong.\n"; return; }

    servis* target = front; 

    cout << "\n=== SELESAIKAN PEKERJAAN ===\n";
    cout << "Nama Pelanggan: " << target->nama_p;
    cout << "\nModel Mobil: " << target->model;
    cout << "\nMerek Mobil: " << target->merek;
    cout << "\nMontir: " << target->montir << endl;
    cout << "Selesaikan sekarang? (y/n): ";
    string opt; getline(cin, opt);

    if (opt == "y") {
        servis* selesai = dequeue();

        pelanggan* pel = caripelanggan(selesai->nama_p);
        if (pel) {
            selesai->next = pel->riwayatservis;
            pel->riwayatservis = selesai;
        }

        simpan(); 
        simpanantrian();
        cout << "Pekerjaan sudah selesai dan masuk ke Riwayat.\n";
    }
}


void tampilsemuaservis() {
    cout << "\n=== SEMUA SERVIS ===\n";
    pelanggan* p = headpel;
    bool ada = false;
    while (p != NULL) {
        servis* s = p->riwayatservis;
        while (s != NULL) {
            cout << "-----------------------\n";
            cout << "Pelanggan: " << p->nama << "\nNo Telp: " << p->nomor << endl;
            cout << "Model Mobil: " << s->model << "\nMerek: " << s->merek << endl;
            cout << "Kendala: " << s->kendala << "\nMontir: " << s->montir << endl;
            s = s->next;
            ada = true;
        }
        p = p->next;
    }
    if(!ada) cout << "Belum ada data servis.\n";
    cout << "\nTekan Enter untuk kembali..."; cin.get();
}

void riwayatmontir() {
    cout << "\n=== RIWAYAT PEKERJAAN MONTIR ===\n";
    string listmontir[200];
    int count = 0;

    servis* s_antrian = front;
    while (s_antrian != NULL) {
        bool exist = false;
        int i = 0;
        while (i < count) {
            if (listmontir[i] == s_antrian->montir) { exist = true; break; }
            i++;
        }
        if (!exist && s_antrian->montir != "") {
            listmontir[count++] = s_antrian->montir;
        }
        s_antrian = s_antrian->next;
    }

    pelanggan* p = headpel;
    while (p != NULL) {
        servis* s_riwayat = p->riwayatservis;
        while (s_riwayat != NULL) {
            bool exist = false;
            int i = 0;
            while (i < count) {
                if (listmontir[i] == s_riwayat->montir) { exist = true; break; }
                i++;
            }
            if (!exist && s_riwayat->montir != "") {
                listmontir[count++] = s_riwayat->montir;
            }
            s_riwayat = s_riwayat->next;
        }
        p = p->next;
    }

    if (count == 0) {
        cout << "Belum ada data montir.\n";
        return;
    }

    int j = 0;
    while (j < count) {
        cout << j + 1 << ". " << listmontir[j] << endl;
        j++;
    }
    
    int pilih; cout << "Pilihan: "; cin >> pilih; cin.ignore();
    if (pilih < 1 || pilih > count) return;

    string target = listmontir[pilih - 1];
    cout << "\n=== RIWAYAT KERJA: " << target << " ===\n";
    
    pelanggan* p_temp = headpel;
    while (p_temp != NULL) {
        servis* s_temp = p_temp->riwayatservis;
        while (s_temp != NULL) {
            if (s_temp->montir == target) {
                cout << "\nNama Pelanggan: " << p_temp->nama << endl;
                cout << "Model Mobil: " << s_temp->model << endl;
                cout << "Merek Mobil: " << s_temp->merek << endl;
                cout << "Kendala: " << s_temp->kendala<< endl;
                cout << "Tanggal Masuk: " << s_temp->tanggal << endl;
                cout << "Nomor Telepon: " << s_temp->nomor_p << endl;
            }
            s_temp = s_temp->next;
        }
        p_temp = p_temp->next;
    }
    
    cout << "\nTekan Enter untuk kembali..."; 
    cin.get();
}

void menusemuapelanggan() {
    cout << "\n=== SEMUA PELANGGAN ===\n";
    pelanggan* p = headpel;
    if (p == NULL) cout << "Data pelanggan kosong.\n";
    while (p != NULL) {
        cout << "-----------------------\n";
        cout << "Nama: " << p->nama << "\nTelepon: " << p->nomor << "\nAlamat: " << p->alamat << endl;
        
        servis* s = p->riwayatservis;
        if (s != NULL) {
            while (s->next != NULL) s = s->next;
            cout << "--- Servis Terakhir ---\nMobil: " << s->model << "\nMontir: " << s->montir << endl;
        }
        p = p->next;
    }
    cout << "\nTekan Enter untuk kembali..."; 
    cin.get();
}

void menudatapelanggan() {
    if (headpel == NULL) {
        cout << "\nData pelanggan masih kosong!\n";
        cout << "Tekan Enter..."; 
        cin.get();
        return;
    }
    pelanggan* curr = headpel;
    string pilih;
    do {
        cout << "\n=== Data Pelanggan ===\n";
        cout << "Nama: " << curr->nama << "\nTelepon: " << curr->nomor << "\nUmur: " << curr->umur << endl;
        cout << "Gender: " << curr->gender << "\nAlamat: " << curr->alamat << endl;
        cout << "-----------------------\nRiwayat Servis:\n";
        servis* s = curr->riwayatservis;
        while (s != NULL) {
            cout << "- " << s->model << " | " << s->kendala << " | Montir: " << s->montir << endl;
            s = s->next;
        }
        cout << "-----------------------\n[N]ext, [P]revious, [E]xit\nPilihan: ";
        getline(cin, pilih);
        
        if ((pilih == "n" || pilih == "N") && curr->next != NULL) curr = curr->next;
        else if ((pilih == "p" || pilih == "P") && curr->prev != NULL) curr = curr->prev;
        
    } while (pilih != "e" && pilih != "E");
}


void antrianservis() {
    cout << "\n=== ANTRIAN SERVIS ===\n";
    
    servis* temp = front;
    bool ada = false;

    if (temp == NULL) {
        cout << "Belum ada antrian servis.\n";
    } else {
    while (temp != NULL) {
            cout << "-----------------------\n";
            cout << "Pelanggan: " << temp->nama_p << endl;
            cout << "Nomor Telepon: " << temp->nomor_p << endl;
            cout << "Model Mobil: " << temp->model << endl;
            cout << "Merek Mobil: " << temp->merek << endl;
            cout << "Kendala: " << temp->kendala << endl;
            cout << "Montir: " << temp->montir << endl;
            cout << "Status: Menunggu / Sedang dikerjakan" << endl;

            temp = temp->next;
            ada = true;
        }
    }
    cout << "\nTekan Enter untuk kembali..."; 
    cin.get();
}


void menuadmin() {
    int pilihan;
    do {
        cout << "\n=== ADMIN UNOSERVIS ===\n";
        cout << "Pilih Menu!\n";
        cout << "\n1. Servis\n";
        cout << "2. Pelanggan Baru\n";
        cout << "3. Keluar\n";
        cout << "Pilihan: ";
        if (!(cin >> pilihan)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        cin.ignore();

        if (pilihan == 1) {
            cout << "\n=== SUB-MENU SERVIS ===\n";
            cout << "1. Semua Servis Singkat\n";
            cout << "2. Servis Baru\n";
            cout << "3. Selesaikan Pekerjaan\n";
            cout << "4. Riwayat Kerja Montir\n";
            cout << "Pilihan: ";
            int p; cin >> p; cin.ignore();

            if (p == 1) tampilsemuaservis();
            else if (p == 2) tambahservis();
            else if (p == 3) selesaikan();
            else if (p == 4) riwayatmontir();

        } else if (pilihan == 2) {
            tambahservis();

        }
    } while (pilihan != 3);

    return;
}

void riwayatservisanda () {
    string nama;
    cout << "\n=== RIWAYAT SERVIS ANDA ===\n";
    cout << "Masukkan Nama: "; getline(cin, nama);

    pelanggan* p = caripelanggan(nama);
    if (p != NULL && p->riwayatservis != NULL) {
        servis* s = p->riwayatservis;
        while (s != NULL) {
            cout << "-----------------------\n";
            cout << "Pelanggan: " << p->nama << "\nNo Telp: " << p->nomor << "\nTanggal Masuk: " << p->tanggal << endl;
            cout << "Model Mobil: " << s->model << "\nMerek: " << s->merek << endl;
            cout << "Kendala: " << s->kendala << "\nMontir: " << s->montir << endl;
            s = s->next;
        }
    } else {
        cout << "Belum ada data servis.\n";
    }
    cout << "\nTekan Enter untuk kembali..."; cin.get();
    return;
}


int main() {
    loading();
    string pilihan;
    do {
        cout << "\n=== SELAMAT DATANG DI UNOSERVIS ===\n";
        cout << "Pilih Menu!\n";
        cout << "\n1. Antrian Servis\n";
        cout << "2. Riwayat Servis Anda\n";
        cout << "3. Keluar\n";
        cout << "Pilihan: ";
        if (!(cin >> pilihan)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        cin.ignore();

        if (pilihan == "1") {
            antrianservis();

        } else if (pilihan == "2") {
            riwayatservisanda();

        } else if (pilihan == "adminakses777") {
            menuadmin();
        }
    } while (pilihan != "3");

    return 0;
}
