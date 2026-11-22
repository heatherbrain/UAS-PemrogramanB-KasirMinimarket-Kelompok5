#include <stdio.h>
#include <string.h>
#include "../include/struct.h"
#include "../include/file_io.h"
#include "../include/admin.h"

#define FILE_DB "data/inventaris.bin"
#define FILE_LAPORAN "data/laporan.txt"

void jalankanAdmin(void) {
    Barang daftar[100];
    int jumlah = bacaBarangDariBinary(FILE_DB, daftar);

    int pilihan;
    do {
        printf("\n--- MENU ADMIN: KELOLA STOK ---\n");
        printf("1. Lihat Daftar Barang\n");
        printf("2. Tambah Barang Baru\n");
        printf("3. Edit Barang\n");
        printf("4. Hapus Barang\n");
        printf("5. Cetak Laporan Stok\n");
        printf("6. Keluar\n");
        printf("Pilihan: ");
        if (scanf("%d", &pilihan) != 1) {
            printf("Input tidak valid!\n");
            while (getchar() != '\n');
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
                           i + 1, daftar[i].kode, daftar[i].nama, daftar[i].stok, daftar[i].harga);
                }
            }
            printf("========================================\n");

        } else if (pilihan == 2) {
            if (jumlah >= 100) {
                printf("Kapasitas inventaris penuh!\n");
                continue;
            }

            char kode_baru[20];
            printf("Masukkan Kode Barang: ");
            scanf("%19s", kode_baru);

            // Cek seluruh array dari indeks 0 sampai jumlah - 1
            int duplikat = 0;
            for (int i = 0; i < jumlah; i++) {
                if (strcmp(daftar[i].kode, kode_baru) == 0) {
                    duplikat = 1;
                    break;
                }
            }

            if (duplikat) {
                printf("Peringatan: Kode barang '%s' sudah terdaftar! Gunakan kode lain atau menu Edit.\n", kode_baru);
                continue;
            }

            strcpy(daftar[jumlah].kode, kode_baru);

            printf("Masukkan Nama Barang: ");
            scanf(" %[^\n]", daftar[jumlah].nama);

            printf("Masukkan Stok: ");
            if (scanf("%d", &daftar[jumlah].stok) != 1 || daftar[jumlah].stok < 0) {
                printf("Input stok tidak valid!\n");
                while (getchar() != '\n');
                continue;
            }

            printf("Masukkan Harga: ");
            if (scanf("%f", &daftar[jumlah].harga) != 1 || daftar[jumlah].harga < 0) {
                printf("Input harga tidak valid!\n");
                while (getchar() != '\n');
                continue;
            }

            jumlah++;
            simpanBarangKeBinary(FILE_DB, daftar, jumlah);
            catatLogLaporan(FILE_LAPORAN, "BARANG MASUK", daftar[jumlah - 1].kode, daftar[jumlah - 1].nama, daftar[jumlah - 1].stok);
            printf("Barang berhasil ditambahkan dan dicatat ke laporan!\n");
        } else if (pilihan == 3) {
            char kode_cari[20];
            printf("Masukkan Kode Barang yang ingin diedit: ");
            scanf("%19s", kode_cari);

            int ditemukan = -1;
            for (int i = 0; i < jumlah; i++) {
                if (strcmp(daftar[i].kode, kode_cari) == 0) {
                    ditemukan = i;
                    break;
                }
            }

            if (ditemukan != -1) {
                char kode_baru[20];
                char nama_baru[100];
                int stok_lama = daftar[ditemukan].stok;
                int stok_baru;
                float harga_baru;

                printf("Barang ditemukan: [%s] %s (Stok: %d, Harga: %.2f)\n",
                       daftar[ditemukan].kode, daftar[ditemukan].nama,
                       daftar[ditemukan].stok, daftar[ditemukan].harga);

                printf("Masukkan Kode Baru: ");
                scanf("%19s", kode_baru);

                // Cek duplikasi ke seluruh item lain (abaikan indeks barang ini sendiri)
                int duplikat = 0;
                for (int i = 0; i < jumlah; i++) {
                    if (i != ditemukan && strcmp(daftar[i].kode, kode_baru) == 0) {
                        duplikat = 1;
                        break;
                    }
                }

                if (duplikat) {
                    printf("Peringatan: Kode barang '%s' sudah digunakan barang lain! Edit dibatalkan.\n", kode_baru);
                    continue;
                }

                printf("Masukkan Nama Barang Baru (bisa spasi): ");
                scanf(" %[^\n]", nama_baru);

                printf("Masukkan Stok Baru: ");
                if (scanf("%d", &stok_baru) != 1 || stok_baru < 0) {
                    printf("Input stok baru tidak valid!\n");
                    while (getchar() != '\n');
                    continue;
                }

                printf("Masukkan Harga Baru: ");
                if (scanf("%f", &harga_baru) != 1 || harga_baru < 0) {
                    printf("Input harga baru tidak valid!\n");
                    while (getchar() != '\n');
                    continue;
                }

                strcpy(daftar[ditemukan].kode, kode_baru);
                strcpy(daftar[ditemukan].nama, nama_baru);
                daftar[ditemukan].stok = stok_baru;
                daftar[ditemukan].harga = harga_baru;
                simpanBarangKeBinary(FILE_DB, daftar, jumlah);

                if (stok_baru > stok_lama) {
                    catatLogLaporan(FILE_LAPORAN, "BARANG MASUK", daftar[ditemukan].kode, daftar[ditemukan].nama, stok_baru - stok_lama);
                } else if (stok_baru < stok_lama) {
                    catatLogLaporan(FILE_LAPORAN, "BARANG KELUAR", daftar[ditemukan].kode, daftar[ditemukan].nama, stok_lama - stok_baru);
                }

                printf("Data barang berhasil diperbarui!\n");
            } else {
                printf("Barang dengan kode tersebut tidak ditemukan!\n");
            }
        } else if (pilihan == 4) {
            char kode_cari[20];
            printf("Masukkan Kode Barang yang ingin dihapus: ");
            scanf("%19s", kode_cari);

            int ditemukan = -1;
            for (int i = 0; i < jumlah; i++) {
                if (strcmp(daftar[i].kode, kode_cari) == 0) {
                    ditemukan = i;
                    break;
                }
            }

            if (ditemukan != -1) {
               catatLogLaporan(FILE_LAPORAN, "HAPUS BARANG", daftar[ditemukan].kode, daftar[ditemukan].nama, daftar[ditemukan].stok);
                for (int i = ditemukan; i < jumlah - 1; i++) {
                    daftar[i] = daftar[i + 1];
                }
                jumlah--;
                simpanBarangKeBinary(FILE_DB, daftar, jumlah);
                printf("Barang berhasil dihapus dari inventaris!\n");
            } else {
                printf("Barang dengan kode tersebut tidak ditemukan!\n");
            }

        } else if (pilihan == 5) {
            tulisLaporanStok(FILE_LAPORAN, daftar, jumlah);
            printf("Rekap stok berhasil ditambahkan ke %s\n", FILE_LAPORAN);
        } else if (pilihan != 6) {
            printf("Pilihan tidak tersedia! Masukkan angka 1 sampai 6.\n");
        }
    } while (pilihan != 6);
    printf("Keluar dari menu admin. Terima kasih!\n");
}