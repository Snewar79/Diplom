
#include <vector>
#include <iostream>

#define v3i std::vector<int>

template<class data_type>
class data_holder{
private:
    std::vector<std::vector<std::vector<data_type>>> data;
    int data_size;

public:
    data_holder(int _size, data_type default_value){
        data_size = _size;
        data = std::vector<std::vector<std::vector<data_type>>>(data_size, std::vector<std::vector<data_type>>(data_size, std::vector<data_type>(data_size, default_value)));
    }

    int size(){
        return data_size;
    }

    data_type& operator[](std::vector<int> zyx){
        return data[zyx[0]][zyx[1]][zyx[2]];
    }

    void print(){
        for (int z = 0; z < data_size; z++){
            std::cout << "Layer z = " << z << "\n";
            for (int y = 0; y < data_size; y++){
                for (int x = 0; x < data_size; x++){
                    std::cout << data[x][y][z] << " ";
                }
                std::cout << "\n";
            }
        }
    }
};
