
#include <vector>
#include <iostream>

class data_holder{

private:
    std::vector<std::vector<std::vector<double>>> data;
    int data_size;

public:
    data_holder(int _size){
        data_size = _size;
    }

    int size(){
        return data_size;
    }
};
