#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// 定数の設定
#include "./common_files/include/common_parameter.h"
#include "./common_files/include/checkAlloc1DDouble.h"
#include "./common_files/include/getFilePath.h"
#include "./common_files/include/setSinWave.h"
#include "./common_files/include/set1DEyHz.h"
#include "./common_files/include/set1DEyHz_half_calc.h"
#include "./common_files/include/set2DVecCSV.h"
#include "./common_files/include/set1DDoubleCSV_Column.h"
#include "./common_files/include/fft.h"
#include "./common_files/include/dft.h"
#include "./common_files/include/frequency_analysis.h"

#include "./fdtd1d_sinwave/include/memo_sinwave.h"

int main(int argc,char **argv) {

    printf("Sin Wave Excitation.\n");

    time_t start_clock, end_clock;

    start_clock = clock();

    printf("argc=%d,argv=%s\n",argc,argv[1]);

    int excite_point=(cells-1)/2;

    int calculation_timestep=fft_length;

    printf("(main) cells=%d\n",cells);
    printf("(main) calculation timestep=%d\n",calculation_timestep);

    const int angular_frequency_num=atoi(argv[1]);    
    
    printf("angular frequency number=%d\n",angular_frequency_num);

    // sin wave setting
    double *exciteWave=setSinWave(angular_frequency_num,calculation_timestep);

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
        fft_length
    );

    memo_sinwave(angular_frequency_num);

    free(exciteWave);

    end_clock = clock();

    printf("(main) processing time=%f(sec)\n", (double)(end_clock-start_clock)/CLOCKS_PER_SEC);

    return 0;
}
