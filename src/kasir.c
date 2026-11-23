#include <stdio.h>
#include <string.h>
#include "../include/main.h"

#define FILE_DB "data/inventaris.bin"
#define FILE_STRUK "data/struk_transaksi.txt"
#define FILE_LAPORAN "data/laporan.txt"

void jalankanKasir(void) {
    Barang daftar[100];
    int jumlah_barang = bacaBarangDariBinary(FILE_DB, daftar);

    if (jumlah_barang == 0) {
        printf("Inventaris kosong! Harap isi stok melalui mode admin terlebih dahulu.\n");
        return;
    }

    ItemTransaksi keranjang[50];
    int indeks_keranjang[50];
    int jumlah_item = 0;
    float total_bayar = 0.0f;
    char lanjut = 'y';

    printf("\n--- MODE KASIR: TRANSAKSI ---\n");
    while (lanjut == 'y' || lanjut == 'Y') {
        if (jumlah_item >= 50) {
            printf("Keranjang belanja sudah penuh!\n");
            break;
        }

        char kode_cari[20];
        int qty;

        printf("Masukkan Kode Barang: ");
        scanf("%19s", kode_cari);
        printf("Masukkan Jumlah Beli: ");
        if (scanf("%d", &qty) != 1) {
            printf("Input jumlah tidak valid!\n");
            while (getchar() != '\n');
            continue;
        }

        if (qty <= 0) {
            printf("Jumlah beli harus lebih dari 0!\n");
            printf("Tambah barang lagi? (y/n): ");
            scanf(" %c", &lanjut);
            continue;
        }

        int indeks_ditemukan = -1;
        for (int i = 0; i < jumlah_barang; i++) {
            if (strcmp(daftar[i].kode, kode_cari) == 0) {
                indeks_ditemukan = i;
                break;
            }
        }

        if (indeks_ditemukan != -1 && daftar[indeks_ditemukan].stok >= qty) {
            daftar[indeks_ditemukan].stok -= qty;
            float subtotal = daftar[indeks_ditemukan].harga * qty;

            strcpy(keranjang[jumlah_item].kode, kode_cari);
            keranjang[jumlah_item].jumlah_beli = qty;
            keranjang[jumlah_item].subtotal = subtotal;
            indeks_keranjang[jumlah_item] = indeks_ditemukan;
            jumlah_item++;

            total_bayar += subtotal;
            printf("Ditambahkan! Subtotal: %.2f\n", subtotal);
        } else {
            printf("Barang tidak ditemukan atau stok tidak mencukupi!\n");
        }

        printf("Tambah barang lagi? (y/n): ");
        scanf(" %c", &lanjut);
    }

    if (jumlah_item == 0) {
        printf("Tidak ada barang di keranjang. Transaksi dibatalkan.\n");
        return;
    }

    float uang_tunai = 0.0f, kembalian = 0.0f;
    printf("\nTotal Belanja: %.2f\n", total_bayar);
    printf("Masukkan Uang Tunai: ");
    if (scanf("%f", &uang_tunai) != 1) {
        printf("Input uang tunai tidak valid! Transaksi dibatalkan.\n");
        while (getchar() != '\n');
        return;
    }

    if (uang_tunai < total_bayar) {
        printf("Uang tunai kurang! Transaksi dibatalkan.\n");
        for (int i = 0; i < jumlah_item; i++) {
            int idx = indeks_keranjang[i];
            daftar[idx].stok += keranjang[i].jumlah_beli;
        }
        return;
    }

    kembalian = uang_tunai - total_bayar;
    printf("Kembalian: %.2f\n", kembalian);

    simpanBarangKeBinary(FILE_DB, daftar, jumlah_barang);
    cetakStrukTeks(FILE_STRUK, keranjang, jumlah_item, total_bayar, uang_tunai, kembalian);

    for (int i = 0; i < jumlah_item; i++) {
        int idx = indeks_keranjang[i];
        catatLogLaporan(FILE_LAPORAN, "BARANG KELUAR", keranjang[i].kode, daftar[idx].nama, keranjang[i].jumlah_beli);
    }

    printf("Transaksi selesai! Struk dicetak ke %s dan log tercatat ke %s\n", FILE_STRUK, FILE_LAPORAN);
}