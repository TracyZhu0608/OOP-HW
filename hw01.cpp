// HW01: Caesar Cypher
// Tracy Zhu yz5698
// A program to decrypt an encrypted file using the classic Caesar Cypher algorithm

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

//Decrypting Character Function:
//take a encypted character and an integer rotation distance; decrypt with the Caesar cypher algorithm; return the unencrypted character

char decrypt_char(char character, int distance){
    if (character >= 'a' && character <= 'z'){     // if the character is a lowercase character, rotate the character
        
        distance %= 26; //take into account the case if rotation distance is greater than 26 or less than -26, so only consider its reminder divided by 26
        
        // if rotation distance is greater than zero and the character is less than 'a' after the decrypting, this means some distances are overflowed. Go to 'z'.
        if (distance > 0 && character - distance < 'a'){
            int overflow = distance - (character - 'a') - 1;   // Calculate the overflowing distance
            character = 'z' - overflow;   // Rotate from 'z' by the overflowing distance
        }
        
        // if rotation distance is less than zero and the character is greater than 'z' after the decrypting, this means some distances are overflowed. Go to 'a'.
        else if (distance < 0 && character - distance > 'z'){
            
            int overflow = distance - (character - 'z') + 1;    // Calculate the overflowing distance
            character = 'a' - overflow;   // Rotate from 'a' by the overflowing distance
        }
        
        
        // base case: rotate the character by the rotation distance
        else{
            character = character - distance;
        }
        
    } // if character is not a lowercase charactor, return the character directly
    return character;
}



//Decrypting String Function:
//take a encrypted string and an integer rotation distance; decrypt the string with the Decrypting Chracter Function; return nothing

void decrypt_string(string& string, int distance){    // Pass-by-reference
    
    //Iterate over the encrypted string and use the Decrypting Chracter Function to decypt the string character by character
    for (size_t i = 0; i < string.size(); ++i) {
        string[i] = decrypt_char(string[i], distance);
    }
}



// Main function:
// open and read the 'encrpted.txt' files; create a vector and store file lines in the vector; call Decrypting String Function to decrypt the file; print out results in the reverse order

int main() {
    //Attempt to open file "encrypted.txt"; return an error if the file is not found
    ifstream encrypted_file;
    encrypted_file.open("encrypted.txt");
    if (!encrypted_file) {
        cerr << "cannot find encrypted file 'encrypted.txt'";
        exit(1);
    }
    
    //Create a integer variable and get the first line which contains the rotation distance value
    int distance;
    encrypted_file >> distance;
    
    //Create a string variable for temporarily storing each string line
    string line;
    
    //Create a vector to store all the lines read from the file
    vector<string> filelines;
    
    //Read the file line by line and keep storing the lines into the filelines vector
    while (getline(encrypted_file, line)) {
        filelines.push_back(line);
    }
    
    //Close the file
    encrypted_file.close();
    
    //Iterate through the vector, decrypt the file and print out results in the correct order
    for (size_t i = 0; i < filelines.size(); i++) {
        string curr_line = filelines[filelines.size() - i - 1];    //Reverse the order and store as a new string
        decrypt_string(curr_line, distance);     //Decrypt the new string using the Decrypting String Function
        cout << curr_line << endl;    //print out result
    }
}
