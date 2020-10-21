#include <iostream>
#include <vector>

void generatePlaneVertexData(std::vector<float> & planeVertices,
    float squareSize, uint planeSize_x, uint planeSize_z,
    float center_x, float center_y, float center_z){

    uint totalVertices = (planeSize_x + 1) * 6 + (planeSize_z + 1) * 6;
    if(totalVertices != planeVertices.size())
        planeVertices.resize(totalVertices);

    float offset_x = planeSize_x * squareSize / 2.f + center_x;
    float offset_z = planeSize_z * squareSize / 2.f + center_z;

    for (uint i = 0 ; i < totalVertices / 2 ; i += 6){
        uint row_col = i / 6;
        planeVertices[i]     = row_col * squareSize - offset_x;
        planeVertices[i + 1] = center_y;
        planeVertices[i + 2] = planeSize_x * squareSize - offset_z;
        planeVertices[i + 3] = row_col * squareSize - offset_x;
        planeVertices[i + 4] = center_y;
        planeVertices[i + 5] = -offset_z;
    }
    for (uint i = totalVertices / 2 ; i < totalVertices ; i += 6) {
        uint row_col = (i - totalVertices / 2 )/ 6;

        planeVertices[i]     = planeSize_z * squareSize - offset_x;
        planeVertices[i + 1] = center_y;
        planeVertices[i + 2] = row_col * squareSize - offset_z;
        planeVertices[i + 3] = -offset_x;
        planeVertices[i + 4] = center_y;
        planeVertices[i + 5] = row_col * squareSize - offset_z;
    }
}

int main(int argc, char const *argv[]){
    std::vector<float> v;
    generatePlaneVertexData(v, 1.f, 10, 10, 0,0,0);
    for (int i = 0; i < v.size() / 3; ++i){
        int idx = i * 3;
        std::cout<<"v "<<v[idx]<<" "<<v[idx + 1]<<" "<<v[idx + 2]<<std::endl;
        /* code */
    }
    std::cout<<"l ";
    for (int i = 0; i < v.size() / 6; ++i){
        int idx = i * 2;
        std::cout<<" "<<idx<<" "<<idx + 1;
        /* code */
    }
    /* code */
    return 0;
}
