#ifndef STRUCT_H
#define STRUCT_H

typedef struct {
    char kode[20];
    char nama[100];
    int stok;
    float harga;
} Barang;

typedef struct {
    char kode[20];
    int jumlah_beli;
    float subtotal;
} ItemTransaksi;

#endif