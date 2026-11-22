#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/struct.h"
#include "../include/file_io.h"
#include "../include/admin.h"

#define FILE_LAPORAN "data/laporan.txt"
#define MAKS_BARANG 100   /* harus sama dengan batas di bacaBarangDariBinary() */
#define MAKS_INPUT 128
#define BATAS_ANGKA 1000000000

/* Status hasil membaca input dari keyboard */
#define INPUT_OK 1
#define INPUT_SALAH 0
#define INPUT_EOF -1

/* ---------- fungsi bantu input (static = hanya dipakai di file ini) ---------- */

/* Membaca satu baris dengan aman (fgets), membuang '\n', dan membuang sisa
   baris bila input melebihi ukuran buffer. */
static int bacaBaris(const char *prompt, char *buf, size_t ukuran) {
    printf("%s", prompt);
    if (fgets(buf, (int)ukuran, stdin) == NULL) {
        buf[0] = '\0';
        return INPUT_EOF;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
        return INPUT_OK;
    }
    if (len == ukuran - 1) { /* terlalu panjang: buang sisanya */
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return INPUT_SALAH;
    }
    return INPUT_OK; /* baris terakhir tanpa '\n' */
}

static int ubahKeInt(const char *teks, int *hasil) {
    char *akhir;
    long v = strtol(teks, &akhir, 10);
    if (akhir == teks) return 0;
    while (*akhir == ' ') akhir++;
    if (*akhir != '\0' || v < 0 || v > BATAS_ANGKA) return 0;
    *hasil = (int)v;
    return 1;
}

static int ubahKeFloat(const char *teks, float *hasil) {
    char *akhir;
    float v = strtof(teks, &akhir);
    if (akhir == teks) return 0;
    while (*akhir == ' ') akhir++;
    if (*akhir != '\0' || v != v || v < 0 || v > BATAS_ANGKA) return 0;
    *hasil = v;
    return 1;
}

static int bacaInt(const char *prompt, int *hasil) {
    char buf[MAKS_INPUT];
    int status = bacaBaris(prompt, buf, sizeof(buf));
    if (status != INPUT_OK) return status;
    return ubahKeInt(buf, hasil) ? INPUT_OK : INPUT_SALAH;
}

static int bacaFloat(const char *prompt, float *hasil) {
    char buf[MAKS_INPUT];
    int status = bacaBaris(prompt, buf, sizeof(buf));
    if (status != INPUT_OK) return status;
    return ubahKeFloat(buf, hasil) ? INPUT_OK : INPUT_SALAH;
}

/* Menyalin teks ke buffer tujuan dengan batas ukuran (selalu diakhiri '\0') */
static void salinTeks(char *tujuan, size_t ukuran, const char *sumber) {
    strncpy(tujuan, sumber, ukuran - 1);
    tujuan[ukuran - 1] = '\0';
}

/* Kode wajib: tidak kosong, tidak ada spasi (kasir membacanya dengan %s),
   dan muat di struct (maksimal MAX_KODE - 1 karakter). */
static int kodeValid(const char *kode) {
    size_t len = strlen(kode);
    return len > 0 && len < MAX_KODE && strchr(kode, ' ') == NULL;
}

static int namaValid(const char *nama) {
    size_t len = strlen(nama);
    return len > 0 && len < MAX_NAMA;
}

/* Mengembalikan indeks barang dengan kode tersebut, atau -1 bila tidak ada.
   Indeks 'abaikan' dilewati (dipakai saat edit agar tidak bentrok dengan diri sendiri). */
static int cariIndeksBarang(const Barang *daftar, int jumlah, const char *kode, int abaikan) {
    for (int i = 0; i < jumlah; i++) {
        if (i != abaikan && strcmp(daftar[i].kode, kode) == 0) {
            return i;
        }
    }
    return -1;
}

/* ---------- fitur admin ---------- */

float hitungNilaiStok(Barang b) { /* pass by value */
    return b.stok * b.harga;
}

