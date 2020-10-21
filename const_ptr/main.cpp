#include <iostream>

int main(int argc, char const *argv[])
{
	float * const val = new float[6]{1,2,3,4,5,6};
	val[0] = 7;

	std::cout<<val[0]<<std::endl;

	//float const val = 0;
	//val = 1;
	/* code */
	return 0;
}