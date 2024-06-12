#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#define BEGINNER 0
#define INTERMEDIATE 1
#define ADVANCED 2
#define MAXSIDE 25
#define MAXMINES 99
#define MOVESIZE 526 // (25 * 25 - 99)

int SIDE; // Length/width board
int MINES; // Number of mines

// Helper function, determines if x,y is in range
bool isValid(int x, int y)
{
    return (x >= 0) && (x < SIDE) && (y >= 0) && (y < SIDE);
}

class Board {
    public:
        char** board;
        Board() {
            board = new char*[MAXSIDE+1];
            for (int i = 0; i<= MAXSIDE; i++){
                board[i] = new char[MAXSIDE + 1];
                for (int j = 0; j <= MAXSIDE; j++){
                    board[i][j] = '-';
                }
            }
        }

        // Check cell for mine
        bool isMine(int x, int y){
            if(board[x][y] == '*'){
                return true;
            } else {
                return false;
            }
        }

        // Get user input
        void userInput(int* x, int* y){
            string move;
            cout << "Enter move (x y): ";
            /* These are reversed as a botch fix lol
            when x, y: the logic in rest of game handles the other way
            so by swapping, account for that
            yay!
            */
            scanf("%d %d", y, x);
            return;
        }

        void printBoard(){
            int i, j;
            cout << "     ";
            // x coords
            for(i = 0; i < SIDE; i++){
                cout << i << " ";
            }
            cout << endl << endl;
            for(i = 0; i < SIDE; i++){
                // y coord
                if(i < 10){
                    cout << i << "  | ";
                } else {
                    cout << i << " | ";

                }

                // cell
                for(j=0; j<SIDE; j++){
                    cout << board[i][j] << " ";
                }
                cout << endl;
            }
        }

        // Counts surrounding mines of given cell
        int adjacentMines(int x, int y, int mines[][2]){
            int i = 0;
            int count = 0;

            /*
            TL  T  TR
            L   X  R
            BL  B  BR

            TL  =  x-1, y-1     Top left
            T   = x, y-1        Top
            TR  = x+1, y-1      Top right
            L   = x-1, y        Left
            X   = x, y          Current cell
            R   = x+1, y        Right
            BL  = x-1, y+1      Bottom left
            B   = x, y+1        Bottom
            BR  = x+1, y+1      Bottom right
            */

            int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1};
            int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

            for (int d=0; d<8; d++){
                int newx = x + dx[d];
                int newy = y + dy[d];

                if(isValid(newx, newy)) {
                    if(isMine(newx, newy)) {
                        count++;
                    }
                }
            }
            return(count);
        }

        // Populate board with mines
        void placeMines(int mines[][2]){
            bool mark[MAXSIDE*MAXSIDE];

            memset(mark, false, sizeof(mark));

            for(int i=0; i<MINES;){
                int random = rand() % (SIDE*SIDE);
                int x = random / SIDE;
                int y = random % SIDE;

                if(mark[random] == false){
                    mines[i][0] = x;
                    mines[i][1] = y;

                    board[mines[i][0]][mines[i][1]] = '*';
                    mark[random] = true;
                    i++;
                }
            }
            return;
        }

    // Replace the mine from x,y and place into vacant space. Used for when first move is on a mine!
    void replaceMine(int x, int y){
        for(int i = 0; i<SIDE; i++){
            for(int j=0; j<SIDE; j++){
                if(board[i][j] != '*'){
                    board[i][j] = '*';
                    board[x][y] = '-';
                    return;
                }
            }
        }
        return;
    }
};


class Game {
public:
    bool playMinesweeperUtil(Board& playerBoard, Board& realBoard, 
                            int mines[][2], int x, int y, 
                            int* movesLeft){

        if(playerBoard.board[x][y] != '-'){
            return false;
        }

        int i, j;

        //Hit a mine -> lose
        if(realBoard.board[x][y] == '*'){
            playerBoard.board[x][y] = '*';
            for(i = 0; i< MINES; i++){
                playerBoard.board[mines[i][0]][mines[i][1]] = '*';
            }
            playerBoard.printBoard();
            cout << endl << "You lost." << endl;
            return true;
        }
        else {
            // Count number of mines and put on board
            int count = realBoard.adjacentMines(x, y, mines);
            (*movesLeft)--;

            playerBoard.board[x][y] = count + '0';

            if(!count){
                    /*
                TL  T  TR
                L   X  R
                BL  B  BR

                TL  =  x-1, y-1     Top left
                T   = x, y-1        Top
                TR  = x+1, y-1      Top right
                L   = x-1, y        Left
                X   = x, y          Current cell
                R   = x+1, y        Right
                BL  = x-1, y+1      Bottom left
                B   = x, y+1        Bottom
                BR  = x+1, y+1      Bottom right
                */

                int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1};
                int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

                for (int d=0; d<8; d++){
                    int newx = x + dx[d];
                    int newy = y + dy[d];

                    if(isValid(newx, newy)) {
                        if(realBoard.isMine(newx, newy) == false) {
                            playMinesweeperUtil(playerBoard, realBoard, 
                                                mines, newx, newy, 
                                                movesLeft);
                        }
                    }
                }
            }
            return false;
        }
    }

    // Reveals mine locations
    void cheat(Board& realBoard){
        cout << "Mine locations:" << endl;
        realBoard.printBoard();
        return;
    }

    void playMinesweeper(Board& realBoard, Board& playerBoard){
        bool gameover = false;

        int movesLeft = SIDE * SIDE - MINES, x, y;
        int mines[MAXMINES][2];

        realBoard.placeMines(mines);

        // cheat(realBoard);

        int currentMoveIndex=0;
        while (gameover == false)
        {
            cout << "Board:" << endl;
            playerBoard.printBoard();
            playerBoard.userInput(&x, &y);

            // Guarantee first move is safe
            if(currentMoveIndex == 0){
                if(realBoard.isMine(x, y)){
                    realBoard.replaceMine(x, y);
                }
            }
            currentMoveIndex++;

            gameover = playMinesweeperUtil(playerBoard, realBoard, 
                                        mines, x, y, &movesLeft);

            if ((gameover == false) && (movesLeft == 0)){
                cout << "You win!" << endl;
                gameover = true;
            }
        }
        return;
    }
};

void getDifficulty()
{
    /*
    --> BEGINNER = 9 * 9 Cells and 10 Mines
    --> INTERMEDIATE = 16 * 16 Cells and 40 Mines
    --> ADVANCED = 24 * 24 Cells and 99 Mines
    */

    int level;

    printf("Enter the Difficulty Level\n");
    printf("Press 0 for BEGINNER (9 * 9 Cells and 10 "
           "Mines)\n");
    printf("Press 1 for INTERMEDIATE (16 * 16 Cells and 40 "
           "Mines)\n");
    printf("Press 2 for ADVANCED (24 * 24 Cells and 99 "
           "Mines)\n");

    scanf("%d", &level);

    if (level == BEGINNER) {
        SIDE = 9;
        MINES = 10;
    }

    if (level == INTERMEDIATE) {
        SIDE = 16;
        MINES = 40;
    }

    if (level == ADVANCED) {
        SIDE = 24;
        MINES = 99;
    }
}

int main(){
    // Choose difficulty
    Board myBoard, realBoard;

    Game* game = new Game();

    getDifficulty();

    game->playMinesweeper(myBoard, realBoard);

    return 0;
}