//
// HW3: encapsulation, data hiding, delegation. Operator Overloading
// Tracy Zhu
// Medieval Battle Simulator
//
// Purpose: This code simulates warriors battling in medieval times. It reads from a file of warriors with their names, their weapons and their weapon's strengths. All informations are stored in a Warrior class, where a Weapon class is nested. This code keeps track of warriors battling, and outputs battle reults and status of warriors.
//
// Rules: When a warrior battles another, the warrior with a higher strength value wins. The winer will lose the loser's strengths. The loser will lose all his strengths. If warriors with the same strength battle, they will both die.

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;


// Warrior Class
class Warrior {

private:
    //output operator
    friend ostream& operator << (ostream& os, const Warrior& warrior){
        os << "Warrior: " << warrior.name << ", " << warrior.weapon << endl;
        return os;
    }
    
    //private nested Weapon class
    class Weapon {
    private:
        // private variables of Weapon class
        string name;
        int strength;
        
        // output operator
        friend ostream& operator << (ostream& os, const Weapon& weapon){
            os << "weapon: " << weapon.name <<", " << weapon.strength;
            return os;
        }

    public:
        // initialization list
        Weapon(const string& weaponName, int weaponStrength) : name(weaponName), strength(weaponStrength) {}
                
        //sets the strength of the weapon
        void setStrength(int newStrength) {
            strength = newStrength;
        }
        
        //return the strength of the weapon
        int getStrength() const {
            return strength;
        }
    };
    
    // private variables of Warrior class
    string name;
    Weapon weapon;

public:
    // initialization list
    Warrior(const string& warriorName, const string& weaponName, int strength) : name(warriorName), weapon(weaponName, strength){}
    
    //return the name of the warrior
    const string& getName() const { return name;}
    
    //sets the strength of the weapon
    void setStrength(int strength) {
        weapon.setStrength(strength);
    }
    
    //returns the strength of the weapon
    int getStrength() const {
        return weapon.getStrength();
    }
    
    
    // battle is a method used to modify strength of Warriors, and display the result of battle according to two waariors' strengths
    void battle(Warrior& enemy) {
        cout << name << " battles " << enemy.name << endl;
        
        if (getStrength() == 0 && enemy.getStrength() == 0) {
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
            // if both warriors are already dead, then print that they are both already dead
        }
        
        else if (getStrength() == 0 && enemy.getStrength() > 0) {
            cout << "He's dead, " << enemy.name << endl;
            // if one warrior is alive and the other is dead, then print he's dead and the name of the alive warrior
            
        }
        
        else if (getStrength() > 0 && enemy.getStrength() == 0) {
            cout << "He's dead, " << name << endl;
            // same thing but swtich around
        }
        
        else {  // if the strengths of both warriors are not 0
           
            if (getStrength() == enemy.getStrength()) {
                setStrength(0);
                enemy.setStrength(0);
                cout << "Mutual Annihilation: " << name << " and " << enemy.name
                     << " die at each other's hands" << endl;
                //if both warriors have the same strength but are not both dead, then there is a mutual annihilation and both strengths are set to zero.
            }
            
            else if (getStrength() > enemy.getStrength()) {
                setStrength(getStrength() - enemy.getStrength());
                enemy.setStrength(0);
                cout << name << " defeats " << enemy.name << endl;
                //if one warrior is stronger than the other, then one defeats the other. The loser's strength is set to zero and the winner loses the amount of strength the loser had.
            }
            
            else {  // if getStrength() < enemy.getStrength()
                enemy.setStrength(enemy.getStrength() - getStrength());
                setStrength(0);
                cout << enemy.name << " defeats " << name << endl;
                //same thing but switch around
            }
        }

    }
};



// Prototypes
void open_read_file(ifstream& stream);
size_t locate_warrior(const vector <Warrior>& warriors, const string& name);
void display_status(const vector<Warrior>& warriors);



//Main function
int main(){
    ifstream filename;      // create an ifstream variable for reading the file
    open_read_file(filename);
    filename.close();    //close the file
}


//This function attempts to open the warriors.txt file by taking in input ifstream. It outputs an error message if failed to open it, and returns nothing. It also reads through file commands, and prints the outputs accordingly. It returns nothing.
void open_read_file(ifstream& ifs){
    
    vector<Warrior> warriors;    // stores all Warrior structures
    string commands;            // read commands from ifstream
    
    ifs.open("warriors.txt");
    if(!ifs) {
        cerr << "Could not find 'warriors.txt'" << endl;
        exit(1);
    }
    
    while (ifs >> commands){      // read commands and take different actions
        
        if (commands == "Warrior"){        // construct a new Warrior and put it into the vector
            int strength;
            string warrior_name, weapon_name;
            ifs >> warrior_name >> weapon_name >> strength;
            // construct a new Warrior and emplace back into the vector
            warriors.emplace_back(warrior_name, weapon_name, strength);
        }
        
        if (commands == "Status"){     // call display_status to show warriors, their weapons and their strengths
            display_status(warriors);
        }
        
        else if (commands == "Battle") {    //  use battle method in class to put warriors in battle
            string name1, name2;
            ifs >> name1 >> name2;
            
            // find the postions of the 2 warriors involved in the battle
            size_t warrior_index1 = locate_warrior(warriors, name1);
            size_t warrior_index2 = locate_warrior(warriors, name2);
            
            // use battle method in the class
            
            if (warrior_index1 == warriors.size() or warrior_index2 == warriors.size()){
                cerr << "One of the warriors is not found" << endl;
            }
            else{
                warriors[warrior_index1].battle(warriors[warrior_index2]);
            }
            
            
        }
    }
    
}



//This functions finds the position of a warrior. It takes in the vector of Warrior by reference and a string name for warrior, then it returns a type size_t of the position of the warrior.
size_t locate_warrior(const vector <Warrior>& warriors, const string& name){
    for (size_t i = 0; i< warriors.size(); ++i) {
        if (warriors[i].getName() == name) {
            return i;
        }
    }
    return warriors.size();
}



// This function shows the current status of warriors, inlcuding the number of warriors, their names, their weapons and their strength values. It takes the input type vector and returns nothing.
void display_status(const vector<Warrior>& warriors){
    cout << "There are: " << warriors.size() << " warriors" << endl;
    
    for (const Warrior& warrior : warriors)     //for each warrior, print name, weapon and strength
    {
        cout << warrior;
    }
};


