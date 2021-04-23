
// hw05: dynamic memory
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
    
    size_t getArmySize() const { return army.size(); }

    bool isDead() const{return dead;}   // retrun true for dead, false for alive
    
    void dies(){
        // when noble dies, all the warriors dies
        dead = true;
        for (Warrior* curr_warrior : army){
           curr_warrior -> updateStrength(1);  //set all warrior strength to 0, (ratio is 1)
        }
    }
    
    const double getArmyStrength() {
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
                cout << "You don't work for me anymore ";
                cout << fired_warrior.getName() << "! -- " << name << endl;
            }
            
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

    
    
    
// Prototypes
void openFile(ifstream& stream);
Warrior* checkWarrior(const string& name, const vector<Warrior*>& warriors);
Noble* checkNoble(const string& name, const vector<Noble*>& nobles);
void clearAll(vector<Noble*>& nobles, vector<Warrior*>& warriors);
void displayStatus(vector<Noble*> const& nobles, vector<Warrior*> const& warriors);

int main() {
    ifstream stream;
    openFile(stream);  // open file

    string command;
    vector<Noble*> nobles;
    vector<Warrior*> warriors;

    // identify the command and take different actions accordingly
    while (stream >> command) {
        if (command == "Noble") {
            string name;
            stream >> name;
            
            if (checkNoble(name, nobles)) {  // if Noble alredy exists
                cout << "Noble " << name << " already exists" << endl;
            }
            
            else {  // if not
                nobles.push_back(new Noble(name));
            }
        }
        
        else if (command == "Warrior") {
            string name;
            int strength;
            stream >> name >> strength;
            if (checkWarrior(name,warriors)) {  // if Warrior already exists
                cout << "Warrior " << name << " already exists" << endl;
            }
            
            else { //if not
                warriors.push_back(new Warrior(name, strength));
            }
        }
        
        else if (command == "Status") {
            displayStatus(nobles, warriors);
        }
        
        else if (command == "Hire") {
            string noble, warrior;
            stream >> noble >> warrior;
            Noble* noblePoi = checkNoble(noble, nobles);
            Warrior* warriorPoi = checkWarrior(warrior, warriors);
            
            // if noble and warrior both exist, and if warrior is unemployed
            if ((noblePoi) && (warriorPoi) && !warriorPoi -> isHired()) {
                noblePoi -> hire(*warriorPoi);  //then, hire the warrior
            }
            
            else {
                if (!noblePoi) { // if noble does not exist
                    cout << "Error: " << noble << " is an unkown noble " << endl;
                }
                
                else if (!warriorPoi) {  // if warrior does not exist
                    cout << "Error: noble " << noble << " is attempting to hire an unkown warrior " << warrior << endl;
                }
                
                else{ // if the warrior is already employed
                    cout << "Error: noble " << noble << " is attempting to hire warrior " << warrior << " who is already employed" << endl;
                }
            }
        }
        
        else if (command == "Fire") {
            string noble, warrior;
            stream >> noble >> warrior;
            
            Noble* noblePoi = checkNoble(noble, nobles);
            Warrior* warriorPoi = checkWarrior(warrior, warriors);
            
            // if noble and warrior both exist, and if warrior is employed
            if ((noblePoi) && (warriorPoi) && warriorPoi -> isHired()) {
                noblePoi->fire(*warriorPoi);   // then, fire the warrior
            }
            
            else {
                if (!noblePoi) {  // if noble does not exist
                    cout << "Error: " << noble << " is an unkown noble " << endl;
                }
                
                else if (!warriorPoi) {   // if warrior does not exist
                    cout << "Error: noble " << noble << "is attempting to fire an unkown warrior " << warrior << endl;
                }
                
                else{ // if warrior is already unemployed
                    cout << "Error: noble " << noble << "is attempting to fire warrior " << warrior << "who is not employed" << endl;
                }
            }
        }
        
        
        else if (command == "Battle") {
            string name1, name2;
            stream >> name1 >> name2;
            Noble* noblePoi1 = checkNoble(name1, nobles);
            Noble* noblePoi2 = checkNoble(name2, nobles);
            
            //if nobles both exist
            if ((noblePoi1) && (noblePoi2)) {
                noblePoi1->battle(*noblePoi2); // then, battle
            }
            
            else {
                if (!noblePoi1) {  // if noble1 does not exist
                    cout << "Error: " << name1 << " is an unkown noble " << endl;
                }
                
                if (!noblePoi2) {// if nobe2 does not exist
                    cout << "Error: " << name2 << " is an unkown noble " << endl;
                }
            }
        }
        
        else if (command == "Clear"){
            clearAll(nobles, warriors);  // free up heap space
        }
        
        else{
            cout << "Command is unknown" << endl;
        }
    }
    stream.close();
}


// This function takes in ifstream, and attemps to open the file. If cannot open the file, return an error message.
void openFile(ifstream& stream) {
    stream.clear();
    stream.open("nobleWarriors.txt");
    if (!stream) {
        cerr << "failed to open the file";
        exit(1);
    }
}

// This function checks warrior within a vector of pointers to warriors, return address of the warrior if found.
// If not found, return nullptr
Warrior* checkWarrior(const string& name, const vector<Warrior*>& warriors) {
    for (Warrior* warriorPoi : warriors) {
        if (warriorPoi->getName() == name) {
            return warriorPoi;
        }
    }
    return nullptr;
}

// This function checks noble within a vector of pointers to nobles, return address of the noble if found.
// If not found, return nullptr
Noble* checkNoble(const string& name, const vector<Noble*>& nobles) {
    for (Noble* noblePoi : nobles) {
        if (noblePoi->getName() == name) {
            return noblePoi;
        }
    }
    return nullptr;
}

// This function iterates through vectors of pointers pointing to the address on heap, then deletes all nobes and warriors,
// frees up heap space, returns nothing
void clearAll(vector<Noble*>& nobles, vector<Warrior*>& warriors) {
    for (Noble* noblePoi : nobles) {
        delete noblePoi;
    }
    nobles.clear();
    
    for (Warrior* warriorPoi : warriors) {
        delete warriorPoi;
    }
    warriors.clear();
}

// This function prints current status of all nobles and warriors, returns nothing
void displayStatus(vector<Noble*> const& nobles, vector<Warrior*> const& warriors) {
    cout << "Status" << endl << "======" << endl << "Nobles:" << endl;
    
    int hiredWarriorCount = 0;
    if (nobles.size() > 0) {
        for (Noble* noblePoi : nobles) {
            cout << *noblePoi;
            hiredWarriorCount += noblePoi-> getArmySize();  // add the army size to the hiredWarriorCount
        }
    }
    else {
        cout << "NONE" << endl;
    }
    
    
    cout << "Unemployed Warriors:" << endl;
    
    
    if (warriors.size() - hiredWarriorCount > 0) {   // check if there is unemployed warriors
        for (Warrior* warriorPoi : warriors) {
            if (!warriorPoi -> isHired()) {
                cout << *warriorPoi << endl;
            }
        }
    }
    
    else {  // no unemployed warriors
        cout << "NONE" << endl;
    }
}
