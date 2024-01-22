/**
*  
* Solution to course project # <номер на вариант>
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Pavel Petkov
* @idnumber 0MI0600302
* @compiler VC
*
* 
*
*/

#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;
constexpr int GAMEMATRIXROWS = 10;
constexpr int MAXNAMESIZE = 20;
constexpr int MAX_LINE_LENGTH = 30;
const char* FILE_NAME = "database.txt";
const char colorArray[] = {'a', 'b', 'c', 'd'};
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int getColorIndex(const char* arr, char symbol) {
    int defaultColor = 12;
    int startColorIndex = 0;
    int endColorIndex = 4;
    for (int i = startColorIndex;  i < endColorIndex; i++) {
        if (arr[i] == symbol) {
            return i+1;
        }
    }
    return defaultColor;
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
bool stringsAreEqual (const char* first, const char* second) {
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
void readUsername(char* username) {
    cout << "Enter username" << endl;
    cin >> username;
    while (myStrLen(username) > MAXNAMESIZE) {
        cout << "Please enter username below 20 symbols" << endl;
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
        range = 1;
    }
    return rand() % range + start;
}
void generateRow(char matrix[][GAMEMATRIXROWS], int currentRow) {
    int minBricksCount = 1;
    int maxBricksCount = 4;
    int countOfBricks = randIntInRange(minBricksCount, maxBricksCount);
    int rowAvailableSpace = GAMEMATRIXROWS;
    int startBrickPosition = 0;
    for (int i = 0; i < countOfBricks; i++) {
        int currentBrickLength = randIntInRange(1, rowAvailableSpace / countOfBricks + 1);
        rowAvailableSpace -= currentBrickLength;
        int currentBrickStartIndex = randIntInRange(0, rowAvailableSpace / countOfBricks);
        rowAvailableSpace -= currentBrickStartIndex;
        startBrickPosition += currentBrickStartIndex;
        int colorIndex = randIntInRange(0, 3);
        for (int j = 0; j < currentBrickLength; j++) {
            matrix[currentRow][j + startBrickPosition] = colorArray[colorIndex];
        }
        startBrickPosition += currentBrickLength;
    }
}
void printBoard(char matrix[][GAMEMATRIXROWS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            SetConsoleTextAttribute(hConsole, getColorIndex(colorArray, matrix[i][j]));
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
bool allRowShrinked(char matrix[][GAMEMATRIXROWS], int &currentRow, bool &isMovedDown, int rowIndex) {
    int brickMatrix[5][2];
    int brickMatrixIndex = 0;
    int currentIndex = 0;
    while (currentIndex < GAMEMATRIXROWS) {
        if (matrix[rowIndex][currentIndex] != '0') {
            int startBlockIndex = 0;
            int endBlockIndex = 0;
            findBlockRange(matrix[rowIndex], startBlockIndex, endBlockIndex, currentIndex);
            currentIndex = endBlockIndex + 1;
            brickMatrix[brickMatrixIndex][0] = startBlockIndex;
            brickMatrix[brickMatrixIndex][1] = endBlockIndex;
            brickMatrixIndex++;
        }
        else {
            currentIndex++;
        }
    }
    bool allRowDown = true;
    for (int j = 0; j < brickMatrixIndex; j++) {
        if (availableSpaceInRowBelow(matrix, brickMatrix[j][0], brickMatrix[j][1], rowIndex)) {
            isMovedDown = true;
            moveBrickDown(matrix, brickMatrix[j][0], brickMatrix[j][1], rowIndex);
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
void moveBrickLeft(char matrix[][GAMEMATRIXROWS], int startBlockIndex, int endBlockIndex, int timesToMove, int currentRow) {
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
void moveBrickRight(char matrix[][GAMEMATRIXROWS], int startBlockIndex, int endBlockIndex, int timesToMove, int currentRow) {
    for (int i = endBlockIndex; i < GAMEMATRIXROWS - 1; i++) {
        if (matrix[currentRow][i + 1] == '0' && timesToMove != 0) {
            for (int j = i; j >= startBlockIndex; j--) {
                matrix[currentRow][j + 1] = matrix[currentRow][j];
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
void moveInDirection(char matrix[][GAMEMATRIXROWS], char direction, int startBlockIndex, int endBlockIndex, int timesToMove, int currentRow) {
    if (direction == 'r') {
        moveBrickRight(matrix, startBlockIndex, endBlockIndex, timesToMove, currentRow);
    }
    else if (direction == 'l') {
        moveBrickLeft(matrix, startBlockIndex, endBlockIndex, timesToMove, currentRow);
    }
}
bool validMoveEntries(int row, int col, char direction, int movePositions) {
    bool isValid = row >= 0 && row <= GAMEMATRIXROWS - 1 &&
        col >= 0 && col <= GAMEMATRIXROWS - 1 && (direction == 'r' || direction == 'l') && movePositions > 0;
    if (!isValid) {
        cin.clear();
        cin.ignore();
        cout << "Invalid move brick operation(Try Again)" << endl;
    }
    return isValid;
}
void validateMoveInput(int &row, int &col, char &direction, int &movePositions) {
    do
    {
        cin >> row >> col >> direction >> movePositions;
        
    } while (!validMoveEntries(row, col, direction, movePositions));
}
void makeMove(char matrix[][GAMEMATRIXROWS], int currentRow) {
    int row, col;
    char direction;
    int movePositions;
    validateMoveInput(row, col, direction, movePositions);
    int startBlockIndex = 0;
    int endBlockIndex = 0;
    findBlockRange(matrix[row], startBlockIndex, endBlockIndex, col);
    moveInDirection(matrix, direction, startBlockIndex, endBlockIndex, movePositions, row);
}
void runGame(char* username, unsigned int &usernameCurrentPoints) {
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
}
void saveRecord(const char * username, unsigned int maxPoints) {
    const char* tempFileName = "temp.txt";

    std::ifstream inFile(FILE_NAME);
    std::ofstream outFile(tempFileName);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }
    char line[MAX_LINE_LENGTH];
    while (inFile.getline(line, MAX_LINE_LENGTH)) {
        char currentUsername[MAXNAMESIZE] = " ";
        int currentPoints = 0;
        splitRowFromFile(line, currentUsername, currentPoints);
        if (stringsAreEqual(currentUsername, username)) {
            outFile << currentUsername << " - " << maxPoints << endl;
        }
        else {
            outFile << currentUsername << " - " << currentPoints << endl;
        }
    }
    inFile.close();
    outFile.close();
    remove(FILE_NAME);
    rename(tempFileName, FILE_NAME);
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
        saveRecord(username, usernameCurrentPoints);
    }
}
int strLen(char* str) {
    if (!str) {
        return 0;
    }
    int counter = 0;
    while (*str) {
        counter++;
        str++;
    }
    return counter;
}
int numLen(int number) {
    int counter = 0;
    if (number == 0) {
        return 1;
    }
    while (number) {
        counter++;
        number /= 10;
    }
    return counter;
}
void printDashes(int len) {
    cout << "|";    
    for (int i = 0; i < len; i++) {
        cout << "-";
    }
    cout << "|";
}
void showLeaderBoard() {
    ifstream MyFile(FILE_NAME); // starts from the beginning of the file
    char line[MAX_LINE_LENGTH];
    int playerMessageLen = 22;
    cout << "|--------Player--------|" << " " << "|--------POINTS--------|" << endl;
    while (MyFile.getline(line, MAX_LINE_LENGTH)) {
        char currentUsername[MAXNAMESIZE] = " ";
        int currentPoints = 0;
        splitRowFromFile(line, currentUsername, currentPoints);
        int usernameLen = strLen(currentUsername);
        int sideLen = (playerMessageLen - usernameLen) / 2;
        printDashes(sideLen);
        cout << currentUsername;
        printDashes(sideLen);
        sideLen = (playerMessageLen - numLen(currentPoints)) / 2;
        printDashes(sideLen);
        cout << currentPoints;
        printDashes(sideLen);
        cout << endl;
    }
    MyFile.close();
}
void runOptions(bool &mainLoop) {
    int command;
    cin >> command;
    system("cls");
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
void printHeader() {
    std::cout << "|===============================|" << std::endl;
    std::cout << "|     Welcome to Bricks Drop    |" << std::endl;
    std::cout << "|===============================|" << std::endl;
}
void printDialogue() {
    std::cout << "|===============================|" << std::endl;
    std::cout << "|           Options             |" << std::endl;
    std::cout << "|===============================|" << std::endl;
    std::cout << "|          1. Play              |" << std::endl;
    std::cout << "|===============================|" << std::endl;
    std::cout << "|          2. See Leaderboard   |" << std::endl;
    std::cout << "|===============================|" << std::endl;
    std::cout << "Choose an option (1-2): ";
}
int main()
{
    printHeader();
    bool mainLoop = true;
    while (mainLoop) {
        printDialogue();
        runOptions(mainLoop);
    } 
}
