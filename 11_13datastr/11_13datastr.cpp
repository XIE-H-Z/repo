#include <iostream>
#include <string>

using namespace std;


template <typename T>
int binarysearch(const T* data, int n, const T& target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (data[mid] == target) {
            return mid;  
        }
        else if (data[mid] < target) {
            left = mid + 1;  
        }
        else {
            right = mid - 1; 
        }
    }
    return -1;  
}


int main() {
    int n, q; 
    char type; 

    cin >> type >> n >> q;

  
    if (type == 'i') {
        int* data = new int[n];
        
        for (int i = 0; i < n; ++i) {
            cin >> data[i];
        }

        
        for (int i = 0; i < q; ++i) {
            int target;
            cin >> target;
            int index = binarysearch(data, n, target);
            cout << index << endl; 
        }

        delete[] data; 
    }
    else if (type == 's') {
        string* data = new string[n];
       
        for (int i = 0; i < n; ++i) {
            cin >> data[i];
        }

        
        for (int i = 0; i < q; ++i) {
            string target;
            cin >> target;
            int index = binarysearch(data, n, target);
            cout << index << endl; 
        }

        delete[] data; 
    }

    return 0;
}
