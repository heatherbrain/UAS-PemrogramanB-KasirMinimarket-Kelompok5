#ifndef FILE_IO_H
#define FILE_IO_H

#include "struct.h"

void simpanBarangKeBinary(const char *filename, Barang *daftar_barang, int jumlah);
int bacaBarangDariBinary(const char *filename, Barang *daftar_barang);
void cetakStrukTeks(const char *filename, ItemTransaksi *keranjang, int jumlah_item, float total_bayar, float uang_tunai, float kembalian);
void catatLogLaporan(const char *filename, const char *jenis, const char *kode, const char *nama, int qty);
void tulisLaporanStok(const char *filename, Barang *daftar, int jumlah);
#endif