void tampilkanDaftarBarang(const Barang *daftar, int jumlah) {
    float total = 0;
    printf("\n========================================\n");
    printf("            DAFTAR INVENTARIS           \n");
    printf("========================================\n");
    if (jumlah == 0) {
        printf("Inventaris masih kosong.\n");
    } else {
        for (int i = 0; i < jumlah; i++) {
            printf("%d. Kode: %s | Nama: %s | Stok: %d | Harga: %.2f\n",
                   i + 1, daftar[i].kode, daftar[i].nama, daftar[i].stok, daftar[i].harga);
            total += hitungNilaiStok(daftar[i]);
        }
        printf("----------------------------------------\n");
        printf("Total nilai stok: %.2f\n", total);
    }
    printf("========================================\n");
}

void tambahBarang(Barang *daftar, int *jumlah, const char *file_db) {
    if (*jumlah >= MAKS_BARANG) {
        printf("Kapasitas inventaris penuh!\n");
        return;
    }

    Barang baru;
    char buf[MAKS_INPUT];

    if (bacaBaris("Masukkan Kode Barang: ", buf, sizeof(buf)) != INPUT_OK || !kodeValid(buf)) {
        printf("Kode tidak valid! (maks %d karakter, tanpa spasi, tidak boleh kosong)\n", MAX_KODE - 1);
        return;
    }
    if (cariIndeksBarang(daftar, *jumlah, buf, -1) != -1) {
        printf("Peringatan: Kode barang '%s' sudah terdaftar! Gunakan kode lain atau menu Edit.\n", buf);
        return;
    }
    salinTeks(baru.kode, sizeof(baru.kode), buf);

    if (bacaBaris("Masukkan Nama Barang: ", buf, sizeof(buf)) != INPUT_OK || !namaValid(buf)) {
        printf("Nama tidak valid! (maks %d karakter, tidak boleh kosong)\n", MAX_NAMA - 1);
        return;
    }
    salinTeks(baru.nama, sizeof(baru.nama), buf);

    if (bacaInt("Masukkan Stok: ", &baru.stok) != INPUT_OK) {
        printf("Input stok tidak valid!\n");
        return;
    }
    if (bacaFloat("Masukkan Harga: ", &baru.harga) != INPUT_OK) {
        printf("Input harga tidak valid!\n");
        return;
    }

    daftar[*jumlah] = baru;
    (*jumlah)++;
    simpanBarangKeBinary(file_db, daftar, *jumlah);
    catatLogLaporan(FILE_LAPORAN, "BARANG MASUK", baru.kode, baru.nama, baru.stok);
    printf("Barang berhasil ditambahkan dan dicatat ke laporan!\n");
}

void editBarang(Barang *daftar, int jumlah, const char *file_db) {
    char buf[MAKS_INPUT];

    if (bacaBaris("Masukkan Kode Barang yang ingin diedit: ", buf, sizeof(buf)) != INPUT_OK) {
        printf("Input tidak valid!\n");
        return;
    }
    int idx = cariIndeksBarang(daftar, jumlah, buf, -1);
    if (idx == -1) {
        printf("Barang dengan kode tersebut tidak ditemukan!\n");
        return;
    }

    Barang *b = &daftar[idx]; /* pass by pointer: mengubah data aslinya */
    Barang baru = *b;         /* kerja di salinan, baru dipakai bila semua input valid */
    int stok_lama = b->stok;

    printf("Barang ditemukan: [%s] %s (Stok: %d, Harga: %.2f)\n", b->kode, b->nama, b->stok, b->harga);
    printf("Tekan Enter tanpa mengetik apa pun untuk mempertahankan nilai lama.\n");

    if (bacaBaris("Kode Baru: ", buf, sizeof(buf)) != INPUT_OK) {
        printf("Input tidak valid! Edit dibatalkan.\n");
        return;
    }
    if (buf[0] != '\0') {
        if (!kodeValid(buf)) {
            printf("Kode tidak valid! Edit dibatalkan.\n");
            return;
        }
        if (cariIndeksBarang(daftar, jumlah, buf, idx) != -1) {
            printf("Peringatan: Kode barang '%s' sudah digunakan barang lain! Edit dibatalkan.\n", buf);
            return;
        }
        salinTeks(baru.kode, sizeof(baru.kode), buf);
    }

    if (bacaBaris("Nama Barang Baru (bisa spasi): ", buf, sizeof(buf)) != INPUT_OK) {
        printf("Input tidak valid! Edit dibatalkan.\n");
        return;
    }
    if (buf[0] != '\0') {
        if (!namaValid(buf)) {
            printf("Nama terlalu panjang! Edit dibatalkan.\n");
            return;
        }
        salinTeks(baru.nama, sizeof(baru.nama), buf);
    }

    if (bacaBaris("Stok Baru: ", buf, sizeof(buf)) != INPUT_OK ||
        (buf[0] != '\0' && !ubahKeInt(buf, &baru.stok))) {
        printf("Input stok baru tidak valid! Edit dibatalkan.\n");
        return;
    }

    if (bacaBaris("Harga Baru: ", buf, sizeof(buf)) != INPUT_OK ||
        (buf[0] != '\0' && !ubahKeFloat(buf, &baru.harga))) {
        printf("Input harga baru tidak valid! Edit dibatalkan.\n");
        return;
    }

    *b = baru;
    simpanBarangKeBinary(file_db, daftar, jumlah);

    if (b->stok > stok_lama) {
        catatLogLaporan(FILE_LAPORAN, "BARANG MASUK", b->kode, b->nama, b->stok - stok_lama);
    } else if (b->stok < stok_lama) {
        catatLogLaporan(FILE_LAPORAN, "BARANG KELUAR", b->kode, b->nama, stok_lama - b->stok);
    }
    printf("Data barang berhasil diperbarui!\n");
}

