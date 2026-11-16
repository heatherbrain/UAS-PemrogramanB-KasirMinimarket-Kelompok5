#include <stdio.h>
#include <string.h>
#include "../include/struct.h"
#include "../include/file_io.h"
#include "../include/admin.h"

#define FILE_DB "data/inventaris.bin"

void jalankanAdmin(void) {
    Barang daftar[100];
    int jumlah = bacaBarangDariBinary(FILE_DB, daftar);

    int pilihan;
    do {
        printf("\n--- MENU ADMIN: KELOLA STOK ---\n");
        printf("1. Lihat Daftar Barang\n");
        printf("2. Tambah Barang Baru\n");
        printf("3. Edit Barang (Stok/Harga)\n");
        printf("4. Hapus Barang\n");
        printf("5. Keluar\n");
        printf("Pilihan: ");
        if (scanf("%d", &pilihan) != 1) {
            printf("Input tidak valid!\n");
            while(getchar() != '\n'); // Bersihkan buffer
            continue;
        }

        if (pilihan == 1) {
            printf("\n========================================\n");
            printf("            DAFTAR INVENTARIS           \n");
            printf("========================================\n");
            if (jumlah == 0) {
                printf("Inventaris masih kosong.\n");
            } else {
                for (int i = 0; i < jumlah; i++) {
                    printf("%d. Kode: %s | Nama: %s | Stok: %d | Harga: %.2f\n", 
                            i+1, daftar[i].kode, daftar[i].nama, daftar[i].stok, daftar[i].harga);
                }
            }
            printf("========================================\n");

        } else if (pilihan == 2) {
            if (jumlah >= 100) {
                printf("Kapasitas inventaris penuh!\n");
                continue;
            }
            printf("Masukkan Kode Barang: ");
            scanf("%s", daftar[jumlah].kode);
            
            // Membersihkan buffer sebelum membaca string dengan spasi
            getchar(); 
            printf("Masukkan Nama Barang (bisa spasi): ");
            scanf("%[^\n]", daftar[jumlah].nama);
            
            printf("Masukkan Stok: ");
            scanf("%d", &daftar[jumlah].stok);
            printf("Masukkan Harga: ");
            scanf("%f", &daftar[jumlah].harga);
            
            jumlah++;
            simpanBarangKeBinary(FILE_DB, daftar, jumlah);
            printf("Barang berhasil ditambahkan dan disimpan!\n");

        } else if (pilihan == 3) {
            char kode_cari[10];
            printf("Masukkan Kode Barang yang ingin di-edit: ");
            scanf("%s", kode_cari);

            int ditemukan = -1;
            for (int i = 0; i < jumlah; i++) {
                if (strcmp(daftar[i].kode, kode_cari) == 0) {
                    ditemukan = i;
                    break;
                }
            }

            if (ditemukan != -1) {
                printf("Barang ditemukan: %s (Stok: %d, Harga: %.2f)\n", 
                        daftar[ditemukan].nama, daftar[ditemukan].stok, daftar[ditemukan].harga);
                printf("Masukkan Stok Baru: ");
                scanf("%d", &daftar[ditemukan].stok);
                printf("Masukkan Harga Baru: ");
                scanf("%f", &daftar[ditemukan].harga);

                simpanBarangKeBinary(FILE_DB, daftar, jumlah);
                printf("Data barang berhasil diperbarui!\n");
            } else {
                printf("Barang dengan kode tersebut tidak ditemukan!\n");
            }

        } else if (pilihan == 4) {
            char kode_cari[10];
            printf("Masukkan Kode Barang yang ingin dihapus: ");
            scanf("%s", kode_cari);

            int ditemukan = -1;
            for (int i = 0; i < jumlah; i++) {
                if (strcmp(daftar[i].kode, kode_cari) == 0) {
                    ditemukan = i;
                    break;
                }
            }

            if (ditemukan != -1) {
                // Geser elemen array untuk menimpa data yang dihapus
                for (int i = ditemukan; i < jumlah - 1; i++) {
                    daftar[i] = daftar[i + 1];
                }
                jumlah--;
                simpanBarangKeBinary(FILE_DB, daftar, jumlah);
                printf("Barang berhasil dihapus dari inventaris!\n");
            } else {
                printf("Barang dengan kode tersebut tidak ditemukan!\n");
            }
        }
    } while (pilihan != 5);
}