#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

struct Node {
    int coef;
    int exp;
    Node* link;
};

class Polynomial {
private:
    Node* header;

    void clear() {
        if (!header) return;
        Node* current = header->link;
        while (current != header) {
            Node* temp = current;
            current = current->link;
            delete temp;
        }
        header->link = header;
    }

    void copyFrom(const Polynomial& a) {
        clear();
        Node* currentA = a.header->link;
        Node* last = header;
        while (currentA != a.header) {
            Node* newNode = new Node{ currentA->coef, currentA->exp, header };
            last->link = newNode;
            last = newNode;
            currentA = currentA->link;
        }
    }

public:
    Polynomial() {
        header = new Node{ 0, 0, nullptr };
        header->link = header;
    }

    ~Polynomial() {
        clear();
        delete header;
    }

    Polynomial(const Polynomial& a) : Polynomial() {
        copyFrom(a);
    }

    const Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            copyFrom(a);
        }
        return *this;
    }

    friend istream& operator>>(istream& is, Polynomial& x) {
        x.clear();
        string input;
        getline(is, input);
        stringstream ss(input);
        Node* last = x.header;
        string term;
        while (ss >> term) {
            size_t xPos = term.find("x^");
            int coef = 0, exp = 0;
            if (xPos != string::npos) {
                coef = stoi(term.substr(0, xPos));
                exp = stoi(term.substr(xPos + 2));
            }
            else {
                coef = stoi(term);
                exp = 0;
            }
            Node* newNode = new Node{ coef, exp, x.header };
            last->link = newNode;
            last = newNode;
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        Node* current = x.header->link;
        while (current != x.header) {
            os << current->coef << "x^" << current->exp;
            current = current->link;
            if (current != x.header) os << " + ";
        }
        return os;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* currentA = header->link;
        Node* currentB = b.header->link;
        Node* last = result.header;

        while (currentA != header || currentB != b.header) {
            int coef, exp;
            if (currentA != header && (currentB == b.header || currentA->exp > currentB->exp)) {
                coef = currentA->coef;
                exp = currentA->exp;
                currentA = currentA->link;
            }
            else if (currentB != b.header && (currentA == header || currentA->exp < currentB->exp)) {
                coef = currentB->coef;
                exp = currentB->exp;
                currentB = currentB->link;
            }
            else {
                coef = currentA->coef + currentB->coef;
                exp = currentA->exp;
                currentA = currentA->link;
                currentB = currentB->link;
            }
            if (coef != 0) {
                Node* newNode = new Node{ coef, exp, result.header };
                last->link = newNode;
                last = newNode;
            }
        }
        return result;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Node* currentA = header->link;
        Node* currentB = b.header->link;
        Node* last = result.header;

        while (currentA != header || currentB != b.header) {
            int coef, exp;
            if (currentA != header && (currentB == b.header || currentA->exp > currentB->exp)) {
                coef = currentA->coef;
                exp = currentA->exp;
                currentA = currentA->link;
            }
            else if (currentB != b.header && (currentA == header || currentA->exp < currentB->exp)) {
                coef = -currentB->coef;
                exp = currentB->exp;
                currentB = currentB->link;
            }
            else {
                coef = currentA->coef - currentB->coef;
                exp = currentA->exp;
                currentA = currentA->link;
                currentB = currentB->link;
            }
            if (coef != 0) {
                Node* newNode = new Node{ coef, exp, result.header };
                last->link = newNode;
                last = newNode;
            }
        }
        return result;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        Node* currentA = header->link;
        while (currentA != header) {
            Polynomial temp;
            Node* last = temp.header;
            Node* currentB = b.header->link;
            while (currentB != b.header) {
                Node* newNode = new Node{ currentA->coef * currentB->coef, currentA->exp + currentB->exp, temp.header };
                last->link = newNode;
                last = newNode;
                currentB = currentB->link;
            }
            result = result + temp;
            currentA = currentA->link;
        }
        return result;
    }

    float Evaluate(float x) const {
        float result = 0;
        Node* current = header->link;
        while (current != header) {
            result += current->coef * pow(x, current->exp);
            current = current->link;
        }
        return result;
    }
};

int main() {
    Polynomial p1, p2;
    cout << "輸入第一個多項式: ";
    cin >> p1;
    cout << "第一個多項式: " << p1 << endl;

    cout << "輸入第二個多項式: ";
    cin >> p2;
    cout << "第二個多項式: " << p2 << endl;

    Polynomial sum = p1 + p2;
    cout << "相加: " << sum << endl;

    Polynomial diff = p1 - p2;
    cout << "相減: " << diff << endl;

    Polynomial prod = p1 * p2;
    cout << "相乘: " << prod << endl;

    float x;
    cout << "輸入帶入值";
    cin >> x;
    cout << "結果:" << x << ": " << p1.Evaluate(x) << endl;

    return 0;
}
