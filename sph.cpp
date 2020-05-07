
#include <iostream>
#include <math.h>

double u0 = 0;
double u1 = 100;
double a = 0.0028;
double r0 = 0.25;

double temp_func(double r, double t){
    
    double sum = 0;
    for (int n = 1; n < 10000; n++)
    {
        double zn = 1.0 / n;

        if (n % 2 == 0)
            zn *= -1;

        double ex = -1 * (n * n * 3.14 * 3.14 * a * a) * t / (r0 * r0);

        double si = sin(n * 3.14 * r / r0) / r; 
        
        sum += zn * exp(ex) * si;
    }
    
    //std::cout << "sum is " << sum << "\n";
    
    return abs(u0 + ((2 * r0) / 3.14) * (u1 -u0) * sum);
}

int main(){

    std::cout.precision(3);
    std::cout.setf(std::ios::fixed);

    for (double radius = 0.01; radius < 0.25; radius += 0.01)
        std::cout << "rad = " << radius << " " << temp_func(radius, 0.01) << "\n";

    std::cout << "--------------------\n";

    for (double radius = 0.01; radius < 0.25; radius += 0.01)
        std::cout << "rad = " << radius << " " << temp_func(radius, 1) << "\n";
std::cout << "--------------------\n";
        for (double radius = 0.01; radius < 0.25; radius += 0.01)
        std::cout << "rad = " << radius << " " << temp_func(radius, 10) << "\n";
std::cout << "--------------------\n";
        for (double radius = 0.01; radius < 0.25; radius += 0.01)
        std::cout << "rad = " << radius << " " << temp_func(radius, 500) << "\n";
std::cout << "--------------------\n";
    for (double radius = 0.01; radius < 0.25; radius += 0.01)
        std::cout << "rad = " << radius << " " << temp_func(radius, 1000) << "\n";

        std::cout << "--------------------\n";
    for (double radius = 0.01; radius < 0.25; radius += 0.01)
        std::cout << "rad = " << radius << " " << temp_func(radius, 3000) << "\n";

    return 0;
}