#ifndef STRUCT_H
#define STRUCT_H

#define MAX_KODE 10
#define MAX_NAMA 50

// Struct untuk data barang / inventaris
typedef struct {
    char kode[MAX_KODE];
    char nama[MAX_NAMA];
    int stok;
    float harga;
} Barang;

// Struct untuk item transaksi kasir (keranjang belanja)
typedef struct {
    char kode[MAX_KODE];
    int jumlah_beli;
    float subtotal;
} ItemTransaksi;

#endif