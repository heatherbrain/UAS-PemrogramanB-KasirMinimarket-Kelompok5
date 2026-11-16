#include <stdio.h>
#include <string.h>
#include "../include/struct.h"
#include "../include/file_io.h"
#include "../include/kasir.h"

#define FILE_DB "data/inventaris.bin"
#define FILE_STRUK "data/struk_transaksi.txt"

void jalankanKasir(void) {
    Barang daftar[100];
    int jumlah_barang = bacaBarangDariBinary(FILE_DB, daftar);

    if (jumlah_barang == 0) {
        printf("Inventaris kosong! Harap isi stok melalui mode admin terlebih dahulu.\n");
        return;
    }

    ItemTransaksi keranjang[50];
    int jumlah_item = 0;
    float total_bayar = 0;
    char lanjut = 'y';

    printf("\n--- MODE KASIR: TRANSAKSI ---\n");
    while (lanjut == 'y' || lanjut == 'Y') {
        char kode_cari[10];
        int qty;

        printf("Masukkan Kode Barang: ");
        scanf("%s", kode_cari);
        printf("Masukkan Jumlah Beli: ");
        scanf("%d", &qty);

        // Cari barang di inventaris
        int indeks_ditemukan = -1;
        for (int i = 0; i < jumlah_barang; i++) {
            if (strcmp(daftar[i].kode, kode_cari) == 0) {
                indeks_ditemukan = i;
                break;
            }
        }

        if (indeks_ditemukan != -1 && daftar[indeks_ditemukan].stok >= qty) {
            daftar[indeks_ditemukan].stok -= qty; // Kurangi stok secara real-time
            float subtotal = daftar[indeks_ditemukan].harga * qty;
            
            // Masukkan ke keranjang sementara
            strcpy(keranjang[jumlah_item].kode, kode_cari);
            keranjang[jumlah_item].jumlah_beli = qty;
            keranjang[jumlah_item].subtotal = subtotal;
            jumlah_item++;

            total_bayar += subtotal;
            printf("Ditambahkan! Subtotal: %.2f\n", subtotal);
        } else {
            printf("Barang tidak ditemukan atau stok tidak mencukupi!\n");
        }

        printf("Tambah barang lagi? (y/n): ");
        scanf(" %c", &lanjut);
    }

    float uang_tunai, kembalian;
    printf("\nTotal Belanja: %.2f\n", total_bayar);
    printf("Masukkan Uang Tunai: ");
    scanf("%f", &uang_tunai);

    if (uang_tunai < total_bayar) {
        printf("Uang tunai kurang! Transaksi dibatalkan.\n");
        // Kembalikan stok yang sempat dikurangi jika transaksi batal
        // (Bisa disesuaikan dengan kebutuhan validasi tambahan)
        return;
    }

    kembalian = uang_tunai - total_bayar;
    printf("Kembalian: %.2f\n", kembalian);

    // Simpan pembaruan stok terbaru ke file binary & cetak struk ke file teks
    simpanBarangKeBinary(FILE_DB, daftar, jumlah_barang);
    cetakStrukTeks(FILE_STRUK, keranjang, jumlah_item, total_bayar, uang_tunai, kembalian);
    
    printf("Transaksi selesai! Struk telah dicetak ke %s\n", FILE_STRUK);
}