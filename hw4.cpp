//
// hw04: pointer basics
// Tracy Zhu
// Medieval Battle Model
//
// This code simulates battles between Nobles and their armies. A Noble can hire or fire Warriors to form armies, and battle against other Nobles. The army's strength is the combined strengths of all its warriors. During battles, two armis' strengths are compared. The winner of the battle is the one with a greater total strength. The warrirors of the winning Noble decrease their strengths by a ratio of the loser's total strength to the winner's total strength. The loser all dies.

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

// Warrior class
class Warrior{
   friend ostream& operator<<(ostream& os, const Warrior& somewarrior){
       os << somewarrior.name << ": " << somewarrior.strength;
       return os;
   }

public:
    Warrior(const string& name, double strength):
    name(name), strength(strength), employed(false) {}; //Warrior constructor

    bool isHired() const {   //return warrior employment status (true for hired, false for unhired)
        return employed;
    }
    
    void getHired(){        // set employed to true when warrior is hired
        employed = true;
    }

    void getFired() {     // set employed to false when warrior is fired
        employed = false;
    }

    double getStrength() const{return strength;} // strength getter
    const string& getName() const{return name;} // name getter

    
    void updateStrength(const double ratio) {
       // updates strength using winning ratio from battle
       strength *= (1-ratio);
    }
   
  
   
private:
    string name;
    double strength;
    bool employed;
};


//Noble class
class Noble{
   friend ostream& operator<<(ostream& os, const Noble& somenoble){
       os << somenoble.name << " has an army of " << somenoble.army.size() << endl;
       for (const Warrior* curr_warrior : somenoble.army){
           os << "     " << *curr_warrior << endl;
       }
       return os;
   }
    
    
public:
    Noble(const string& name) : name(name), dead(false){}; // Noble constructor

    const string& getName() const{return name;} // name getter

    bool isDead() const{return dead;}   // retrun true for dead, false for alive
    
    void dies(){
        // when noble dies, all the warriors dies
        dead = true;
        for (Warrior* curr_warrior : army){
           curr_warrior -> updateStrength(1);  //set all warrior strength to 0, (ratio is 1)
        }
    }
    
    double getArmyStrength() {
        // loop through the vecotor to calculate total army strength
        double total_army_strength = 0;
        
        for (const Warrior* curr_warrior : army){
            total_army_strength += curr_warrior->getStrength();
        }
        
        return total_army_strength;
    }

    void setArmyStrength(const double ratio){
        // multiply each warrior's strength by (1-losing ratio)
        for (Warrior* curr_warrior : army){
           curr_warrior->updateStrength(ratio);
        }
    }
    
    bool hire(Warrior& new_warrior){
        // if the noble is not dead and warrior is not already hired, then hire warrior
        if (!dead && !new_warrior.isHired()){
            
            army.push_back(&new_warrior);    // add new warrior to the army vector
            new_warrior.getHired();
            return true;
        }
        
        else{    //cannot hire anyone if noble is dead or warrior already hired, return false
            return false;
        }
    }

    
    bool fire(Warrior& fired_warrior){
        // if noble is dead, it cannot fire warriors
        if (dead){
            return false;
        }
        
        else if (!fired_warrior.isHired()){ // if warrior is unemployed, cannot be fired
            return false;
        }
        
        // else, fire the warrior
        else{
            fired_warrior.getFired();
            
            bool shift_vector = false;
            for(size_t index = 0; index<army.size(); ++index){
                // if we found the warrior to remove, set bool to true, and shift rest of the vector over one
                if (army[index] == &fired_warrior){
                    shift_vector = true;
                }
                
                // if it is last item in list, pop it off
                if (index == army.size()-1){
                    army.pop_back();
                    break;
                }
                
                // if not last item: check if bool is true, if so, shift vector to the left
                else if (shift_vector == true){
                    army[index] = army[index+1];
                }
            
            }
            cout << "You don't work for me anymore ";
            cout << fired_warrior.getName() << "! -- " << name << endl;
            return true;
        }
    }


    void battle(Noble& opponent){
        cout << name << " battles " << opponent.getName() << endl;

        // if one noble is dead
        if (dead || opponent.isDead()){
            
            if (dead && opponent.isDead()){       //if both are dead
                cout << "Oh, NO!  They're both dead!  Yuck!" << endl;
            }
            else if (dead){           //if self is dead
                cout << "He's dead, " << opponent.getName() << endl;
            }
            else{            //if opponent is dead
                cout << "He's dead, " << name << endl;
            }
        }
        
        // if both are not dead
        else{
            double thisStren = getArmyStrength();
            double opponentStren = opponent.getArmyStrength();
            
            // if both strengths are equal
            if(thisStren == opponentStren){
                dies();
                opponent.dies();
                cout << "Mutual Annihalation: " << name << " and " << opponent.getName() << "die at each other's hands" << endl;
            }


            // if opponent defeats self
            else if (opponentStren > thisStren){
                double losing_ratio = thisStren / opponentStren;
                opponent.setArmyStrength(losing_ratio);
                dies();
                cout << opponent.getName() << " defeats " << name << endl;
            }


            // if self defeats opponent
            else if (opponentStren < thisStren){
                double losing_ratio = opponentStren / thisStren;
                setArmyStrength(losing_ratio);
                opponent.dies();
                cout << name << " defeats " << opponent.getName() << endl;
            }
                
        }
            
    }

private:
    string name;
    vector<Warrior*> army;
    bool dead;
};

    
    
    
int main() {
    
    Noble art("King Arthur");
    Noble lance("Lancelot du Lac");
    Noble jim("Jim");
    Noble linus("Linus Torvalds");
    Noble billie("Bill Gates");
    
    Warrior cheetah("Tarzan", 10);
    Warrior wizard("Merlin", 15);
    Warrior theGovernator("Conan", 12);
    Warrior nimoy("Spock", 15);
    Warrior lawless("Xena", 20);
    Warrior mrGreen("Hulk", 8);
    Warrior dylan("Hercules", 3);
    
    jim.hire(nimoy);
    lance.hire(theGovernator);
    art.hire(wizard);
    lance.hire(dylan);
    linus.hire(lawless);
    billie.hire(mrGreen);
    art.hire(cheetah);
    
    cout << "==========\n"
         << "Status before all battles, etc.\n";
    cout << jim << endl;
    cout << lance << endl;
    cout << art << endl;
    cout << linus << endl;
    cout << billie << endl;
    cout << "==========\n";
    
    art.fire(cheetah);
    cout << art << endl;
    
    art.battle(lance);
    jim.battle(lance);
    linus.battle(billie);
    billie.battle(lance);
    
    
    cout << "==========\n"
         << "Status after all battles, etc.\n";
    cout << jim << endl;
    cout << lance << endl;
    cout << art << endl;
    cout << linus << endl;
    cout << billie << endl;
    cout << "==========\n";
    
}
