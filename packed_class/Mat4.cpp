#include <iostream>

class Mat4{
public:
	Mat4(float * data){

        for(int i = 0; i<4 ; i++)
            col1[i] = data[i];
        for(int i = 4; i<8 ; i++)
            col2[i] = data[i];
        for(int i = 8; i<12 ; i++)
            col3[i] = data[i];
        for(int i = 12; i<16 ; i++)
            col4[i] = data[i];
    }
    float operator [](int i)
    {
        if (i < 4)
            return col1[i];
        else if (i < 8)
            return col2[i];
        else if (i < 12)
            return col3[i];
        else if (i < 16)
            return col4[i];
    }
private:
	float col1[4];
	float col2[4];
	unsigned int size = 16;
	float col3[4];
	float col4[4];
};

int main(){
    float * data = new float[16]{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    Mat4 test_mat(data);
    for(int i = 0; i<16 ; i++)
        (float *)(test_mat[i] + sizeof(float) * 9);
        std::cout<<test_mat[i]<<std::endl;
}