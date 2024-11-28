#include <iostream>
#include <sstream>
#include <cmath> // �Ω� pow ���
using namespace std;

// �w�q Term ���O�A��ܦh����������@��
class Term {
public:
    float coef; // �Y��
    int exp;    // ����

    Term(float c = 0, int e = 0) : coef(c), exp(e) {}
};

// �w�q Polynomial ���O�A��ܾ�Ӧh����
class Polynomial {
private:
    Term* termArray; // �ʺA�}�C�s��h�������C�@��
    int capacity;    // �}�C���e�q
    int terms;       // �D�s�����ƶq

    // �X�i�ʺA�}�C���e�q
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
    // �غc�l�A��l�Ƭ��Ŧh����
    Polynomial() : termArray(new Term[1]), capacity(1), terms(0) {}

    // �Ѻc�l�A����ʺA�O����
    ~Polynomial() {
        delete[] termArray;
    }

    // ��J�h����
    friend istream& operator>>(istream& is, Polynomial& poly) {
        string input;
        getline(is, input); // Ū������J
        stringstream ss(input);
        float coef;
        int exp;
        char x, caret;

        poly.terms = 0; // ���]�D�s���ƶq
        while (ss >> coef) {
            if (poly.terms >= poly.capacity) {
                poly.resize(poly.capacity * 2); // �X�i�e�q
            }

            // ���ոѪR `x^` ������
            if (ss.peek() == 'x') {
                ss >> x; // ���L 'x'
                if (ss.peek() == '^') {
                    ss >> caret >> exp; // Ū�� '^' �M����
                }
                else {
                    exp = 1; // �Y�L '^'�A���Ƭ� 1
                }
            }
            else {
                exp = 0; // �Y�L 'x'�A�h���`�ƶ�
            }
            poly.termArray[poly.terms++] = Term(coef, exp);

            // ���L�i��s�b�� '+' �Ÿ�
            if (ss.peek() == '+') {
                ss.ignore();
            }
        }
        return is;
    }

    // ��X�h����
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

    // �h�����ۥ[
    Polynomial PolynomialADD(const Polynomial& poly) const {
        Polynomial result;
        int i = 0, j = 0;

        // �X�֨�Ӧh�������D�s��
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

        // �[�J�ѤU����
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

    // �h�����ۭ�
    Polynomial PolynomialMult(const Polynomial& poly) const {
        Polynomial result;
        for (int i = 0; i < terms; ++i) {
            for (int j = 0; j < poly.terms; ++j) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;
                result.addTerm(newCoef, newExp);
            }
        }
        result.combineLikeTerms(); // �X�֦P����
        return result;
    }

    // �p��h�����b�Y�ӭȪ����G
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; ++i) {
            result += termArray[i].coef * pow(x, termArray[i].exp);
        }
        return result;
    }

    // �s�W�@����h������
    void addTerm(float coef, int exp) {
        for (int i = 0; i < terms; ++i) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                if (termArray[i].coef == 0) { // �����Y�Ƭ� 0 ����
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

    // �X�֦P����
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
    cout << "��J�Ĥ@�Ӧh�����G";
    cin >> p1;
    cout << "��J�ĤG�Ӧh�����G";
    cin >> p2;

    Polynomial sum = p1.PolynomialADD(p2);
    Polynomial product = p1.PolynomialMult(p2);

    cout << "�Ĥ@�Ӧh������: " << p1 << endl;
    cout << "�ĤG�Ӧh������: " << p2 << endl;
    cout << "�h�����ۥ[���G: " << sum << endl;
    cout << "�h�����ۭ����G: " << product << endl;

    float evalValue;
    cout << "��J�@�ӭȨӸѲĤ@�Ӧh����: ";
    cin >> evalValue;
    cout << "���G: " << p1.Eval(evalValue) << endl;

    return 0;
}
