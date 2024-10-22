#include <iostream>

using namespace std;

const int maxm = 1000; // m的最大值
const int maxn = 1000; // n的最大值
int arr[maxm + 1][maxn + 1]; // 使用二維陣列來記憶化

// 初始化二維陣列
void a() {
    for (int i = 0; i <= maxm; ++i) {
        for (int j = 0; j <= maxn; ++j) {
            arr[i][j] = -1;  // -1表示該位置尚未計算過
        }
    }
}

int ack(int m, int n) {
    if (arr[m][n] != -1) {
        return arr[m][n]; // 如果該值已經計算過，直接返回
    }

    if (m == 0) {
        arr[m][n] = n + 1;
    }
    else if (m > 0 && n == 0) {
        arr[m][n] = ack(m - 1, 1);
    }
    else if (m > 0 && n > 0) {
        arr[m][n] = ack(m - 1, ack(m, n - 1));
    }

    return arr[m][n];
}

int main() {
    int m, n;
    cout << "請輸入m和n的值: ";
    cin >> m >> n;
    // 初始化記憶化陣列
    a();
    int result = ack(m, n);
    cout << "Ackermann(" << m << ", " << n << ") = " << result << endl;


    return 0;
}
