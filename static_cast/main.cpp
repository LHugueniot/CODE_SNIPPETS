#include <iostream>

class Base {};

class Derived1 : public Base{};
class Derived2 : public Base{};

int main(){
	Base * b = new Derived1;
	Derived1 * d1 = static_cast<Derived1*>(b);

}