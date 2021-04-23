//HW02
//Tracy Zhu
//Medieval Battle Simulator
//This file will simulate warriors battling in medieval times. It will read from a file of warriors with name and strengths. According to commands in the files, warriirs will be put into battles. Battles will alter the strengths of warriors. Results will be displayed after each battle, and changes in strengths will also be displayed when status command is called.

// Rules: When a warrior battles another, the winner will be the warrior with a higher strength value. The winer will lose as much strength as his opponent has. The loser will lose all his strengths. When warriors with the same strength battle, they will both lose.

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

// Construct warrior
struct Warrior{
  string name;
  int strength;
};


//Prototypes
void open_file(ifstream& stream);
size_t locate_warrior(const vector <Warrior>& warriors, const string& name);
void battle_result(const string& name1, const string& name2, vector <Warrior>& warriors);
void display_status(const vector<Warrior>& warriors);
void add_warrior(vector<Warrior>& warriors, const string& name, int strength);
void read_commands(vector<Warrior>& warriors, ifstream& ifs);



//Main function
int main(){
    vector<Warrior> warriors;   //create a vector that containes all Warrior sturcture
    ifstream filename;      // create an ifstream variable for reading the file
    open_file(filename);
    read_commands(warriors, filename);
    filename.close();    //close the file
}



//This function attempts to open the warriors.txt file by taking ifstream. It outputs an error message if failed to open it, and returns nothing
void open_file(ifstream& stream){
    stream.open("warriors.txt");
    if(!stream) {
        cerr << "Could not find 'warriors.txt'" << endl;
        exit(1);
    }
}



//This functions finds the position of a warrior. It takes in the vector of Warrior by reference and a string name for warrior, then it returns a type size_t of the position of the warrior.
size_t locate_warrior(const vector <Warrior>& warriors, const string& name){
    for (size_t i = 0; i< warriors.size(); ++i) {
        if (warriors[i].name == name) {
            return i;
        }
    }
    return warriors.size();
}



// This function displays the result of battle according to two waariors' strengths. It takes in the input vector of Warrior and constant strings of two warriors' names. It returns nothing.
void battle_result(const string& name1, const string& name2, vector <Warrior>& warriors){
    
    size_t loc1 = locate_warrior(warriors,name1); // find where the first warrior is
    size_t loc2 = locate_warrior(warriors,name2);// find where the second warrior is
    
    cout<< warriors[loc1].name << " battles " << warriors[loc2].name << endl;
    
    if (warriors[loc1].strength == 0 || warriors[loc2].strength == 0) {
        
        if (warriors[loc1].strength == 0 && warriors[loc2].strength == 0){
            cout <<"Oh, NO! They're both dead! Yuck!" << endl;
            // if both warriors are already dead, then print that they are both already dead
        }
        
        else if (warriors[loc1].strength == 0){
            cout << "He's dead, " << warriors[loc2].name << endl;
            // if one warrior is alive and the other is dead, then print he's dead and the name of the alive warrior
        }

        else if (warriors[loc2].strength == 0){
            cout << "He's dead, " << warriors[loc1].name << endl;
            // if one warrior is alive and the other is dead, then print he's dead and the name of the alive warrior (same thing but swtich around)
        }
    }
    
    
    else if (warriors[loc1].strength == warriors[loc2].strength) {
        cout <<"Mutual Annihilation: "<< warriors[loc1].name << " and " << warriors[loc2].name <<" die at each other's hands " << endl;
        warriors[loc1].strength = warriors[loc2].strength = 0;
        //if both warriors have the same strength but are not both dead, then there is a mutual annihilation and both strengths are set to zero.
    }
    
    
    else if (warriors[loc1].strength > warriors[loc2].strength){
        cout << warriors[loc1].name << " defeats " << warriors[loc2].name << endl;
        warriors[loc1].strength -= warriors[loc2].strength;
        warriors[loc2].strength = 0;
    }
    // if one warrior is stronger than the other, then one warrior defeats the other. The loser's strength is set to zero and the winner loses the amount of strength the loser had.
    

    else if (warriors[loc1].strength < warriors[loc2].strength){
        cout << warriors[loc2].name << " defeats " << warriors[loc1].name << endl;
        warriors[loc2].strength -= warriors[loc1].strength;
        warriors[loc1].strength = 0;
    }
     // if one warrior is stronger than the other, then one warrior defeats the other. The loser's strength is set to zero and the winner loses the amount of strength the loser had. (same thing but switch around)
}


// This function shows the current status of warriors, inlcuding the number of warriors, names and strength value of each warrior. It takes the input type vector of Warrior and returns nothing.
void display_status(const vector<Warrior>& warriors){
    cout<< "There are " << warriors.size() << " warriors\n";
    for (size_t i = 0; i < warriors.size(); ++i){
        cout<< "Warrior: "<< warriors[i].name << ", strength: " << warriors[i].strength << endl;
    }
    // loop through the vector, print the name and strength value of each warriror
    
}


// This function takes in the vector by reference, integer strength value and string name of warrior. It adds new warrior to the vector and returns nothing
void add_warrior(vector<Warrior>& warriors, const string& name, int strength) {
    Warrior new_warrior;     //create a Warrior struct
    new_warrior.name = name;    //assign the warrior name
    new_warrior.strength = strength;    //assign the warrior name
    warriors.push_back(new_warrior);    //add the new Warrior struct to the vector
}



//This function takes input ifstream and the vector of Warrior by reference to print the outputs according to the file commands. It returns nothing.
void read_commands(vector<Warrior>& warriors, ifstream& ifs){
    string commands;
    while (ifs >> commands){      // read commands and take different actions
        
        if (commands == "Warrior"){   // if command is 'Warrior', call add_warrior to add the warrior into vector
            int strength;
            string name;
            ifs >> name >> strength;
            add_warrior(warriors, name, strength);
        }
        
        if (commands == "Status"){     // if command is 'Status', call display_status to show all warriors and their strengths
            display_status(warriors);
        }
        
        else if (commands == "Battle") {    // if command is 'Battle', call battle_result to show the results of battle
            string name1, name2;
            ifs >> name1 >> name2;
            battle_result (name1, name2, warriors);
        }
    }
}


