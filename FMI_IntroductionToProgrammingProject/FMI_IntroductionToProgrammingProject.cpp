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

bool userAlreadyLoggedIn(char* username, fstream& MyFile, unsigned int &userCurrentPoints) {
    char line[MAX_LINE_LENGTH];
    while (MyFile.getline(line, MAX_LINE_LENGTH)) {
        cout << line << endl;
    }
    return false;
}


int logUser(char *username, const char* filename, unsigned int &userCurrentPoints)
{
    fstream MyFile(filename, ios::in | ios::out);
    if (!MyFile.is_open()) {
        return 1;
    }

    if (userAlreadyLoggedIn(username, MyFile, userCurrentPoints)) {
    }

    MyFile.clear();
    MyFile.seekp(0, ios::end);

    MyFile << username << " - " << "0" << endl;
    MyFile.close();
    return 0;
}




int main()
{
    char gameMatrix[GAMEMATRIXROWS][GAMEMATRIXROWS];
    int currentRow = GAMEMATRIXROWS - 1;
    char username[MAXNAMESIZE];
    readUsername(username);
    unsigned int usernameCurrentPoints = 0;
    logUser(username, FILE_NAME, usernameCurrentPoints);
    cout << username << endl;
}
