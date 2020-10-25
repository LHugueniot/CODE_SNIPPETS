// std lib includes
#include <ctime>
#include <chrono>
#include <iostream>

// Eigen vector includes
//#include <Eigen/Core>
#include <Eigen/Eigen>

//Time Funcs
using ns=std::chrono::nanoseconds;
using tp=std::chrono::high_resolution_clock::time_point;
constexpr auto tnow=std::chrono::high_resolution_clock::now;

ns elapsed(tp start){
	return std::chrono::duration_cast<ns> (tnow() - start);
}

std::ostream &operator<<(std::ostream &out, const std::chrono::nanoseconds & t){
    out<<t.count()<<"[ns]";
    return out;
}

namespace Eigen{

typedef Eigen::Map<Eigen::Vector3f> Vector3fRef;	

}


int main(int argc, char const *argv[])
{
	tp start;
	ns end;

	start = tnow();
	float test_1[3] = {1,2,3};
    end = elapsed(start);
	std::cout<<"test_1 constr took: "<<end<<std::endl;

	start = tnow();
	Eigen::Vector3f test_2(4,5,6);
    end = elapsed(start);
	std::cout<<"test_2 constr took: "<<end<<std::endl;

	start = tnow();
	Eigen::Vector3fRef test_3(new float [3] {7,8,9});
    end = elapsed(start);
	std::cout<<"test_3 constr took: "<<end<<std::endl;

	start = tnow();
	float * test_4 = new float [3] {10,11,12};
    end = elapsed(start);
	std::cout<<"test_4 constr took: "<<end<<std::endl;


	std::cout<<sizeof(test_1)<<std::endl;
	std::cout<<sizeof(test_2)<<std::endl;
	std::cout<<sizeof(test_3)<<std::endl;
	std::cout<<sizeof(test_4)<<std::endl;

    float a, b, c;

	start = tnow();
	a = test_1[0];
	b = test_1[1];
	c = test_1[2];
    end = elapsed(start);
	std::cout<<"test_1 access took: "<<end<<std::endl;
	std::cout<<a<<b<<c<<std::endl;


	start = tnow();
	a = test_2[0];
	b = test_2[1];
	c = test_2[2];
    end = elapsed(start);
	std::cout<<"test_2 access took: "<<end<<std::endl;
	std::cout<<a<<b<<c<<std::endl;


	start = tnow();
	a = test_3[0];
	b = test_3[1];
	c = test_3[2];
    end = elapsed(start);
	std::cout<<"test_3 access took: "<<end<<std::endl;
	std::cout<<a<<b<<c<<std::endl;


	start = tnow();
	a = test_4[0];
	b = test_4[1];
	c = test_4[2];
    end = elapsed(start);
	std::cout<<"test_4 access took: "<<end<<std::endl;
	std::cout<<a<<b<<c<<std::endl;


	delete[] (&test_3[0]);
	delete[] test_4;
	/* code */
	return 0;
}