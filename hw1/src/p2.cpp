#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;
void powerset(char* set, int size) {
    unsigned int power_set_size = pow(2, size); // 計算冪集大小

    // 遍歷所有可能的子集
    for (unsigned int counter = 0; counter < power_set_size; counter++) {
        cout << "{ ";
        // 檢查每一位
        for (int j = 0; j < size; j++) {
            // 如果當前位是1，則包括這個元素
            if (counter & (1 << j)) {
                cout << set[j] << " ";
            }
        }
        cout << "}" << endl; 
    }
}

int main() {
    char set[100]; // 假設最大元素數量為 100
    cout << "輸入S的內容: ";
    cin >> set; 
    int size = strlen(set); // 取得集合大小
    cout << "S可能的組合為:\n";
    powerset(set, size); 

    return 0;
}
