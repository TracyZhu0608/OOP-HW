//
//  polynomial.h
//  hw08
//  Tracy Zhu
//

#ifndef polynomial_h
#define polynomial_h

#include <iostream>
#include <vector>
#include <cmath>


class Polynomial {
    struct Node;

    // non-member friend functions
    friend void display_result(Polynomial::Node* nodeptr, int degree);
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& rhs);
    friend bool operator==(const Polynomial& lhs, const Polynomial& rhs);

public:
    // constructors
    Polynomial();
    Polynomial(const std::vector<int>& data);
    Polynomial(const Polynomial& rhs);

    // member operators
    Polynomial& operator+=(const Polynomial& rhs);
    Polynomial& operator=(const Polynomial& rhs);

    void add_lowest(int data);
    Polynomial::Node* add_highest(int data, Node* highest);
    void clear_zeroes();
    void clear_polynomial(Node* current);

    // evaluation
    int evaluate(int value) const;

    // destructor
    ~Polynomial();

private:
    // nested Term struct
    struct Node {
        // constructor
        Node(int data = 0, Node* next = nullptr);

        // public variables
        int data;
        Node* next;
    };

    Node* headptr;
    int degree;
};

// non-member non-friend operators
Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs);
bool operator!=(const Polynomial& lhs, const Polynomial& rhs);


#endif /* polynomial_h */
