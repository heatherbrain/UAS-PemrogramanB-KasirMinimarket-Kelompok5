#include <stdio.h>
#include <string.h>
#include "../include/admin.h"
#include "../include/kasir.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Gunakan argumen yang benar!\n");
        printf("Contoh:\n");
        printf("  %s --admin\n", argv[0]);
        printf("  %s --kasir\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--admin") == 0) {
        jalankanAdmin();
    } else if (strcmp(argv[1], "--kasir") == 0) {
        jalankanKasir();
    } else {
        printf("Argumen tidak dikenal: %s\n", argv[1]);
        printf("Gunakan --admin atau --kasir.\n");
    }

    return 0;
}