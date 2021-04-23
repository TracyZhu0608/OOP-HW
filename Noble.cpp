//
//  Noble.cpp
//  HW07: inheritance
//

//
#include "Noble.h"
#include "Protector.h"

#include <string>
#include <iostream>
#include <vector>
using namespace std;

namespace WarriorCraft {

    // constructor
    Noble::Noble(const string& name) : name(name), alive(true) {}

    // getters and setters
    const string& Noble::getName() const { return name; }
    void Noble::changeAliveStatus() { alive = !alive; }
    bool Noble::getAliveStatus() const { return alive; }

    // battle betweem Nobles
    void Noble::battle(Noble& enemy) {
        cout << name << " battles " << enemy.name << endl;
        
        
        if (!alive && !enemy.alive) {  // if both nobles are dead
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
        }
        
        // if one of nobles is alive, call the defend method for alive noble
        else if (!alive && enemy.alive) {
            enemy.defend();
            cout << "He's dead, " << enemy.name << endl;
        }
        
        else if (alive && !enemy.alive) {
            cout << "He's dead, " << name << endl;
        }
        
        else {  // if both Nobles are alive
            double self_strength = getStrength();
            double enemy_strength = enemy.getStrength();
            defend();
            enemy.defend();
            
            // determine which Noble to die
            if (self_strength == enemy_strength) {   //both die
                die();
                enemy.die();
                cout << "Mutual Annihilation: " << name << " and " << enemy.name
                     << " die at each other's hands" << endl;
            }
            
            else if (self_strength > enemy_strength) {
                updateStrength(self_strength, enemy_strength);
                enemy.die();
                cout << name << " defeats " << enemy.name << endl;
            }
            
            else {  // self_strength < enemy_strength
                enemy.updateStrength(enemy_strength, self_strength);
                die();
                cout << enemy.name << " defeats " << name << endl;
            }
        }
    }

    void Noble::die() {  // change the status and set strength to 0
        changeAliveStatus();
        updateStrength(1, 1);
    }




// Lord class
    Lord::Lord(const std::string& name) : Noble(name) {}

    // return the total strengths of all the protectors in the army
    double Lord::getStrength() const {
        double total_strength = 0;
        for (const Protector* curr_ptr : army) {
            total_strength += curr_ptr->getStrength();
        }
        return total_strength;
    }

    // multiply the strength of each protector with the ratio
    void Lord::updateStrength(double winner_strength, double loser_strength) {
        for (Protector* curr_ptr : army) {
            double updatedStrength =  curr_ptr->getStrength() * (winner_strength - loser_strength) / winner_strength;
            // current strength * the ratio
            curr_ptr->setStrength(updatedStrength);
        }
    }

    // call defend for every protector
    void Lord::defend() const {
        for (Protector* curr_ptr : army) {
            curr_ptr->defend();
        }
    }

    // hire a protector
    bool Lord::hires(Protector& protector) {
        if (!getAliveStatus()) {
            cout << "Lord's dead. Cannot hire any protector." << endl;
            return false;  // fail if Noble is dead
        }
        if (protector.getLord()) {  // if protector has a lord already
            cout << "Protector " << protector.getName() << " is already hired!" << endl;
            return false;  // fail if Warrior is already hired
        }
        // else hire protector
        protector.setLord(this);
        army.push_back(&protector);
        return true;
    }

    // fire a protector
    bool Lord::fire(Protector& protector) {
        if (!getAliveStatus()) {   // fail if Lord is dead
            cout << "Lord's dead. Cannot fire any protector." << endl;
            return false;
        }
        if (remove(protector)) {
            cout << "You don't work for me anymore " << protector.getName()
                 << "! -- " << getName() << "." << endl;
            return true;
        }
        return false;  // fail if the protector is not removed
    }

    // remove a protector (fire or runaway)
    bool Lord::remove(Protector& someprotector) {
        
        bool shift_vector = false;
        for(size_t index = 0; index<army.size(); ++index){
            // if we found the warrior to remove, set bool to true, and shift rest of the vector over one
            if (army[index] == &someprotector){
                shift_vector = true;
            }
            
            // check if bool is true
            if (shift_vector == true){
                if (index == army.size()-1){       // if it is last item in list, pop it off
                    army.pop_back();
                    break;
                }
                army[index] = army[index+1];   //shift vector to the left
            }

        }

        if (shift_vector == true){
            cout << someprotector.getName() << " is not hired by " << getName() << "!" << endl;
        }

        return true;
        
    }





// PersonWithStrengthToFight
    PersonWithStrengthToFight::PersonWithStrengthToFight(const std::string& name, double strength)
        : Noble(name), strength(strength) {}

    // getter and setter
    double PersonWithStrengthToFight::getStrength() const { return strength; }
    void PersonWithStrengthToFight::updateStrength(double winner_strength, double loser_strength) {
        strength -= loser_strength;
    }

    void PersonWithStrengthToFight::defend() const { cout << "Ugh!" << endl; }

}  // namespace WarriorCraft
