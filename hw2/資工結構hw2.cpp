#include <iostream>
#include <string>
using namespace std;

class Polynomial; 

class Term { 
    friend Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& p); 
private:
    float coef; // 係數
    int exp;    // 指數
};

class Polynomial { // 多項式類別
    friend ostream& operator<<(ostream& os, const Polynomial& p); 
    friend istream& operator>>(istream& input, Polynomial& p);   
private:
    Term* termArray; // 儲存非零項的陣列
    int capacity;    // 陣列容量
    int terms;       // 非零項數量

public:
    Polynomial(); // 預設建構函式：p(x) = 0
    Polynomial Add(Polynomial poly);  // 多項式相加
    Polynomial Mult(Polynomial poly); // 多項式相乘
    float Eval(float f);              // 多項式求值
    void NewTerm(const float newCoef, const int newExp); // 新增項目
};

// 預設建構函式
Polynomial::Polynomial() : capacity(2), terms(0) {
    this->termArray = new Term[capacity];
}

// 新增項目函式
void Polynomial::NewTerm(const float newCoef, const int newExp) {
    if (this->terms == this->capacity) { // 空間不足時擴展容量
        this->capacity *= 2;
        Term* temp = new Term[this->capacity];
        copy(this->termArray, this->termArray + terms, temp);
        delete[] this->termArray;
        this->termArray = temp;
    }
    this->termArray[this->terms].coef = newCoef; // 設定係數
    this->termArray[this->terms++].exp = newExp; // 設定指數
}

// 多項式相加
Polynomial Polynomial::Add(Polynomial poly) {
    Polynomial res;
    int* loc = new int[poly.terms + this->terms]; // 儲存指數
    float* data = new float[poly.terms + this->terms]; // 儲存係數
    int use_len = 0; // 使用長度

    // 將當前多項式的項加入陣列
    for (int i = 0; i < this->terms; i++) {
        int t = -1; // -1 表示未找到
        for (int j = 0; j < use_len; j++) {
            if (this->termArray[i].exp == loc[j]) {
                t = j;
                break;
            }
        }
        if (t == -1) { // 新的指數
            loc[use_len] = this->termArray[i].exp;
            data[use_len++] = this->termArray[i].coef;
        }
        else {
            data[t] += this->termArray[i].coef; // 相同指數合併係數
        }
    }

    // 處理第二個多項式
    for (int i = 0; i < poly.terms; i++) {
        int t = -1;
        for (int j = 0; j < use_len; j++) {
            if (poly.termArray[i].exp == loc[j]) {
                t = j;
                break;
            }
        }
        if (t == -1) {
            loc[use_len] = poly.termArray[i].exp;
            data[use_len++] = poly.termArray[i].coef;
        }
        else {
            data[t] += poly.termArray[i].coef;
        }
    }

    // 將結果存入 res
    for (int i = 0; i < use_len; i++)
        res.NewTerm(data[i], loc[i]);

    delete[] loc;
    delete[] data;

    return res;
}

// 多項式相乘
Polynomial Polynomial::Mult(Polynomial poly) {
    Polynomial res;
    int* loc = new int[poly.terms * this->terms];
    float* data = new float[poly.terms * this->terms];
    int use_len = 0;

    for (int i = 0; i < this->terms; i++) {
        for (int j = 0; j < poly.terms; j++) {
            float t_coef = this->termArray[i].coef * poly.termArray[j].coef; // 係數相乘
            int t_exp = this->termArray[i].exp + poly.termArray[j].exp;     // 指數相加

            int t = -1;
            for (int k = 0; k < use_len; k++) {
                if (t_exp == loc[k]) {
                    t = k;
                    break;
                }
            }
            if (t == -1) { // 新的項目
                loc[use_len] = t_exp;
                data[use_len++] = t_coef;
            }
            else {
                data[t] += t_coef;
            }
        }
    }

    for (int i = 0; i < use_len; i++)
        res.NewTerm(data[i], loc[i]);

    delete[] loc;
    delete[] data;

    return res;
}

// 多項式求值
float Polynomial::Eval(float f) {
    float res = 0.0f;
    for (int i = 0; i < this->terms; i++) {
        float temp = this->termArray[i].coef;
        for (int j = 0; j < this->termArray[i].exp; j++)
            temp *= f; // 計算次方
        res += temp;
    }
    return res;
}

// 輸入運算子
istream& operator>>(istream& input, Polynomial& p) {
    float coef = 0.0f;
    int exp = 0;
    char ch;
    bool isNegative = false;

    while (true) {
        input >> coef;
        if (isNegative) coef = -coef;
        isNegative = false;

        input.get(ch);
        if (ch == 'x') {
            input.get(ch);
            if (ch == '^') {
                input >> exp;
            }
            else {
                exp = 1;
                input.unget();
            }
        }
        else {
            exp = 0;
            input.unget();
        }

        p.NewTerm(coef, exp);

        input.get(ch);
        if (ch == '\n' || input.eof()) break;
        if (ch == '-') {
            isNegative = true;
        }
        else if (ch != '+') {
            cout << "輸入格式錯誤！請使用正確的多項式格式。\n";
            break;
        }
    }
    return input;
}

// 輸出運算子
ostream& operator<<(ostream& output, const Polynomial& p) {
    for (int i = 0; i < p.terms; i++) {
        if (p.termArray[i].exp == 0) {
            output << (p.termArray[i].coef < 0 ? "" : "+") << p.termArray[i].coef;
        }
        else {
            if (i == 0) {
                output << p.termArray[i].coef << "x^" << p.termArray[i].exp;
            }
            else {
                output << (p.termArray[i].coef < 0 ? "" : "+") << p.termArray[i].coef << "x^" << p.termArray[i].exp;
            }
        }
    }
    return output;
}

int main() {
    cout << "輸入格式 ax^n2+bx^n1+cx^n0+d（若為常數，可省略 x^0）\n";
    Polynomial p1, p2;
    cout << "p1：";
    cin >> p1;
    cout << "p2：";
    cin >> p2;

    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;

    cout << "-----------------------------------------------------------\n";
    cout << "新增項目\n";
    float t_coef = 0.0f;
    int t_exp = 0;
    cout << "請輸入項次的係數：";
    cin >> t_coef;
    cout << "請輸入項次的指數：";
    cin >> t_exp;
    cout << "新增前的多項式 = " << p1 << endl;
    p1.NewTerm(t_coef, t_exp);
    cout << "新增後的多項式 = " << p1 << endl;

    cout << "-----------------------------------------------------------\n";
    cout << "求值\n";
    float f = 0.0f;
    cout << "請輸入要帶入的值：";
    cin >> f;
    cout << "多項式：" << p1 << endl;
    cout << "結果 = " << p1.Eval(f) << endl;

    cout << "-----------------------------------------------------------\n";
    cout << "相加測試\n";
    cout << "(" << p1 << ") + (" << p2 << ") = " << p1.Add(p2) << endl;

    cout << "-----------------------------------------------------------\n";
    cout << "相乘測試\n";
    cout << "(" << p1 << ") * (" << p2 << ") = " << p1.Mult(p2) << endl;

    return 0;
}
