#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// 定数の設定
#include "./common_files/include/common_parameter.h"
#include "./common_files/include/checkAlloc1DDouble.h"
#include "./common_files/include/getFilePath.h"
#include "./common_files/include/setGaussianWave.h"
#include "./common_files/include/set1DEyHz.h"
#include "./common_files/include/set1DEyHz_half_calc.h"
#include "./common_files/include/set2DVecCSV.h"
#include "./common_files/include/set1DDoubleCSV_Column.h"
#include "./common_files/include/fft.h"
#include "./common_files/include/dft.h"
#include "./common_files/include/getPeak.h"
#include "./common_files/include/frequency_analysis.h"

#include "./fdtd1d_gaussian/include/memo_gaussian.h"

int main(void) {

    printf("Gaussian Pulse Excitation.\n");

    time_t start_clock, end_clock;

    start_clock = clock();

    // double dt =get_dt();

    double *exciteWave;

    char *file_name;

    int excite_point=(cells-1)/2;

    int fft_timestep_start=2*gaussianPeaktimePosition+cells;
    int fft_timestep_end=fft_timestep_start+fft_length;

    int calculation_timestep=fft_timestep_end;

    printf("(main) cells=%d\n",cells);
    printf("(main) fft length=%d\n",fft_length);
    printf("(main) calc timestep=%d\n",calculation_timestep);
    printf("(main) gaussian peak=%d\n",gaussianPeaktimePosition);

    // wave initialize
    exciteWave=checkAlloc1DDouble("excite wave",calculation_timestep);

    // 励起波形をgaussian pulseに設定
    exciteWave=setGaussianWave(calculation_timestep);

    // fdtdの計算でey成分の最大値、最小値を求める
    double ey_max=0.0;
    double ey_min=0.0;

    // 1 dimensional fdtd calculation
    const double **ey_t_plane;

    ey_t_plane=set1DEyHz_half_calc(
       cells,
       calculation_timestep,
       exciteWave,
       excite_point,
       &ey_max,
       &ey_min
    );

    set2DVecCSV(
        "./csv_files/",
        "ey_t_plane.csv",
        ey_t_plane,
        cells,
        fft_timestep_end
    );

    // fft calculation , array allocation
    double *fft_array=checkAlloc1DDouble("in main fft alloc",fft_length);

    // 得られたeyの波形から2のべき乗個を採取する。
    for(int time=fft_timestep_start;time<fft_timestep_start+fft_length;time++){

        for(int x=0;x<cells;x++){
            if(x==excite_point){
                fft_array[time-fft_timestep_start]=ey_t_plane[time][x];
            }

        }
    }

    // fftの処理
    // ファイルの名前を付ける。
    file_name=getFilePath(csv_dir,"before_fft_eyt_time",csv_extension);

    // set csv value
    set1DDoubleCSV_Column(fft_array,file_name,fft_length);

    file_name=getFilePath(csv_dir,"after_fft_eyt_freq",csv_extension);

    const double *fft_wave=frequency_analysis(fft_array,file_name,fft_length);

    file_name=getFilePath(csv_dir,"getPeak_of_fft",csv_extension);

    const int *peak;
    peak=getPeak(fft_wave,file_name,fft_length);

    printf("(main) gaussian fft peak df=%d\n",(int)round(fft_length/(2.0*cells)));

    memo_gaussian(peak,&ey_max,&ey_min);

    free(exciteWave);
    free(fft_array);
    free(file_name);

    end_clock = clock();

    printf("(main) processing time=%f(sec)\n", (double)(end_clock-start_clock)/CLOCKS_PER_SEC);

    return 0;
}
