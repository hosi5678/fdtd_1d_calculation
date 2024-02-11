#ifndef A5BC9FB8_A124_4D26_BAF2_82B2A53D26A4
#define A5BC9FB8_A124_4D26_BAF2_82B2A53D26A4

// 円周率
#define pi 3.1415926535897932384626433832795

// 真空の誘電率
#define eps0 8.8541878128e-12

// 真空の透磁率
#define u0 1.25663706212e-6

// 光速
#define light_speed 2.99792458e8

// 次元
#define dimension 1

// Yee Cellの長さ 単位は[m]
#define dx 25e-9
#define time_margin 1.0

#define cu_sigma 64.5e6

#define pml_layer_number 50

// ey成分をfftするときのデータの長さを2のべき乗にする
#define fft_length 16384

// Yeeセルの個数
#define cells 55

// gaussian pulseのpeak の時間軸上の位置
#define gaussianPeaktimePosition 16

// gaussian pulseの半値幅を設定する係数。小さいほど幅が狭い
#define gaussian_alpha 0.05

// gaussian pulseの強度
#define pulseAmplitude 1.0

/* #define angular_frequency_num 455 */
// 152,455,759,1062

#define refractive_index 3.0

// 空気の屈折率
#define air_reflactive_index 1.000292

// ガラスの屈折率
#define glass_reflactive_index 1.55

// fftの周波数成分で採取するpeakの数
#define fft_peak_number 15

#define csv_dir "./csv_files/"

#define timestep_dir "./ey_timestep_cvs/"

#define memo_dir "./memos/"

#define csv_extension ".csv"

#define txt_extension ".txt"

#endif /* A5BC9FB8_A124_4D26_BAF2_82B2A53D26A4 */
