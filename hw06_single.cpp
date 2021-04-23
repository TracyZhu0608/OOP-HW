//
//  main.cpp
//  hw06-single
//
//  Created by Tracy Zhu on 3/29/21.
//

#include <iostream>
#include <vector>
using namespace std;

class Noble;
class Warrior {
    // friend funtion prorotype
    friend std::ostream& operator<<(std::ostream& os, const Warrior& rhs);
    
public:
    // constructor
    Warrior(const std::string& name, const double strength);
    
    // getters and setters
    const std::string& getName() const;
    double getStrength() const;
    Noble* getBoss() const;
    void setStrength(const double ratio);
    void setBoss(Noble* newBoss);
    
    bool isHired() const;

    bool runaway();
private:
    std::string name;
    double strength;
    Noble* boss;
};

class Noble {
    // friend funtion prorotype
    friend std::ostream& operator<<(std::ostream& os, const Noble& rhs);
public:
    // constructor
    Noble(const std::string& name);
    
    const std::string& getName() const;
    bool isDead() const;

    void dies();
    bool hire(Warrior& new_warrior);
    bool fire(Warrior& warrior);
    bool removeWarrior(Warrior& warrior);
    void battle(Noble& opponent);
    
private:
    double getArmyStrength() const;
    void setArmyStrength(const double ratio);
    size_t locateWarrior(const Warrior& warrior);

    std::string name;
    bool dead;
    std::vector<Warrior*> army;
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

    cout << "================ Initial Status ===============" << endl;
    cout << jim << endl;
    cout << lance << endl;
    cout << art << endl;
    cout << linus << endl;
    cout << billie;
    cout << "===============================================\n\n";

    cheetah.runaway();
    cout << art << endl;

    art.battle(lance);
    jim.battle(lance);
    linus.battle(billie);
    billie.battle(lance);

    cout << "\n================ Final Status ===============" << endl;
    cout << jim << endl;
    cout << lance << endl;
    cout << art << endl;
    cout << linus << endl;
    cout << billie << endl;
    // Tarzan should be unemployed
    cout << "Tarzan's Hire Status: " << boolalpha << cheetah.isHired() << endl;
    cout << "===============================================\n\n";
}




// constructor
Warrior::Warrior(const string& name, const double strength)
    : name(name), strength(strength), boss(nullptr)
{}

//getters and setters
const string& Warrior::getName() const { return name; }

double Warrior::getStrength() const { return strength; }

Noble* Warrior::getBoss() const { return boss; }  // get the noble boss

void Warrior::setStrength(const double ratio) {   // updates warrior strength using winning ratio
    strength *= (1 - ratio);
}
void Warrior::setBoss(Noble* newBoss) { boss = newBoss; }

bool Warrior::isHired() const { return boss != nullptr; }

bool Warrior::runaway() {
    if (strength == 0) {  // fail if warrior is dead
        cout << name << " is already dead!" << endl;
        return false;
    }
    
    if (boss) {
        cout << name << " flees in terror, abandoning his lord, " << boss->getName() << endl;
        boss->removeWarrior(*this);
        return true;
    }
    
    return false;
}



//constructor
Noble::Noble(const string& name): name(name), dead (false) {};

// getters and setters
const string& Noble::getName() const { return name; }

double Noble::getArmyStrength() const {
    // loop through the vecotor to sum up total army strength
    double total_army_strength = 0;
    for (const Warrior* warrior : army) {
        total_army_strength += warrior->getStrength();
    }
    return total_army_strength;
}

void Noble::setArmyStrength(const double ratio) {
    for (Warrior* curr_warrior : army) {
        // multiply each warrior's strength by (1-losing ratio)
        curr_warrior->setStrength(ratio);
    }
}


bool Noble::isDead() const { return dead; }

void Noble::dies() {
    dead = true;
    setArmyStrength(1);  //set all warriors' strengths to 0, which means the ratio is 1
}


size_t Noble::locateWarrior(const Warrior& warrior) {   //find warrior in the army, return the index
    for (size_t index = 0; index < army.size(); ++index) {
        if (army[index] == &warrior) {
            return index;
        }
    }
    return army.size();
}


bool Noble::hire(Warrior& new_warrior) {
    if (dead) {  // if the noble is dead,fail to hire
        cout << "Noble " << name << " is already dead!" << endl;
        return false;
    }
    
    else if (new_warrior.isHired()) {   //if the warrior is already hired, fail to hire
        cout << "Warrior " << new_warrior.getName() << " is already hired!" << endl;
        return false;
    }
    
    else{ // noble is not dead and warrior is not already hired, then hire warrior
        new_warrior.setBoss(this);
        army.push_back(&new_warrior);
        return true;
    }
}

// attempt to remove a warrior (fire or runaway)
bool Noble::removeWarrior(Warrior& warrior) {
    if (warrior.getBoss() != this) { // if warrior does not belong to the noble
        cout << warrior.getName() << " does not belong to " << name << endl;
        return false;
    }
    else{
        size_t warrior_index = locateWarrior(warrior);
        warrior.setBoss(nullptr);
        for (size_t index = warrior_index; index < army.size()-1; ++index) {
            army[index] = army[index + 1]; //shift the rest of the vector
        }
        army.pop_back();
        return true;
    }
}

bool Noble::fire(Warrior& warrior) {
    if (dead) {   // if noble is dead, fail to fire
        cout << "Noble " << name << " is already dead!" << endl;
        return false;
    }
    
    else if (!warrior.isHired()){ // if warrior is already unemployed, cannot be fired
        cout << "Warrior " << warrior.getName() << " is already unemployed!" << endl;
        return false;
    }
    
    else {
        cout << "You don't work for me anymore " << warrior.getName() << " -- " << name << "." << endl;
        removeWarrior(warrior);
        return true;
    }
}


void Noble::battle(Noble& opponent){
    cout << name << " battles " << opponent.getName() << endl;

    // if one of nobles is dead
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


ostream& operator<<(ostream& os, const Warrior& rhs) {
    os << rhs.name << ": " << rhs.strength << endl;
    return os;
}

ostream& operator<<(ostream& os, const Noble& rhs) {
    os << rhs.name << " has an army of " << rhs.army.size() << endl;
    for (Warrior* warriorPoi : rhs.army) {
        os << "        " << *warriorPoi;
    }
    return os;
}




