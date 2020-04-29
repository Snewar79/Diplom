#pragma once
#include "data_holder.h"

class diff_system{

private:
    data_holder<float> temp;
    data_holder<bool> exist;
    data_holder<int> type;

    double a;

    bool init = false;

    int data_size;

public:
    diff_system(int size){
        data_size = size;
        
        temp = data_holder<float>(data_size, 0);
        exist = data_holder<bool>(data_size, false);
        type = data_holder<int>(data_size, 0);

        temp.print();
        exist.print();
        type.print();
    }

    bool isInit(){
        return init;
    }

};