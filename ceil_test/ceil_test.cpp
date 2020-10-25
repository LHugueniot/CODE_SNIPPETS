#include <iostream>
#include <cmath>
int main()
{
	int sizeOfFlock = 2000;
    int GRID_SIZE = std::ceil((float)sizeOfFlock/1024.f);
    int BLOCK_SIZE = 1024;

    std::cout<<"GRID_SIZE: "<<GRID_SIZE<<std::endl;
    std::cout<<"BLOCK_SIZE: "<<BLOCK_SIZE<<std::endl;
}