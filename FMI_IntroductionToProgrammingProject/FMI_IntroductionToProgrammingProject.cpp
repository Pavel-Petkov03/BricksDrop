// FMI_IntroductionToProgrammingProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
constexpr int GAMEMATRIXROWS = 10;
constexpr int MAXNAMESIZE = 20;
constexpr int MAX_LINE_LENGTH = 30;
const char* USERNAME_ERROR_MESSAGE = "Please enter username below 20 symbols";
const char* FILE_NAME = "database.txt";


int myStrLen(const char * text) {
    if (!text) {
        return -1;
    }
    int counter = 0;
    while (*text) {
        counter++;
        text++;
    }
    return counter;
}

bool stringsAreEqual (char * first, char * second) {
    if (!first || !second) {
        return false;
    }
    int index = 0;
    while (first[index] && second[index]) {
        if (first[index] != second[index]) {
            return false;
        }
        index++;
    }
    return (first[index] - second[index]) == 0;
}

void readUsername(char * username) {
    const int bufferSize = 30;
    cin.getline(username, MAXNAMESIZE);
    while (myStrLen(username) > MAXNAMESIZE) {
        cout << USERNAME_ERROR_MESSAGE << endl;
        cin.getline(username, MAXNAMESIZE);
    }
}


int fromCharToDecimal(char symbol){
    return symbol - '0';
}


void splitRowFromFile(char* line, char* currentUsername, int &currentPoinst) {
    while (*line != ' ') {
        *currentUsername = *line;
        line++;
        currentUsername++;
    }
    line += 3;
    while ((*line)) {
        int digit = fromCharToDecimal(*line);
        currentPoinst *= 10;
        currentPoinst += digit;
        line++;
    }
}

bool userAlreadyLoggedIn(char* username, fstream& MyFile, unsigned int &userCurrentPoints) {
    char line[MAX_LINE_LENGTH];
    while (MyFile.getline(line, MAX_LINE_LENGTH)) {
        char currentUsername[10] = " ";
        int currentPoints = 0;
        splitRowFromFile(line, currentUsername, currentPoints);
        if (stringsAreEqual(currentUsername, username)) {
            userCurrentPoints = currentPoints;
            return true;
        }
    }
    return false;
}


int logUser(char *username, const char* filename, unsigned int &userCurrentPoints)
{
    fstream MyFile(filename, ios::in | ios::out); // starts from the beginning of the file
    if (!MyFile.is_open()) {
        return 1;
    }

    if (userAlreadyLoggedIn(username, MyFile, userCurrentPoints)) {
        cout << "Welcome " << username << " You have " << userCurrentPoints << " max score" << endl;
    }
    else {
        MyFile.clear();
        MyFile.seekp(0, ios::end); // this goes to the end of the file

        MyFile << username << " - " << "0" << endl;
        MyFile.close();

        cout << username << " successfully registered" << endl;
        return 0;
    }

    
}




int main()
{
    char gameMatrix[GAMEMATRIXROWS][GAMEMATRIXROWS];
    int currentRow = GAMEMATRIXROWS - 1;
    char username[MAXNAMESIZE];
    readUsername(username);
    unsigned int usernameCurrentPoints = 0;
    logUser(username, FILE_NAME, usernameCurrentPoints);
}
