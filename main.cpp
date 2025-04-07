#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

const int grid_size = 10;
const int mineNum = 20;

class MineSweeper {
private:
    int board[grid_size][grid_size];    // The game board (0 for hidden, 1 for revealed)
    int mineBoard[grid_size][grid_size]; // The mine board (0: empty, -1: mine)
    int lives;

public:
    MineSweeper(): lives(3) {
        for (int row = 0; row < grid_size; row++) {
            for (int column = 0; column < grid_size; column++) {
                board[row][column] = 0;
                mineBoard[row][column] = 0;
            }
        }
        placeMines();
    }

    void clearScreen() {
        system("clear");
    }

    void displayBoard() {
        clearScreen();

        cout << "  ";
        for (int colNum = 0; colNum < grid_size; ++colNum) {
            cout << colNum << " ";
        }
        cout << endl;

        for (int rows = 0; rows < grid_size; ++rows) {
            cout << rows << " ";
            for (int columns = 0; columns < grid_size; ++columns) {
                if (board[rows][columns] == 0) {
                    cout << "░ ";
                } else if (mineBoard[rows][columns] == -1) {
                    cout << "█ ";
                } else {
                    cout << mineBoard[rows][columns] << " ";
                }
            }
            cout << endl;
        }
    }

    void placeMines() {
        srand(time(nullptr));

        int minesPlaced = 0;
        while (minesPlaced < mineNum) {
            int x_mine_cord = rand() % grid_size;
            int y_mine_cord = rand() % grid_size;

            if (mineBoard[y_mine_cord][x_mine_cord] == 0) {
                mineBoard[y_mine_cord][x_mine_cord] = -1;
                minesPlaced++;
            }
        }
    }

    int countAdjacentMines(int cellX, int cellY) {
        int mineCount = 0;
        for (int rowCheck = -1; rowCheck <= 1; ++rowCheck) {
            for (int colCheck = -1; colCheck <= 1; ++colCheck) {
                int adjCellX = cellX + colCheck;
                int adjCellY = cellY + rowCheck;
                if (adjCellX >= 0 && adjCellX < grid_size && adjCellY >= 0 && adjCellY < grid_size && (rowCheck != 0 || colCheck != 0)) {
                    if (mineBoard[adjCellY][adjCellX] == -1) {
                        mineCount++;
                    }
                }
            }
        }
        return mineCount;
    }

    bool revealSquare(int x_cord, int y_cord) {
        if (mineBoard[y_cord][x_cord] == -1) {
            board[y_cord][x_cord] = 1;
            return false;
        }

        int adjacentMines = countAdjacentMines(x_cord, y_cord);
        mineBoard[y_cord][x_cord] = adjacentMines;
        board[y_cord][x_cord] = 1;
        return true;
    }

    bool checkWin() {
        for (int row = 0; row < grid_size; ++row) {
            for (int column = 0; column < grid_size; ++column) {
                if (mineBoard[row][column] != -1 && board[row][column] == 0) {
                    return false;
                }
            }
        }
        return true;
    }

    void GamePlay() {
        displayBoard();

        int x_cord, y_cord;
        bool gameOver = false;

        while (!gameOver && lives > 0) {

            cout << "Lives: ";
            for (int counter = 1; counter <= lives; counter++)
                cout << "♥ ";
            for (int counter = 1; counter <= 3 - lives; counter++)
                cout << "♡ ";

            cout << "\nEnter coordinates (row column): ";

            try {
                cin >> x_cord >> y_cord;

                if (cin.fail()) {
                    throw 101;
                }
                if (x_cord < 0 || x_cord >= grid_size || y_cord < 0 || y_cord >= grid_size) {
                    throw 102;
                }
                if (board[y_cord][x_cord] != 0) {
                    cout << "----------Cell Already Revealed----------" << endl;
                    continue;
                }
                else {
                    bool safe = revealSquare(x_cord, y_cord);
                    if (!safe) {
                        --lives;
                        cout << "BOOM! You hit a mine!" << endl;
                    } else
                        cout << "Safe! Revealed adjacent mines: " << mineBoard[y_cord][x_cord] << endl;

                    if (checkWin()) {
                        gameOver = true;
                        cout << "┏━━━━━━━━━━━━━━━━━┓\n"
                                "  > Game Cleared!  \n"
                                "┗━━━━━━━━━━━━━━━━━┛\n";
                        cout << "Congratulations! You've cleared the MineSweeper!" << endl;
                    } else if (lives == 0) {
                        gameOver = true;
                        cout << "░ ░ ░ █ █ █ █ █ ░ ░ ░\n"
                                "░ ░ █ ░ ░ ░ ░ ░ █ ░ ░\n"
                                "░ █ ░ ░ ░ ░ ░ ░ ░ █ ░\n"
                                "█ ░ ░ █ █ ░ █ █ ░ ░ █\n"
                                "█ ░ █ █ █ ░ █ █ █ ░ █\n"
                                "█ ░ █ █ █ ░ █ █ █ ░ █\n"
                                "█ ░ █ █ ░ ░ ░ █ █ ░ █\n"
                                "█ ░ ░ ░ ░ █ ░ ░ ░ ░ █\n"
                                "░ █ ░ ░ ░ ░ ░ ░ ░ █ ░\n"
                                "░ ░ █ ░ █ ░ █ ░ █ ░ ░\n"
                                "░ ░ █ █ █ █ █ █ █ ░ ░\n";
                        cout << "------Game Over!------" << endl;
                    } else
                        displayBoard();
                }
            } catch (int error) {
                cin.clear();
                cin.ignore();
                cout << "---------Invalid Input----------" << endl;
                continue;
            }
        }
    }
};

int main() {
    MineSweeper game;
    game.GamePlay();
    return 0;
}