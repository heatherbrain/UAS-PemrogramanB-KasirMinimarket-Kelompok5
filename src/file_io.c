#include <stdio.h>
#include "../include/main.h"

void simpanBarangKeBinary(const char *filename, Barang *daftar_barang, int jumlah) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Gagal membuka file untuk menyimpan data!\n");
        return;
    }
    fwrite(&jumlah, sizeof(int), 1, file);
    fwrite(daftar_barang, sizeof(Barang), jumlah, file);
    fclose(file);
}

int bacaBarangDariBinary(const char *filename, Barang *daftar_barang) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0; // File belum ada atau kosong
    }
    int jumlah = 0;
    if (fread(&jumlah, sizeof(int), 1, file) != 1 || jumlah < 0 || jumlah > 100) {
        fclose(file);
        return 0;
    }
    size_t terbaca = fread(daftar_barang, sizeof(Barang), jumlah, file);
    fclose(file);
    return (int)terbaca;
    // return jumlah;
}

void cetakStrukTeks(const char *filename, ItemTransaksi *keranjang, int jumlah_item, float total_bayar, float uang_tunai, float kembalian) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal mencetak struk!\n");
        return;
    }
    fprintf(file, "========================================\n");
    fprintf(file, "          MINIMARKET CLI POS            \n");
    fprintf(file, "========================================\n");
    for (int i = 0; i < jumlah_item; i++) {
        fprintf(file, "Kode: %s | Qty: %d | Subtotal: %.2f\n", 
                keranjang[i].kode, keranjang[i].jumlah_beli, keranjang[i].subtotal);
    }
    fprintf(file, "----------------------------------------\n");
    fprintf(file, "Total Bayar : %.2f\n", total_bayar);
    fprintf(file, "Uang Tunai  : %.2f\n", uang_tunai);
    fprintf(file, "Kembalian   : %.2f\n", kembalian);
    fprintf(file, "========================================\n");
    fprintf(file, "       TERIMA KASIH TELAH BERBELANJA    \n");
    fprintf(file, "========================================\n");
    fclose(file);
}

void catatLogLaporan(const char *filename, const char *jenis, const char *kode, const char *nama, int qty) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Gagal membuka %s untuk mencatat log!\n", filename);
        return;
    }

    fprintf(file, "[%s] Kode: %s | Nama: %s | Jumlah: %d\n", jenis, kode, nama, qty);
    fclose(file);
}

void tulisLaporanStok(const char *filename, Barang *daftar, int jumlah) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Gagal menulis laporan stok ke %s!\n", filename);
        return;
    }

    fprintf(file, "\n========================================\n");
    fprintf(file, "       REKAP STOK INVENTARIS TERKINI    \n");
    fprintf(file, "========================================\n");
    if (jumlah == 0) {
        fprintf(file, "Data inventaris masih kosong.\n");
    } else {
        for (int i = 0; i < jumlah; i++) {
            fprintf(file, "%d. [%s] %s | Stok: %d | Harga: %.2f\n",
                    i + 1, daftar[i].kode, daftar[i].nama, daftar[i].stok, daftar[i].harga);
        }
    }
    fprintf(file, "========================================\n\n");

    fclose(file);
}