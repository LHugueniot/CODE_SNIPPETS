#include <iostream>

//Eigen includes
#include <Eigen/Eigen>
#include <Eigen/Core>
#include <Eigen/Geometry>

int main(int argc, char const *argv[])
{
	size_t numberOfMat4s = 1024;
	Eigen::Matrix4f * mat4Buffer = new Eigen::Matrix4f [numberOfMat4s];
	for (int i = 0; i < numberOfMat4s; ++i)
		for (int j = 0; j < 16; ++j)
		{
			mat4Buffer[i].data()[j] = i * 16 + j;
		}
	for (int i = 0; i < 16 * numberOfMat4s; ++i)
	{
		std::cout<<mat4Buffer[0].data()[i]<<std::endl;
	}
	delete[] mat4Buffer;
	return 0;
}