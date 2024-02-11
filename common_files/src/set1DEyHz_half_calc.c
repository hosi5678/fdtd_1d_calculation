#include <stdio.h>
#include <stdlib.h>

#include "../include/common_parameter.h"
#include "../include/init2DdoublePlane.h"
#include "../include/symmetryCheck.h"
#include "../include/antisymmetryCheck.h"
#include "../include/checkAlloc1DDouble.h"
#include "../include/setEps.h"
#include "../include/setSigma.h"
#include "../include/setCoef1.h"
#include "../include/setCoef2.h"
#include "../include/setCoef3.h"
#include "../include/getFilePath.h"
#include "../include/get_dt.h"
#include "../include/set1DDoubleCSV_Column.h"
#include "../include/set_vec_timestep_csv.h"
#include "../include/set1DEyHz_half_calc.h"

const double *set1DEyHz_half_calc(
    int x_length,
    int time_length,
    double *src_J,
    int excite_point,
    double *ey_max,
    double *ey_min,
    double *hz_max,
    double *hz_min
) {

    double *sigma,*eps;
    double *coef1,*coef2,*coef3;

    double *ey,*hz;
    double dt=get_dt();

    eps=setEps(x_length);
    sigma=setSigma(x_length);

    coef1=setCoef1(eps,sigma,dt,x_length);
    coef2=setCoef2(eps,sigma,dt,x_length);
    coef3=setCoef3(eps,sigma,dt,x_length);

        // ey initialize
    ey=checkAlloc1DDouble("ey calloc",x_length);

    // hz initialize
    hz=checkAlloc1DDouble("hz calloc",x_length-1);

    double coef4=dt/(u0*dx);

    double **ey_t_plane=init2DdoublePlane(time_length,x_length);

    double **hz_t_plane=init2DdoublePlane(time_length,x_length-1);

    double *ey_t=checkAlloc1DDouble("ey_t alloc.",time_length);

    for (int time=0; time < time_length; time++) {

        // eyの値の片側(励起点より左)を求める
        for ( int x = 1; x <= excite_point ; x++ ) {
            ey[x]=coef1[x]*ey[x]-coef2[x]*(hz[x]-hz[x-1]);
        }

        // 励起点にgaussian波形を与える
        int x=excite_point;
        ey[x]-=coef3[x]*src_J[time];

        // 励起点で左右対称なので、左側を右側にコピーする。
        for (int x = excite_point+1 ; x < x_length-1 ; x++ ) {
            ey[x]=ey[x_length-1-x];
        }

        // hzの値の片側(励起点より左)を求める
        for(int x=0;x<(x_length-1)/2;x++){
            hz[x]=hz[x]-coef4*(ey[x+1]-ey[x]);
        }

        // 励起点で左右反対称なので、左側のマイナスを右側にコピーする。
        for(int x=(x_length-1)/2;x<x_length-1;x++){
            hz[x]=-1.0*hz[x_length-2-x];
        }

        // eyの最大値、最小値を求める
        for(int x=0;x<x_length;x++){
            ey_t_plane[time][x]=ey[x];
        }

        for(int x=0;x<x_length-1;x++) {
            hz_t_plane[time][x]=hz[x];
        }

        ey_t[time]=ey[x];

        // symmetryCheck(ey,x_length,time);
        // antisymmetryCheck(hz,x_length-1,time);

        set_vec_timestep_csv(
            "./ey_timestep_csvs/",
            "ey_timestep_",
            ey,
            x_length,
            time
        );

        set_vec_timestep_csv(
            "./hz_timestep_csvs/",
            "hz_timestep_",
            hz,
            x_length-1,
            time
        );



    } // time-loop

    // timestep数 600までのey,hzの値の最大値、最小値を求める。

    for(int time=0;time<600;time++){
        // 電界成分
        for(int x=0;x<x_length;x++){
            if(ey_t_plane[time][x]>*ey_max) *ey_max=ey_t_plane[time][x];
            if(*ey_min>ey_t_plane[time][x]) *ey_min=ey_t_plane[time][x];
        }
        // 磁界成分
        for(int x=0;x<x_length-1;x++){
            if(hz_t_plane[time][x]>*hz_max) *hz_max=hz_t_plane[time][x];
            if(*hz_min>hz_t_plane[time][x]) *hz_min=hz_t_plane[time][x];
        }

    }

    // pythonのheatmapに出力したいので、1.1倍にして表示する。
    printf("(ey max)(x1.1)  ey max=%.15f\n",*ey_max*1.1);
    printf("(ey min)(x1.1)  ey min=%.15f\n",*ey_min*1.1);

    printf("(hz max)(x1.1)  hz max=%.15f\n",*hz_max*1.1);
    printf("(hz min)(x1.1)  hz min=%.15f\n",*hz_min*1.1);


    double *ey_range=checkAlloc1DDouble("in ey range.",2);
    ey_range[0]=*ey_max*1.1;
    ey_range[1]=*ey_min*1.1;

    double *hz_range=checkAlloc1DDouble("in hz range.",2);
    hz_range[0]=*hz_max*1.1;
    hz_range[1]=*hz_min*1.1;

    char *file_path;
    file_path=getFilePath(csv_dir,"ey_range",csv_extension);

    set1DDoubleCSV_Column(ey_range,file_path,2);

    file_path=getFilePath(csv_dir,"hz_range",csv_extension);

    set1DDoubleCSV_Column(hz_range,file_path,2);

    free(eps);
    free(sigma);
    free(coef1);
    free(coef2);
    free(coef3);
    free(hz);
    free(ey);

    free(ey_range);
    free(hz_range);

    return (const double *)ey_t;

}
