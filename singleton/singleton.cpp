#include <iostream>

class Kernel{
public:
	static Kernel & get(){
			return instance;
	}
private:
	Kernel(){}
	static Kernel instance;
};

Kernel Kernel::instance;

int main(){
	std::cout<<"Begin main"<<std::endl;
	Kernel::get();
}


