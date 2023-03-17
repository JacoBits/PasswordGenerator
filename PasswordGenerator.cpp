// Password generator by Jacobo F
// This code randomly generates a password, the user can decide the password's length, and complexity (whether it includes uppercase/lowercase, numbers, and/or symbols)
// This code was intended to test the standard library random header, and should not be used as an actual password generator, due to the algorithm used to generate 
// random numbers not being cryptographically safe

#include <random>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

//Size of the type_options array
const int MAX = 4;

//holds the possible characters that each type can hold
string lower = "abcdefghijklmnopqrstuvwxyz";
string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string numbers = "0123456789";
string symbols = "!@#$%?+=~-,.()/";


//Determines what type of character to can be chosen given a selector and the restrictions for the password
//Returns a number to be used to select the type of character to be used in the password
//@param the selector int, used to determine the type of character to be used in the password
//@param up, num, and sym, booleans that equal true if that type of character is to be used in the password
//@return selector, an int between 0 and 4, used for the program to choose a character type to use in the password
int GetType(int selector, bool up, bool num, bool sym) {
    if (selector == 3 && !sym)
        selector--;
    if (selector == 2 && !num)
        selector--;
    if (selector == 1 && !up)
        selector--;

    return selector;

}


int main()
{
    //random device used to seed ranlux_24
    random_device rd;
    //pseudo random number generator (prng) from the stl::random library, using the ranlux_24 base
    ranlux24_base rng(rd());

    //operator, holds user commands to determine the password options
    char op;

    //hold user preferences for the password, true means that they will be used
    bool longpassword;
    bool useUpper;
    bool useNumbers;
    bool useSymbols;

    //holds the length of the password to be generated, modified by longpassword bool
    int passlength;
    
    //used for the loop
    bool quit;
    bool saveSettings = false;

    //used in the code to make sure that all character types selected by the user are used in the password generation
    int temp = 0;
    

    //an array of strings with the character types to be used in the password
    string options[MAX];
    options[0] = lower;
    options[1] = upper;
    options[2] = numbers;
    options[3] = symbols;

    //used with the prng to select the character type and character to be added for the password
    int selector;
    int position;

    //stores the type of character selected by the selector int
    string chosenType;

    cout << "Welcome to the password generator" << endl << endl;

    char character;

    do {
        quit = false;

        //make the user select the settings for the password they want to generate
        if (!saveSettings) {
            longpassword = true;
            useUpper = true;
            useNumbers = true;
            useSymbols = true;

            cout << "Do you want to generate a long password (16 characters)? [Y/N] " << endl;
            cin >> op;
            if (toupper(op) == 'N')
                longpassword = false;
            cout << "Do you want to include uppercase letters? [Y/N]" << endl;
            cin >> op;
            if (toupper(op) == 'N')
                useUpper = false;
            cout << "Do you want to include numbers? [Y/N]" << endl;
            cin >> op;
            if (toupper(op) == 'N')
                useNumbers = false;
            cout << "Do you want to include special characters? [Y/N]" << endl;
            cin >> op;
            if (toupper(op) == 'N')
                useSymbols = false;
        }

        if (longpassword)
            passlength = 16;
        else
            passlength = 8;

        //creates a password string with no characters in it
        string password = "";


        //this is needed for all the character types that the user selected to appear in the password in every generation
        //generates the first four characters of the password with different types
        for (int i = 0; i < MAX; i++) {
            if (i == 0)
                chosenType = options[i];
            else {
                temp = GetType(i, useUpper, useNumbers, useSymbols);
                chosenType = options[temp];
            }
            uniform_int_distribution<int> positionSelector(0, chosenType.length() - 1);
            position = positionSelector(rng);
            character = chosenType.at(position);
            password += character;
        }


        //generates the rest of the characters in the password, randomly selecting a character type, and then randomly selecting a position in the string of the character type
        for (int i = 4; i < passlength; i++) {

            //generates a number from 0 to 3 as a selector in the options array
            uniform_int_distribution<int> typeSelector(0, MAX - 1);
            selector = typeSelector(rng);

            //depending on user choices, the selector int is changed so only the character types wanted by the user are used
            selector = GetType(selector, useUpper, useNumbers, useSymbols);

            //cout << "Selector: " << selector << endl; DEBUG

            //makes chosentype into the selector index in the options array
            chosenType = options[selector];

            //selects a random position (character) in that string, and adds it to the password
            uniform_int_distribution<int> positionSelector(0, chosenType.length() - 1);
            position = positionSelector(rng);
            character = chosenType.at(position);
            //cout << "Character selected: " << character << endl; DEBUG

            password += character;

            //cout << "Password is: " << password << endl; DEBUG
        }

        //shuffles the password to add randomness, and change the pattern of the first four characters being different types
        shuffle(password.begin(), password.end(), rng);

        cout << "Your password is: --> " << password << " <--" << endl << endl;

        cout << "Do you want to generate another password? [Y/N]" << endl;
        cin >> op;
        if (toupper(op) == 'N') {
            quit = true;
            cout << "Thank you for using the password generator" << endl;
        }
        else {
            cout << "Do you want to use the same settings as the previous password? [Y/N]" << endl;
            cin >> op;
            if (toupper(op) == 'Y')
                saveSettings = true;
            else
                saveSettings = false;
        }
    } while (!quit);
    
    cin.ignore();

    return 0;
}

