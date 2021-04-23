
//  polynomial.cpp
//  hw08
//  Tracy Zhu
//

#include "polynomial.h"
using namespace std;

#include <iostream>
#include <vector>
#include <cmath>

// struct constructor
Polynomial::Node::Node(int data, Node* next)
    : data(data), next(next) {}


// defualt constructor; set it to zero
Polynomial::Polynomial() : headptr(new Node()), degree(0) {}

// constructor
Polynomial::Polynomial(const std::vector<int>& data)
    : headptr(nullptr), degree(-1) {
        for (int coefficient : data) {
            add_lowest(coefficient);
        }
    clear_zeroes();
}

// destructor
Polynomial::~Polynomial() { clear_polynomial(headptr); }

// copy constructor
Polynomial::Polynomial(const Polynomial& rhs): headptr(nullptr), degree(0) {
    Node* current = rhs.headptr;
    Node* lhs_prior = nullptr;
    while (current) {
        lhs_prior = add_highest(current->data, lhs_prior);
        current = current->next;
    }
}




// helper function for output operator
void display_result(Polynomial::Node* nodeptr, int degree) {
    if (nodeptr->next) {
        display_result(nodeptr->next, degree + 1);
        if (nodeptr->data){
            cout << " + ";
        }
    }
    if (nodeptr->data) {
        if (nodeptr->data != 1 || degree == 0){
            cout << nodeptr->data;
        }
        
        if (degree > 0) {
            cout << "x";
            if (degree > 1){
                cout << "^" << degree;
            }
        }
    }
    else if (!nodeptr->next){
        cout << 0;
    }
}

// call the helper function to print result
ostream& operator<<(ostream& os, const Polynomial& rhs) {
    display_result(rhs.headptr, 0);
    return os;
}


// == operator
bool operator==(const Polynomial& lhs, const Polynomial& rhs) {
    if (lhs.degree != rhs.degree){
        return false;
    }// if degrees are unequal, return false
    
    Polynomial::Node* lhs_curr = lhs.headptr;
    Polynomial::Node* rhs_curr = rhs.headptr;
    while (lhs_curr) {
        if (lhs_curr->data != rhs_curr->data){
            return false;
        }
        lhs_curr = lhs_curr->next;
        rhs_curr = rhs_curr->next;
    }
    return true;
}

// + operator
Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs) {
    Polynomial result(lhs);
    return result += rhs;
}

// != operator
bool operator!=(const Polynomial& lhs, const Polynomial& rhs) {
    return !(lhs == rhs);
}


// += operator
Polynomial& Polynomial::operator+=(const Polynomial& rhs) {
    Node* lhs = headptr;
    Node* rhs_curr = rhs.headptr;
    Node* lhs_prior = nullptr;
    
    while (lhs) {
        if (!rhs_curr){
            break;
        }
        
        lhs->data += rhs_curr->data;
        if (!lhs->next){
            lhs_prior = lhs;  // save the address
        }
            
        lhs = lhs->next;
        rhs_curr = rhs_curr->next;
    }
    
    while (rhs_curr) {  // if the degree of rhs Polynomial is higher
        lhs_prior = add_highest(rhs_curr->data, lhs_prior);
        rhs_curr = rhs_curr->next;
    }
    clear_zeroes();
    return *this;
}

// = operator
Polynomial& Polynomial::operator=(const Polynomial& rhs) {
    if (this != &rhs) {            // check for self-assigment
        clear_polynomial(headptr);  // delete current Polynomial
        degree = 0;
        Node* current = rhs.headptr;
        Node* lhs_prior = nullptr;
        while (current) {
            lhs_prior = add_highest(current->data, lhs_prior);
            current = current->next;
        }
    }
    return *this;
}






// add the lowest term to the Polynomial
void Polynomial::add_lowest(int data) {
    headptr = new Node(data, headptr);
    ++degree;
}

// add the highest term to the Polynomial, return a poinnter of the newest added term
Polynomial::Node* Polynomial::add_highest(int data, Node* highest) {
    if (!highest) {
        headptr = new Node(data);
        return headptr;
    }
    highest->next = new Node(data);
    ++degree;
    return highest->next;
}

// clear the leading zeroes in the polynomial
void Polynomial::clear_zeroes() {
    Node* current = headptr;
    Node* prior = nullptr;
    while (current->next) {
        if (!current->next->data) {
            if (!prior){
                prior = current; // store the address of the first zero
            }
        }
        
        else {
            // if the coefficient of the current term is not zero, reset the address of the first zero Term
            if (prior){
                prior = nullptr;
            }
        }
        current = current->next;
    }
    if (prior) {  // if there are leading zeroes
        clear_polynomial(prior->next);
        prior->next = nullptr;
    }
}

// evaluate tbe polynomial
int Polynomial::evaluate(int value) const {
    int result = 0;
    int degree = 0;
    Node* current = headptr;
    
    while (current) {
        if (current->data) {
            int term_val = current->data;
            for (int i = 0; i < degree; ++i) {
                term_val *= value;
            }
            result += term_val;
        }
        current = current->next;
        ++degree;
    }
    return result;
}

void Polynomial::clear_polynomial(Node* current) {
    while (current) {
        Node* prior =
            current;  // used to store the address of the term
        current = current->next;
        delete prior;
        --degree;
    }
    current = nullptr;
}
