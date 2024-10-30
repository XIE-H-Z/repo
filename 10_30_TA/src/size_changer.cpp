#include <iostream>
using namespace std;
void print_arr(int* arr, int size) {
	for (int i = 0; i < size; i++)
		cout << arr[i] << " ";
	cout << endl;
}
void change_arr(int*& arr, int size, int new_size) {
	int* new_arr = new int[new_size];
	copy(arr, arr + size, new_arr);
	for (int i = size; i < new_size; i++)
		new_arr[i] = i + 1;
	arr = new_arr;
}
int main() {
	int size = 3;
	int* arr = new int[size];
	for (int i = 0; i < size; i++) {
		arr[i] = i + 1;
	}
	cout << "一開始的陣列：" << endl;
	print_arr(arr, size);
	int new_size = size * 2;
	cout << "擴充後的陣列" << endl;
	change_arr(arr, size, new_size);
	print_arr(arr, new_size);
	return 0;
}