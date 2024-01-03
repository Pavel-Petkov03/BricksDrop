// FMI_IntroductionToProgrammingProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;
constexpr int GAMEMATRIXROWS = 10;
constexpr int MAXNAMESIZE = 20;
constexpr int MAX_LINE_LENGTH = 30;
const char* USERNAME_ERROR_MESSAGE = "Please enter username below 20 symbols";
const char* FILE_NAME = "database.txt";
const char colorArray[] = {'a', 'b', 'c', 'd'};
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int getIndex(const char* arr, char symbol) {
    for (int i = 0;  i < 4; i++) {
        if (arr[i] == symbol) {
            return i+1;
        }
    }
    return 10;
}

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
    cin >> username;
    while (myStrLen(username) > MAXNAMESIZE) {
        cout << USERNAME_ERROR_MESSAGE << endl;
        cin >> username;
    }
}


int fromCharToDecimal(char symbol){
    return symbol - '0';
}


void splitRowFromFile(char* line, char* currentUsername, int &currentPoints) {
    int index = 0;
    const int spacesBetweenData = 3; 
    while (*line != ' ') {
        currentUsername[index] = *line;
        line++;
        index++;
    }
    line += spacesBetweenData;
    while ((*line)) {
        int digit = fromCharToDecimal(*line);
        currentPoints *= 10;
        currentPoints += digit;
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
        MyFile.close();
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
    int range = end - start + 1;
    if (range == 0) {
        cout << "Everything fucked up" << endl;
    }
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
        int q = randIntInRange(0, 3);
        for (int j = 0; j < currentBrickLength; j++) {
            matrix[currentRow][j + startBrickPosition] = colorArray[q];
        }
        startBrickPosition += currentBrickLength;
    }
}

