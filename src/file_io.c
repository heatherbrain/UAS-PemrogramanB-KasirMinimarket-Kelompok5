#include <stdio.h>
#include "../include/file_io.h"

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
    fread(&jumlah, sizeof(int), 1, file);
    fread(daftar_barang, sizeof(Barang), jumlah, file);
    fclose(file);
    return jumlah;
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