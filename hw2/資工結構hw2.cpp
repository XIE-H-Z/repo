#include <iostream>
#include <string>
using namespace std;

class Polynomial; 

class Term { 
    friend Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& p); 
private:
    float coef; // �Y��
    int exp;    // ����
};

class Polynomial { // �h�������O
    friend ostream& operator<<(ostream& os, const Polynomial& p); 
    friend istream& operator>>(istream& input, Polynomial& p);   
private:
    Term* termArray; // �x�s�D�s�����}�C
    int capacity;    // �}�C�e�q
    int terms;       // �D�s���ƶq

public:
    Polynomial(); // �w�]�غc�禡�Gp(x) = 0
    Polynomial Add(Polynomial poly);  // �h�����ۥ[
    Polynomial Mult(Polynomial poly); // �h�����ۭ�
    float Eval(float f);              // �h�����D��
    void NewTerm(const float newCoef, const int newExp); // �s�W����
};

// �w�]�غc�禡
Polynomial::Polynomial() : capacity(2), terms(0) {
    this->termArray = new Term[capacity];
}

// �s�W���ب禡
void Polynomial::NewTerm(const float newCoef, const int newExp) {
    if (this->terms == this->capacity) { // �Ŷ��������X�i�e�q
        this->capacity *= 2;
        Term* temp = new Term[this->capacity];
        copy(this->termArray, this->termArray + terms, temp);
        delete[] this->termArray;
        this->termArray = temp;
    }
    this->termArray[this->terms].coef = newCoef; // �]�w�Y��
    this->termArray[this->terms++].exp = newExp; // �]�w����
}

// �h�����ۥ[
Polynomial Polynomial::Add(Polynomial poly) {
    Polynomial res;
    int* loc = new int[poly.terms + this->terms]; // �x�s����
    float* data = new float[poly.terms + this->terms]; // �x�s�Y��
    int use_len = 0; // �ϥΪ���

    // �N��e�h���������[�J�}�C
    for (int i = 0; i < this->terms; i++) {
        int t = -1; // -1 ��ܥ����
        for (int j = 0; j < use_len; j++) {
            if (this->termArray[i].exp == loc[j]) {
                t = j;
                break;
            }
        }
        if (t == -1) { // �s������
            loc[use_len] = this->termArray[i].exp;
            data[use_len++] = this->termArray[i].coef;
        }
        else {
            data[t] += this->termArray[i].coef; // �ۦP���ƦX�֫Y��
        }
    }

    // �B�z�ĤG�Ӧh����
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

    // �N���G�s�J res
    for (int i = 0; i < use_len; i++)
        res.NewTerm(data[i], loc[i]);

    delete[] loc;
    delete[] data;

    return res;
}

// �h�����ۭ�
Polynomial Polynomial::Mult(Polynomial poly) {
    Polynomial res;
    int* loc = new int[poly.terms * this->terms];
    float* data = new float[poly.terms * this->terms];
    int use_len = 0;

    for (int i = 0; i < this->terms; i++) {
        for (int j = 0; j < poly.terms; j++) {
            float t_coef = this->termArray[i].coef * poly.termArray[j].coef; // �Y�Ƭۭ�
            int t_exp = this->termArray[i].exp + poly.termArray[j].exp;     // ���Ƭۥ[

            int t = -1;
            for (int k = 0; k < use_len; k++) {
                if (t_exp == loc[k]) {
                    t = k;
                    break;
                }
            }
            if (t == -1) { // �s������
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

// �h�����D��
float Polynomial::Eval(float f) {
    float res = 0.0f;
    for (int i = 0; i < this->terms; i++) {
        float temp = this->termArray[i].coef;
        for (int j = 0; j < this->termArray[i].exp; j++)
            temp *= f; // �p�⦸��
        res += temp;
    }
    return res;
}

// ��J�B��l
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
            cout << "��J�榡���~�I�ШϥΥ��T���h�����榡�C\n";
            break;
        }
    }
    return input;
}

// ��X�B��l
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
    cout << "��J�榡 ax^n2+bx^n1+cx^n0+d�]�Y���`�ơA�i�ٲ� x^0�^\n";
    Polynomial p1, p2;
    cout << "p1�G";
    cin >> p1;
    cout << "p2�G";
    cin >> p2;

    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;

    cout << "-----------------------------------------------------------\n";
    cout << "�s�W����\n";
    float t_coef = 0.0f;
    int t_exp = 0;
    cout << "�п�J�������Y�ơG";
    cin >> t_coef;
    cout << "�п�J���������ơG";
    cin >> t_exp;
    cout << "�s�W�e���h���� = " << p1 << endl;
    p1.NewTerm(t_coef, t_exp);
    cout << "�s�W�᪺�h���� = " << p1 << endl;

    cout << "-----------------------------------------------------------\n";
    cout << "�D��\n";
    float f = 0.0f;
    cout << "�п�J�n�a�J���ȡG";
    cin >> f;
    cout << "�h�����G" << p1 << endl;
    cout << "���G = " << p1.Eval(f) << endl;

    cout << "-----------------------------------------------------------\n";
    cout << "�ۥ[����\n";
    cout << "(" << p1 << ") + (" << p2 << ") = " << p1.Add(p2) << endl;

    cout << "-----------------------------------------------------------\n";
    cout << "�ۭ�����\n";
    cout << "(" << p1 << ") * (" << p2 << ") = " << p1.Mult(p2) << endl;

    return 0;
}
