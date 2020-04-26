
#include "data_holder.h"

int main()
{
    data_holder<double> data(5, 0);
 
    //data.print();

    data[v3i{0, 0, 0}] = 1.1;
    data[v3i{0, 2, 1}] = 2.3;
    data[v3i{3, 0, 2}] = 3.4;

    data.print();

    return 0;
}