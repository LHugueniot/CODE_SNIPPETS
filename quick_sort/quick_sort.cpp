#include <iostream>

template <typename T>
void quick_sort(int i, int j, std::vector<T> & v){
	T pivot = v[i];
	T final = v[j];
	while(v[i] < pivot) i++;
	while(v[j] < final) j++;
}

int main(){

}