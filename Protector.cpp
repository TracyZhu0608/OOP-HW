//
//  Protector.cpp
//  HW07:inheritance
//

#include "Protector.h"
#include "Noble.h"

#include <vector>
#include <string>
#include <iostream>
using namespace std;

namespace WarriorCraft {

    //constructor
    Protector::Protector(const string& name, double strength)
        : name(name), strength(strength), lord(nullptr) {}

    // getters and setters
    const string& Protector::getName() const { return name; }
    double Protector::getStrength() const { return strength; }
    void Protector::setStrength(double theStrength) { strength = theStrength; }
    void Protector::setLord(Lord* lordptr) { lord = lordptr; }
    Lord* Protector::getLord() const { return lord; }

    //runaway
    bool Protector::runaway() {
        if (strength == 0) {  // fail if protector is dead
            cout << name << " is already dead!" << endl;
            return false;
        }
        
        if (!lord) {  // fail if lord is a nullptr
            cout << name << " doesn't have a lord!";
            return false;
        }
        
        string lordname = lord->getName();
        if (lord->remove(*this)) {
            cout << name << " flees in terror, abandoning his lord, "<< lordname << endl;
            return true;
        }
        return false;
    }

    //defend
    void Warrior::defend() const {
        cout << getName() << " says: Take that in the name of my lord, " << getLord()->getName() << endl;
    }

    // Wizard
    void Wizard::defend() const { cout << "POOF!" << endl; }

    // Archer
    void Archer::defend() const {
        cout << "TWANG! ";
        Warrior::defend();
    }

    // Swardman
    void Swordsman::defend() const {
        cout << "CLANG! ";
        Warrior::defend();
    }
}  // namespace WarriorCraft