void printBoard(char matrix[][GAMEMATRIXROWS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            SetConsoleTextAttribute(hConsole, getIndex(colorArray, matrix[i][j]));
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

void findBlockRange(char* row, int &startBlockIndex, int& endBlockIndex, int startPosition) {
    char startColor = row[startPosition];
    startBlockIndex = 0;
    endBlockIndex = 9;
    for (int i = startPosition+1; i < GAMEMATRIXROWS; i++) {
        if (row[i] != startColor) {
            endBlockIndex = i - 1;
            break;
        }
    }
    for (int i = startPosition - 1; i >= 0; i--) {
        if (row[i] != startColor) {
            startBlockIndex = i +1;
            break;
        }
    }
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


void destroyRow(char matrix[][GAMEMATRIXROWS], int rowBelow, int &currentRow, unsigned int &currentUserPoints) {
    for (int q = 0; q < GAMEMATRIXROWS; q++) {
        matrix[rowBelow][q] = matrix[rowBelow - 1][q];
        matrix[rowBelow - 1][q] = '0';
    }
    currentUserPoints += 10;
    cout << "You got 10 points" << endl;
}


void findDestroyableRow(char matrix[][GAMEMATRIXROWS], int &currentRow, unsigned int&currentUserPoints) {
    for (int i = GAMEMATRIXROWS - 1; i > currentRow; i--) {
        if (allRowFilled(matrix, i)) {
            destroyRow(matrix, i, currentRow, currentUserPoints);
            break;
        }
    }
}


bool allRowShrinked(char matrix[][GAMEMATRIXROWS], int &currentRow, bool &isMovedDown, int i) {
    int m[4][2];
    int mI = 0;
    int currentIndex = 0;
    while (currentIndex < GAMEMATRIXROWS) {
        if (matrix[i][currentIndex] != '0') {
            int startBlockIndex = 0;
            int endBlockIndex = 0;
            findBlockRange(matrix[i], startBlockIndex, endBlockIndex, currentIndex);
            currentIndex = endBlockIndex + 1;
            m[mI][0] = startBlockIndex;
            m[mI][1] = endBlockIndex;
            mI++;
        }
        else {
            currentIndex++;
        }
    }
    bool allRowDown = true;
    for (int j = 0; j < mI; j++) {
        if (availableSpaceInRowBelow(matrix, m[j][0], m[j][1], i)) {
            isMovedDown = true;
            moveBrickDown(matrix, m[j][0], m[j][1], i);
        }
        else {
            allRowDown = false;
        }
    }
    return allRowDown;
}

 

bool isShrinkable(char matrix[][GAMEMATRIXROWS], int &currentRow) {
    bool isMovedDown = false;
    bool allRowDown = false;
    for (int i = GAMEMATRIXROWS - 2; i >= currentRow; i--) {
        if (allRowShrinked(matrix, currentRow, isMovedDown, i)) {
            allRowDown = true;
            cout << "Everything dropped down" << endl;
        }
    }
    if (allRowDown) {
        currentRow++;
    }
    return isMovedDown;
}


void shrinkRows(char matrix[][GAMEMATRIXROWS], int &currentRow, unsigned int& usernameCurrentPoints) {
    do
    {
        findDestroyableRow(matrix, currentRow, usernameCurrentPoints);
    } while (isShrinkable(matrix, currentRow));
}

void generateRowUntilNotDestroyable(char matrix[][GAMEMATRIXROWS], int currentRow, unsigned int& usernameCurrentPoints) {
    generateRow(matrix, currentRow);
    while (allRowFilled(matrix, currentRow)) {
        destroyRow(matrix, currentRow, currentRow, usernameCurrentPoints);
        generateRow(matrix, currentRow);
    }
}

void moveInDirection(char matrix[][GAMEMATRIXROWS], char direction, int startBlockIndex, int endBlockIndex, int timesToMove, int currentRow) {
    if (direction == 'r') {
        for (int i = endBlockIndex; i < GAMEMATRIXROWS - 1; i++) {
            if (matrix[currentRow][i + 1] == '0' && timesToMove != 0) {
                for (int j = i; j >= startBlockIndex; j--) {
                    matrix[currentRow][j+1] = matrix[currentRow][j];
                }
                matrix[currentRow][startBlockIndex] = '0';
                startBlockIndex++;
                endBlockIndex++;
                timesToMove--;
                if (availableSpaceInRowBelow(matrix, startBlockIndex, endBlockIndex, currentRow)) {
                    moveBrickDown(matrix, startBlockIndex, endBlockIndex, currentRow);
                    return;
                }
            }
            else {
                break;
            }
        }
    }
    else if (direction == 'l') {
        for (int i = startBlockIndex; i > 0; i--) {
            if (matrix[currentRow][i - 1] == '0' && timesToMove != 0) {
                for (int j = i; j <= endBlockIndex; j++) {
                    matrix[currentRow][j - 1] = matrix[currentRow][j];
                }
                matrix[currentRow][endBlockIndex] = '0';
                startBlockIndex--;
                endBlockIndex--;
                timesToMove--;
                if (availableSpaceInRowBelow(matrix, startBlockIndex, endBlockIndex, currentRow)) {
                    moveBrickDown(matrix, startBlockIndex, endBlockIndex, currentRow);
                    return;
                }
            }
            else {
                break;
            }   
        }
    }
}

void makeMove(char matrix[][GAMEMATRIXROWS], int currentRow) {
    int row, col;
    char direction;
    int movePositions;
    cin >> row >> col;
    cin >> direction;
    cin >> movePositions;
    int startBlockIndex = 0;
    int endBlockIndex = 0;
    findBlockRange(matrix[row], startBlockIndex, endBlockIndex, col);
    moveInDirection(matrix, direction, startBlockIndex, endBlockIndex, movePositions, row);
}



void runGame(char* username, unsigned int usernameCurrentPoints) {
    char gameMatrix[GAMEMATRIXROWS][GAMEMATRIXROWS];
    initMatrix(gameMatrix, GAMEMATRIXROWS, GAMEMATRIXROWS, '0');

    int currentRow = GAMEMATRIXROWS - 1;
    while (currentRow > 0) {
        generateRowUntilNotDestroyable(gameMatrix, currentRow, usernameCurrentPoints);
        shrinkRows(gameMatrix, currentRow, usernameCurrentPoints);
        printBoard(gameMatrix, GAMEMATRIXROWS, GAMEMATRIXROWS);
        cout << "Current score is " << usernameCurrentPoints << endl;
        makeMove(gameMatrix, currentRow);
        system("cls"); // clears the console
        shrinkRows(gameMatrix, currentRow, usernameCurrentPoints);
        currentRow--;
    }
    cout << usernameCurrentPoints << endl;
}


void printDialogue() {
    cout << "Press 1 if you want to play the game" << endl;
    cout << "Press 2 if you want to see the leaderboard" << endl;
    cout << "Press anything else if you want to leave the game" << endl;
}


void loadBricksDropGame() {
    char username[MAXNAMESIZE];
    readUsername(username);
    srand(time(0));
    unsigned int usernameCurrentPoints = 0;
    unsigned int userMaxPoints = 0;
    logUser(username, FILE_NAME, userMaxPoints);
    runGame(username, usernameCurrentPoints);
    if (usernameCurrentPoints > userMaxPoints) {
        cout << "You have beaten your record" << endl;
        cout << "Your new record is " << usernameCurrentPoints << endl;
    }
    fstream MyFile(FILE_NAME, ios::in | ios::out); // starts from the beginning of the file
    if (!MyFile.is_open()) {
        return;
    }
    char line[MAX_LINE_LENGTH];
    while (MyFile.getline(line, MAX_LINE_LENGTH)) {
        char currentUsername[MAXNAMESIZE] = " ";
        int currentPoints = 0;
        splitRowFromFile(line, currentUsername, currentPoints);
        if (stringsAreEqual(currentUsername, username)) {
            MyFile << username << " - " << usernameCurrentPoints;
            break;
        }
    }
    MyFile.close();

}

void showLeaderBoard() {

}

void runOptions(bool &mainLoop) {
    int command;
    cin >> command;
    switch (command)
    {
    case 1:
        loadBricksDropGame();
        break;
    case 2:
        showLeaderBoard();
        break;
    default:
        mainLoop = false;
        break;
    }
}

int main()
{
    cout << "Welcome to Bricks Drop" << endl;
    bool mainLoop = true;
    while (mainLoop) {
        printDialogue();
        runOptions(mainLoop);
    }
}
