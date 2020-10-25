#include <iostream>

struct Obj{
	Obj(int _a) : 
	a(_a){}
	int a;
};

int main(){
	Obj * my_obj = new Obj(5);
	std::cout<<my_obj->a<<std::endl;
	std::cout<<(*my_obj).a<<std::endl;

	Obj my_obj2(6);
	std::cout<<my_obj2.a<<std::endl;
	std::cout<<(*&my_obj2).a<<std::endl;
	//std::cout<<(&*my_obj2).a<<std::endl;
	std::cout<<(&my_obj2)->a<<std::endl;

	Obj & my_obj3 = my_obj2;
	std::cout<<my_obj3.a<<std::endl;
	std::cout<<(*&my_obj3).a<<std::endl;
	//std::cout<<(&*my_obj2).a<<std::endl;
	std::cout<<(&my_obj3)->a<<std::endl;
}