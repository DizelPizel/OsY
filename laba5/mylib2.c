#include <math.h>
#include "mylib.h"
float Square(float a,float b){
    return a*b*0.5;
}


float Derivative(float A,float deltaX){
    return (cos(A+deltaX)-cos(A-deltaX))/(2*deltaX);
}
