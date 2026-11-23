#ifndef MAIN_H
#define MAIN_H
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

//Fungsi untuk membaca dan menulis data barang ke file binary, mencetak struk belanja, mencatat log laporan, dan menulis laporan stok
void simpanBarangKeBinary(const char *filename, Barang *daftar_barang, int jumlah);
int bacaBarangDariBinary(const char *filename, Barang *daftar_barang);
void cetakStrukTeks(const char *filename, ItemTransaksi *keranjang, int jumlah_item, float total_bayar, float uang_tunai, float kembalian);
void catatLogLaporan(const char *filename, const char *jenis, const char *kode, const char *nama, int qty);
void tulisLaporanStok(const char *filename, Barang *daftar, int jumlah);

//Fungsi untuk menjalankan mode kasir dan admin
void jalankanKasir(void);
void jalankanAdmin(void);



#endif