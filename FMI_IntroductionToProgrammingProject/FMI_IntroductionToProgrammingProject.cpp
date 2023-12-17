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
const char colorArray[] = {'a', 'b', 'c', 'd'};

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
    cout << "Enter username" << endl;
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
    int index = 0;
    while (*line != ' ') {
        currentUsername[index] = *line;
        line++;
        index++;
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
        char currentUsername[MAXNAMESIZE] = " ";
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
int randIntInRange(int start, int end) {
    srand(time(0));
    int range = end - start + 1;
    return rand() % range + start;
}


void generateRow(char matrix[][GAMEMATRIXROWS], int currentRow) {
    int countOfBricks = randIntInRange(1, 4);
    int rowAvailabbleSpace = GAMEMATRIXROWS;
    int startBrickPosition = 0;
    for (int i = 0; i < countOfBricks; i++) {
        int currentBrickLength = randIntInRange(1, rowAvailabbleSpace / countOfBricks + 1);
        rowAvailabbleSpace -= currentBrickLength;
        int currentBrickStartIndex = randIntInRange(0, rowAvailabbleSpace / countOfBricks);
        rowAvailabbleSpace -= currentBrickStartIndex;
        startBrickPosition += currentBrickStartIndex;
        for (int j = 0; j < currentBrickLength; j++) {
            matrix[currentRow][j + startBrickPosition] = colorArray[i];
        }
        startBrickPosition += currentBrickLength;
    }
}

void printBoard(char matrix[][GAMEMATRIXROWS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void initMatrix(char matrix[][GAMEMATRIXROWS], int rows, int cols, char defaultValue) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = defaultValue;
        }
    }
}


bool isRowDestroyable(char matrix[GAMEMATRIXROWS], int currentRow) {
    return false;
}

void makeMove() {
    int x, y;
    char direction;
    int movePositions;
    cout << "Enter block coordinates" << endl;
    cin >> x >> y;
    cout << "Enter direction" << endl;
    cin >> direction;
    cout << "Enter move positons" << endl;
    cin >> movePositions;
}


bool availableSpaceInRowBelow(char matrix[][GAMEMATRIXROWS], int startIndex, int endIndex, int currentRow) {
    for (int i = startIndex; i <= endIndex; i++) {
        if (matrix[currentRow+1][i] != '0') {
            return false;
        }
    }
    return true;
}


void moveBrickDown(char matrix[][GAMEMATRIXROWS], int startIndex, int endIndex, int currentRow) {
    for (int i = startIndex; i <= endIndex; i++) {
        matrix[currentRow + 1][i] = matrix[currentRow][i];
        matrix[currentRow][i] = '0';
    }
}


bool allRowFilled(char matrix[][GAMEMATRIXROWS], int currentFilledRow) {
    for (int i = 0; i < GAMEMATRIXROWS; i++) {
        if (matrix[currentFilledRow][i] == '0') {
            return false;
        }
    }
    return true;
}


void destroyRow(char matrix[][GAMEMATRIXROWS], int currentRow) {
    for (int i = GAMEMATRIXROWS - 1; i > currentRow; i--) {
        if (allRowFilled(matrix, i)) {
             for (int q = 0; q < GAMEMATRIXROWS; q++) {
                 matrix[i][q] = matrix[i - 1][q];
             }
        }
    }
}

bool isShrinkable(char matrix[][GAMEMATRIXROWS], int currentRow) {
    bool isMovedDown = false;
    for (int i = currentRow; i < GAMEMATRIXROWS - 1; i++) {
        int startBrickIndex = 0;
        int endBrickIndex = 0;
        char currentColor = matrix[currentRow][0];
        bool isBrick = false;
        for (int j = 0; j < GAMEMATRIXROWS; j++) {
            if (matrix[i][j] == '0' || (currentColor != '0' && currentColor != matrix[i][j])) {
                if (isBrick) {
                    isBrick = false;
                    endBrickIndex = j-1;
                    if (availableSpaceInRowBelow(matrix, startBrickIndex, endBrickIndex, i)) {
                        isMovedDown = true;
                        moveBrickDown(matrix, startBrickIndex, endBrickIndex, i);
                    }
                    if (matrix[i][j] != '0') {
                        startBrickIndex = j;
                        isBrick = true;
                    }
                    currentColor = matrix[i][j];
                }
            }
            else {
                if (!isBrick) {
                    currentColor = matrix[i][j];
                    startBrickIndex = j;
                }
                isBrick = true;
            }
        }
    }
    return isMovedDown;
}


void runGame(char * username, unsigned int usernameCurrentPoints) {
    char gameMatrix[GAMEMATRIXROWS][GAMEMATRIXROWS];
    initMatrix(gameMatrix, GAMEMATRIXROWS, GAMEMATRIXROWS, '0');
    int currentRow = GAMEMATRIXROWS - 1;
    int a = 0;
    while (currentRow >= 0) {
        generateRow(gameMatrix, currentRow);
        printBoard(gameMatrix, GAMEMATRIXROWS, GAMEMATRIXROWS);
        cout << endl;
        do
        {
            usernameCurrentPoints += 10;
            destroyRow(gameMatrix, currentRow);
        } while (isShrinkable(gameMatrix, currentRow));

        printBoard(gameMatrix, GAMEMATRIXROWS, GAMEMATRIXROWS);
        makeMove();
        currentRow--;
    }
    printBoard(gameMatrix, GAMEMATRIXROWS, GAMEMATRIXROWS);
}

int main()
{
    
    char username[MAXNAMESIZE];
    readUsername(username);
    unsigned int usernameCurrentPoints = 0;
    logUser(username, FILE_NAME, usernameCurrentPoints);
    runGame(username, usernameCurrentPoints);
}
