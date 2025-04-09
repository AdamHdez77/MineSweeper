#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

const int grid_size = 10;

class MineSweeper {
private:
    int board[grid_size][grid_size];    // The game board (0 for hidden, 1 for revealed)
    int mineBoard[grid_size][grid_size]; // The mine board (0: empty, -1: mine)
    int lives;
    int mineNum;

public:
    MineSweeper(int difficulty): lives(3), mineNum(difficulty){
        for (int row = 0; row < grid_size; row++) {  //Clears and resets boards to hidden
            for (int column = 0; column < grid_size; column++) {
                board[row][column] = 0;
                mineBoard[row][column] = 0;
            }
        }
        placeMines();
    }

    void placeMines() {
        srand(time(nullptr)); //random seed
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

    static void clearScreen() {
        system("clear");
    }

    void displayBoard() {
        clearScreen();

        cout << "  ";
        for (int colNum = 0; colNum < grid_size; ++colNum) { //column reference number
            cout << colNum << " ";
        }
        cout << endl;

        for (int rows = 0; rows < grid_size; ++rows) {
            cout << rows << " "; // row reference number
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


    int countAdjacentMines(int x_cord, int y_cord) { //Counts nearby mines
        int mineCount = 0;
        for (int rowOffset = -1; rowOffset <= 1; ++rowOffset) {      //coordinate offset
            for (int colOffset = -1; colOffset <= 1; ++colOffset) {
                int adjCellX = x_cord + colOffset;
                int adjCellY = y_cord + rowOffset;
                if (adjCellX >= 0 && adjCellX < grid_size && adjCellY >= 0 && adjCellY < grid_size && (rowOffset != 0 || colOffset != 0)) {
                    if (mineBoard[adjCellY][adjCellX] == -1) // if mine the add to counter
                        mineCount++;
                }
            }
        }
        return mineCount;
    }

    bool revealSquare(int x_cord, int y_cord) {
        if (mineBoard[y_cord][x_cord] == -1) { //if it's a mine
            board[y_cord][x_cord] = 1; //revealed square
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
                if (mineBoard[row][column] != -1 && board[row][column] == 0) { //checks if mines or hidden cells are left
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
                    throw 201;
                }
                if (x_cord < 0 || x_cord >= grid_size || y_cord < 0 || y_cord >= grid_size) {
                    throw 202;
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
                        cout << "Congratulations! You've cleared the MineSweeper!" << endl << endl;

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
                        cout << "------Game Over!------" << endl << endl;
                    } else
                        displayBoard();
                }
            } catch (int error) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                ofstream ErrorLog("error_log.txt", ios::app);
                time_t now = time(0);
                ErrorLog << "Error Code: " << error << " | " << ctime(&now) << endl;
                ErrorLog.close();
                cout << "------------Invalid Input------------" << endl;
                continue;
            }
        }
    }
};

static bool optionsScreen(){ //Options screen
    int choice = 0;
    do {
        try{
            cout << "----------------------------------\n"
                    "> Options:\n"
                    "[1] Main Menu\n"
                    "[2] Exit\n"
                    "----------------------------------\n"
                    "Selection: ";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 2)
                throw 301;
        }
        catch(int error){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ofstream ErrorLog("error_log.txt", ios::app);
            time_t now = time(0);
            ErrorLog << "Error Code: " << error << " | " << ctime(&now) << endl;
            ErrorLog.close();
            cout << "------------Invalid Choice!------------" << endl << endl;

        }
    }while(choice < 1 || choice > 2);
    if (choice == 1)
        return true;
    return false;
}

//Menu and Difficulty selection
int MainMenu(){
    int choice = 0;
    do {
        cout << "The MineSweeper's Quest!\n"
            "----------------------------------\n"
            "> Select Difficulty:\n"
            "[1] Easy\n"
            "[2] Medium\n"
            "[3] Hard\n"
            "----------------------------------\n"
            "Selection: ";
        try {
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 3)
                throw 101;
        } catch (int error) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ofstream ErrorLog("error_log.txt", ios::app);
            time_t now = time(0);
            ErrorLog << "Error Code: " << error << " | " << ctime(&now) << endl;
            ErrorLog.close();
            cout << "------------Invalid Choice!------------" << endl << endl;
            choice = 0;
        }
    }while (choice < 1 || choice > 3);
    switch (choice){
        case 1: return 10;
        case 2: return 20;
        case 3: return 30;
        default: return 20;
    }
}

int main() {
    bool running = true;
        while (running){
            int difficulty = MainMenu(); //get difficulty
            MineSweeper game(difficulty);
            game.GamePlay();
           running = optionsScreen();
        }
    return 0;
}