#include <iostream>
#include <sstream>
#include <cmath> // 用於 pow 函數
using namespace std;

// 定義 Term 類別，表示多項式中的單一項
class Term {
public:
    float coef; // 係數
    int exp;    // 指數

    Term(float c = 0, int e = 0) : coef(c), exp(e) {}
};

// 定義 Polynomial 類別，表示整個多項式
class Polynomial {
private:
    Term* termArray; // 動態陣列存放多項式的每一項
    int capacity;    // 陣列的容量
    int terms;       // 非零項的數量

    // 擴展動態陣列的容量
    void resize(int newCapacity) {
        Term* newArray = new Term[newCapacity];
        for (int i = 0; i < terms; ++i) {
            newArray[i] = termArray[i];
        }
        delete[] termArray;
        termArray = newArray;
        capacity = newCapacity;
    }

public:
    // 建構子，初始化為空多項式
    Polynomial() : termArray(new Term[1]), capacity(1), terms(0) {}

    // 解構子，釋放動態記憶體
    ~Polynomial() {
        delete[] termArray;
    }

    // 輸入多項式
    friend istream& operator>>(istream& is, Polynomial& poly) {
        string input;
        getline(is, input); // 讀取整行輸入
        stringstream ss(input);
        float coef;
        int exp;
        char x, caret;

        poly.terms = 0; // 重設非零項數量
        while (ss >> coef) {
            if (poly.terms >= poly.capacity) {
                poly.resize(poly.capacity * 2); // 擴展容量
            }

            // 嘗試解析 `x^` 的部分
            if (ss.peek() == 'x') {
                ss >> x; // 跳過 'x'
                if (ss.peek() == '^') {
                    ss >> caret >> exp; // 讀取 '^' 和指數
                }
                else {
                    exp = 1; // 若無 '^'，指數為 1
                }
            }
            else {
                exp = 0; // 若無 'x'，則為常數項
            }
            poly.termArray[poly.terms++] = Term(coef, exp);

            // 跳過可能存在的 '+' 符號
            if (ss.peek() == '+') {
                ss.ignore();
            }
        }
        return is;
    }

    // 輸出多項式
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        for (int i = 0; i < poly.terms; ++i) {
            if (i > 0 && poly.termArray[i].coef > 0) os << " + ";
            os << poly.termArray[i].coef;
            if (poly.termArray[i].exp > 0) {
                os << "x";
                if (poly.termArray[i].exp > 1) {
                    os << "^" << poly.termArray[i].exp;
                }
            }
        }
        return os;
    }

    // 多項式相加
    Polynomial PolynomialADD(const Polynomial& poly) const {
        Polynomial result;
        int i = 0, j = 0;

        // 合併兩個多項式的非零項
        while (i < terms && j < poly.terms) {
            if (termArray[i].exp == poly.termArray[j].exp) {
                float sumCoef = termArray[i].coef + poly.termArray[j].coef;
                if (sumCoef != 0) {
                    result.addTerm(sumCoef, termArray[i].exp);
                }
                ++i; ++j;
            }
            else if (termArray[i].exp > poly.termArray[j].exp) {
                result.addTerm(termArray[i].coef, termArray[i].exp);
                ++i;
            }
            else {
                result.addTerm(poly.termArray[j].coef, poly.termArray[j].exp);
                ++j;
            }
        }

        // 加入剩下的項
        while (i < terms) {
            result.addTerm(termArray[i].coef, termArray[i].exp);
            ++i;
        }
        while (j < poly.terms) {
            result.addTerm(poly.termArray[j].coef, poly.termArray[j].exp);
            ++j;
        }
        return result;
    }

    // 多項式相乘
    Polynomial PolynomialMult(const Polynomial& poly) const {
        Polynomial result;
        for (int i = 0; i < terms; ++i) {
            for (int j = 0; j < poly.terms; ++j) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;
                result.addTerm(newCoef, newExp);
            }
        }
        result.combineLikeTerms(); // 合併同類項
        return result;
    }

    // 計算多項式在某個值的結果
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; ++i) {
            result += termArray[i].coef * pow(x, termArray[i].exp);
        }
        return result;
    }

    // 新增一項到多項式中
    void addTerm(float coef, int exp) {
        for (int i = 0; i < terms; ++i) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                if (termArray[i].coef == 0) { // 移除係數為 0 的項
                    for (int j = i; j < terms - 1; ++j) {
                        termArray[j] = termArray[j + 1];
                    }
                    --terms;
                }
                return;
            }
        }
        if (terms >= capacity) {
            resize(capacity * 2);
        }
        termArray[terms++] = Term(coef, exp);
    }

    // 合併同類項
    void combineLikeTerms() {
        for (int i = 0; i < terms; ++i) {
            for (int j = i + 1; j < terms; ++j) {
                if (termArray[i].exp == termArray[j].exp) {
                    termArray[i].coef += termArray[j].coef;
                    for (int k = j; k < terms - 1; ++k) {
                        termArray[k] = termArray[k + 1];
                    }
                    --terms;
                    --j;
                }
            }
        }
    }
};


int main() {
    Polynomial p1, p2;
    cout << "輸入第一個多項式：";
    cin >> p1;
    cout << "輸入第二個多項式：";
    cin >> p2;

    Polynomial sum = p1.PolynomialADD(p2);
    Polynomial product = p1.PolynomialMult(p2);

    cout << "第一個多項式為: " << p1 << endl;
    cout << "第二個多項式為: " << p2 << endl;
    cout << "多項式相加結果: " << sum << endl;
    cout << "多項式相乘結果: " << product << endl;

    float evalValue;
    cout << "輸入一個值來解第一個多項式: ";
    cin >> evalValue;
    cout << "結果: " << p1.Eval(evalValue) << endl;

    return 0;
}
