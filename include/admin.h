#ifndef ADMIN_H
#define ADMIN_H

#include "struct.h"

/* Menu utama admin. file_db = lokasi file binary database barang. */
void jalankanAdmin(const char *file_db);

/* Fitur-fitur admin (dipanggil dari menu) */
void tampilkanDaftarBarang(const Barang *daftar, int jumlah);
void tambahBarang(Barang *daftar, int *jumlah, const char *file_db);
void editBarang(Barang *daftar, int jumlah, const char *file_db);
void hapusBarang(Barang *daftar, int *jumlah, const char *file_db);

/* Pass by value: menerima salinan Barang, data asli tidak berubah */
float hitungNilaiStok(Barang b);

#endif