void hapusBarang(Barang *daftar, int *jumlah, const char *file_db) {
    char buf[MAKS_INPUT];

    if (bacaBaris("Masukkan Kode Barang yang ingin dihapus: ", buf, sizeof(buf)) != INPUT_OK) {
        printf("Input tidak valid!\n");
        return;
    }
    int idx = cariIndeksBarang(daftar, *jumlah, buf, -1);
    if (idx == -1) {
        printf("Barang dengan kode tersebut tidak ditemukan!\n");
        return;
    }

    printf("Hapus [%s] %s? (y/n): ", daftar[idx].kode, daftar[idx].nama);
    if (fgets(buf, sizeof(buf), stdin) == NULL || (buf[0] != 'y' && buf[0] != 'Y')) {
        printf("Penghapusan dibatalkan.\n");
        return;
    }

    Barang dihapus = daftar[idx];
    for (int i = idx; i < *jumlah - 1; i++) { /* geser elemen ke kiri */
        daftar[i] = daftar[i + 1];
    }
    (*jumlah)--;
    simpanBarangKeBinary(file_db, daftar, *jumlah);
    catatLogLaporan(FILE_LAPORAN, "BARANG DIHAPUS", dihapus.kode, dihapus.nama, dihapus.stok);
    printf("Barang berhasil dihapus dari inventaris!\n");
}

/* ---------- menu utama admin ---------- */

void jalankanAdmin(const char *file_db) {
    Barang daftar[MAKS_BARANG];
    int jumlah = bacaBarangDariBinary(file_db, daftar);
    if (jumlah == 0) {
        printf("File '%s' belum ada atau kosong. Dimulai dengan inventaris kosong.\n", file_db);
    }

    int pilihan = 0;
    do {
        printf("\n--- MENU ADMIN: KELOLA STOK ---\n");
        printf("1. Lihat Daftar Barang\n");
        printf("2. Tambah Barang Baru\n");
        printf("3. Edit Barang\n");
        printf("4. Hapus Barang\n");
        printf("5. Cetak Laporan Stok\n");
        printf("6. Keluar\n");

        int status = bacaInt("Pilihan: ", &pilihan);
        if (status == INPUT_EOF) {
            printf("\n");
            break;
        }
        if (status == INPUT_SALAH) {
            printf("Input tidak valid!\n");
            pilihan = 0;
            continue;
        }

        switch (pilihan) {
            case 1: tampilkanDaftarBarang(daftar, jumlah);       break;
            case 2: tambahBarang(daftar, &jumlah, file_db);      break;
            case 3: editBarang(daftar, jumlah, file_db);         break;
            case 4: hapusBarang(daftar, &jumlah, file_db);       break;
            case 5:
                tulisLaporanStok(FILE_LAPORAN, daftar, jumlah);
                printf("Rekap stok berhasil ditambahkan ke %s\n", FILE_LAPORAN);
                break;
            case 6: printf("Keluar dari mode admin.\n");         break;
            default: printf("Menu tidak tersedia!\n");
        }
    } while (pilihan != 6);
}