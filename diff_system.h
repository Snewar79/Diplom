#pragma once
#include "data_holder.h"
#include <map>
#include <iostream>
#include <eigen3/Eigen/Dense>

using namespace Eigen;
using namespace std;

struct triple
{
    int x;
    int y;
    int z;

    bool operator<(const triple& b) const
    {
        if (x < b.x)
            return true;
        
        if (x > b.x)
            return false;

        if (y < b.y)
            return true;
        
        if (y > b.y)
            return false;

        if (z < b.z)
            return true;
        
        if (z >= b.z)
            return false;

        return false;
    }
};

class diff_system{

private:
    data_holder<float> temp;
    data_holder<bool> exist;
    data_holder<int> type;
    std::map<triple, int> xyz2n;
    std::map<int, triple> n2xyz;
    int active_points;
    double simple_temp;


    double a = 0.32;
    double h = 0.1;
    double tau = 0.1;

    bool init = false;

    int data_size;

public:

    diff_system(int size){
        data_size = size;
        
        temp = data_holder<float>(data_size, 0);
        exist = data_holder<bool>(data_size, false);
        type = data_holder<int>(data_size, 0);
        
        simple_temp = 0;

        for (int x = 1; x < 9; x++)
        for (int y = 1; y < 9; y++)
        for (int z = 1; z < 9; z++)
        {
            temp[x][y][z] = x + y + z;
            exist[x][y][z] = true;
            type[x][y][z] = 1;
            simple_temp += x + y + z;
        }

        temp.print();
        exist.print();
        //type.print();

        math_hash();

    }

    void math_hash()
    {
        int counter = 0;
        for (int x = 0; x < data_size; x++)
        for (int y = 0; y < data_size; y++)
        for (int z = 0; z < data_size; z++){
        if (exist[x][y][z]){
            triple t{x, y, z};
            xyz2n[t] = counter;
            n2xyz[counter] = t;
            counter++;
        }
        }
        active_points = xyz2n.size();
        std::cout << "hash maps is ok, size == " << xyz2n.size() << " " << n2xyz.size() << "\n";
    }

    bool isInit(){
        return init;
    }

    void forward_step(){
        std::cout << "a = " << a << " tau = " << tau << " h = " << h << "\n";
        std::vector<std::vector<float>> matrixA(active_points, std::vector<float>(active_points, 0.0));
        std::vector<float> b(active_points, 0);
        int skip_count = 0;
        //may be not should?

        MatrixXf A(active_points, active_points);
        VectorXf B(active_points);

        A.setConstant(0);

        for (int i = 0; i < active_points; i++){
            skip_count = 0;

            triple current_triple = n2xyz[i];
            std::cout << "[" << i << "]\n";
            std::cout << "triple is " << current_triple.x << " " << current_triple.y << " " << current_triple.z << "\n";

            float current_temp = temp[current_triple.x][current_triple.y][current_triple.z];

            triple xUp, xDown, yUp, yDown, zUp, zDown;
            xUp = current_triple; xUp.x++;
            xDown = current_triple; xDown.x--;

            yUp = current_triple; yUp.y++;
            yDown = current_triple; yDown.y--;

            zUp = current_triple; zUp.z++;
            zDown = current_triple; zDown.z--;
            
            

            // xUp
            if (exist[xUp.x][xUp.y][xUp.z])
            {
                std::cout << "has xUp\n";
                matrixA[i][xyz2n[xUp]] = (a * a * tau) / (h * h); //fixme
                //std::cout << "xyz = " <<  xUp.x << " " << xUp.y << " " << xUp.z << "\n";
                A(i, xyz2n[xUp]) = (a * a * tau) / (h * h);
            }
            else{
                skip_count++;
                //A(i, xyz2n[xUp]) = 400;
            }

            // xDown
            if (exist[xDown.x][xDown.y][xDown.z])
            {
                std::cout << "has xDown\n";
                matrixA[i][xyz2n[xDown]] = (a * a * tau) / (h * h); //fixme
                A(i, xyz2n[xDown]) = (a * a * tau) / (h * h);
            }
            else{
                skip_count++;
                //A(i, xyz2n[xDown]) = 400;
            }

            //yUp
            if (exist[yUp.x][yUp.y][yUp.z])
            {
                std::cout << "has yUp\n";
                matrixA[i][xyz2n[yUp]] = (a * a * tau) / (h * h); //fixme
                A(i, xyz2n[yUp]) = (a * a * tau) / (h * h);
            }
            else{
                skip_count++;
                //A(i, xyz2n[yUp]) = 400;
            }

            //yDown
            if (exist[yDown.x][yDown.y][yDown.z])
            {
                std::cout << "has yDown\n";
                matrixA[i][xyz2n[yDown]] = (a * a * tau) / (h * h); //fixme
                A(i, xyz2n[yDown]) = (a * a * tau) / (h * h);
            }
            else{
                skip_count++;
                //A(i, xyz2n[yDown]) = 400;
            }

            //zUp
            if (exist[zUp.x][zUp.y][zUp.z])
            {
                std::cout << "has zUp" << zUp.x << " " << zUp.y << " " << zUp.z << "\n";
                matrixA[i][xyz2n[zUp]] = (a * a * tau) / (h * h); //fixme
                A(i, xyz2n[zUp]) = (a * a * tau) / (h * h);
            }
            else{
                skip_count++;
                //A(i, xyz2n[zUp]) = 400;
            }

            //zDown
            if (exist[zDown.x][zDown.y][zDown.z])
            {
                std::cout << "has zDown\n";
                matrixA[i][xyz2n[zDown]] = (a * a * tau) / (h * h); //fixme
                A(i, xyz2n[zDown]) = (a * a * tau) / (h * h);
            }
            else{
                skip_count++;
                //A(i, xyz2n[zDown]) = 400;
            }

            matrixA[i][i] = - (1 + ((6 - skip_count) * a * a * tau) / (h * h));
            A(i, i) = matrixA[i][i];
            std::cout << "Set " << i << "|" << i << " by " << matrixA[i][i] << "\n";

            b[i] = -current_temp;
            //b[i] -= (a * a * tau * skip_count * current_temp) / (h * h);
            B(i) = b[i];

        }

        std::cout << "Matrix A is = " << "\n" << A << "\n";
        std::cout << "vector B is = " << "\n" << B << "\n";

        VectorXf x = A.partialPivLu().solve(B);

        //VectorXf x = A.bdcSvd(ComputeThinU | ComputeThinV).solve(B);

        std::cout << "vector X is = " << "\n" << x << "\n";

        std::cout << "skip_count = " << skip_count << "\n";

        for (int i = 0; i < active_points; i ++){
            triple current_triple = n2xyz[i];
            temp[current_triple.x][current_triple.y][current_triple.z] = x(i);

        }

        std::cout << "forward step is called\n";
        std::cout << "ss = " << simple_temp << "\n";
    }

};