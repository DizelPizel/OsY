#include <math.h>
#include "mylib.h"
float Square(float a,float b){
    return a*b;
}

float Derivative(float A,float deltaX){
    return (cos(A+deltaX)-cos(A))/deltaX;
}
