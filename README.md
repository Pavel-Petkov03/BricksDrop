# Bricks Drop Game

## Introduction

Bricks Drop is a console-based game where players strategically manage falling bricks to create solid rows and earn points. The game challenges players to achieve the highest score by making strategic moves.

## Gameplay

1. **Username Entry**: Players enter their username before starting the game.
2. **Game Board**: Bricks fall from the top, and the player must strategically place them.
3. **Controls**:
   - `row col direction movePositions`: Input to move a brick (row, col) - starting position, direction - 'r' or 'l', movePositions - number of positions to move).
4. **Scoring**: Players earn points for each completed row.
5. **Game Over**: The game ends when the bricks reach the top of the board.
6. **Leaderboard**: After each game, the player's score is compared to the leaderboard, and if a record is broken, it is updated.

## Code Structure

1. **User Interaction Functions**:
   - `readUsername`: Reads and validates the player's username.
   - `validateMoveInput`: Validates the input for moving a brick.

2. **Game Logic Functions**:
   - `generateRow`: Generates a row of falling bricks.
   - `printBoard`: Displays the current state of the game board.
   - `makeMove`: Handles player moves.
   - `runGame`: Manages the game loop.
   - `destroyRow`: Destroys a filled row, shifting rows above it down, and awards the player 10 points.
   - `findDestroyableRow`: Finds and destroys the lowest filled row.
   - `allRowShrinked`: Checks if all bricks in a row can be moved down to create space.

3. **File Operations Functions**:
   - `logUser`: Checks if a user is already logged in and updates the player's data.
   - `saveRecord`: Saves the player's record in the leaderboard.

4. **Leaderboard Functions**:
   - `showLeaderBoard`: Displays the current leaderboard.
