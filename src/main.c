#include <stdio.h>
#include <string.h>
#include "../include/admin.h"
#include "../include/kasir.h"

#define FILE_DB_DEFAULT "data/inventaris.bin"

static void tampilkanBantuan(const char *nama_program) {
    printf("Penggunaan:\n");
    printf("  %s --admin [file_db]   Mode admin: kelola stok barang\n", nama_program);
    printf("                         (file_db opsional, default: %s)\n", FILE_DB_DEFAULT);
    printf("  %s --kasir             Mode kasir: transaksi penjualan\n", nama_program);
    printf("  %s --help              Tampilkan bantuan ini\n", nama_program);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Argumen belum diberikan!\n\n");
        tampilkanBantuan(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        tampilkanBantuan(argv[0]);
        return 0;
    }

    if (strcmp(argv[1], "--admin") == 0) {
        if (argc > 3) {
            printf("Terlalu banyak argumen untuk --admin!\n\n");
            tampilkanBantuan(argv[0]);
            return 1;
        }
        const char *file_db = (argc == 3) ? argv[2] : FILE_DB_DEFAULT;
        jalankanAdmin(file_db);
        return 0;
    }

    if (strcmp(argv[1], "--kasir") == 0) {
        if (argc > 2) {
            printf("--kasir tidak menerima argumen tambahan!\n\n");
            tampilkanBantuan(argv[0]);
            return 1;
        }
        jalankanKasir();
        return 0;
    }

    printf("Argumen tidak dikenal: %s\n\n", argv[1]);
    tampilkanBantuan(argv[0]);
    return 1;
}