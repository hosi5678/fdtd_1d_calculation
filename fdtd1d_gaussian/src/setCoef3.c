#include <stdio.h>
#include <stdlib.h>

#include "../../common_include/common_parameter.h"
#include "../../common_include/setCoef3.h"

double *setCoef3(double *eps,double *sigma,int length){

    double *coef;
    double dt;

    dt=(dx/light_speed)*time_margin;

    coef=(double *)calloc(length,sizeof(double));

    if(NULL==coef){
        printf("can't alloc error coef1.\n");
        exit(1);
    }

    if(dt==dx/light_speed){
        // printf("margin error.\n");
        // exit(1);
    }

    for(int x=0;x<length;x++){
        coef[x]=(2.0*dt)/((2.0*eps[x]+sigma[x]*dt));
    }

    // printf("coef3 length=%d\n",length);
    // for(int x=0;x<length;x++){
    //     printf("coef3[%d]=%f\n",x,coef[x]);
    // }

    return coef;
}
