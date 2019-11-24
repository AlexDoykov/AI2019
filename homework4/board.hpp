#include <vector>
#include <iostream>
#define HUMAN 1
#define COMPUTER 2

class Board{
    private:
        int width;
        int height;
        std::vector<std::vector<int>> board;
        bool isInside(int, int);
    public:
        Board(int, int);
        Board(int);
        int evaluate(int);
        void print();
        bool place(int, int, int);
        bool isTaken(int, int);
        vector<pair<int, int>> getPossibleMoves();
};

Board::Board(int _width, int _height){
    width = _width;
    height = _height;
    board.resize(height);
    for(int i = 0; i < height; i++){
        board[i].resize(width, 0);
    }
}

Board::Board(int size){
    Board(size, size);
}

int calculateScore(int player, int sum){
    int currentSum = 0;
    if(sum == 3){
        currentSum += 100 * player;
    }else if(sum == 2){
        currentSum += 10 * player;
    }else if(sum == 1){
        currentSum += 1 * player;
    }
    return currentSum;
}

int Board::evaluate(int player){
    int sum = 0;
    for(int i = 0; i < height; i++){
        int rowSum = 0;
        for(int j = 0; j < width; j++){
            if(board[i][j] == player){
                rowSum += player;
            }
            if(board[i][j] == player * -1){
                rowSum = 0;
                break;
            }
        }
        sum += calculateScore(player, rowSum);
    }

    for(int i = 0; i < width; i++){
        int columnSum = 0;
        for(int j = 0; j < height; j++){
            if(board[j][i] == player){
                columnSum += player;
            }
            if(board[i][j] == player * -1){
                columnSum = 0;
                break;
            }
        }
        sum  += calculateScore(player, columnSum);
    }

    int diagonalSum = 0;
    for(int i = 0; i < height; i++){
        if(board[i][i] == player){
            diagonalSum += player;
        }
        if(board[i][i] == player * -1){
            diagonalSum = 0;
            break;
        }
    }
    sum += calculateScore(player, diagonalSum);

    diagonalSum = 0;
    for(int i = 0; i < height; i++){
        if(board[i][width - i] == player){
            diagonalSum += 1;
        }
        if(board[i][width - i] == player * -1){
            diagonalSum = 0;
            break;
        }
    }
    sum += calculateScore(player, diagonalSum);

    return sum;   
}

void Board::print(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            std::cout<<board[i][j];
        }
        std::cout<<std::endl;
    }
}

bool Board::isInside(int x, int y){
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Board::isTaken(int x, int y){
    return board[y][x] != 0;
}

bool Board::place(int y, int x, int value){
    if(!isInside(x, y)){
        return false;
    }
    if(isTaken(x, y)){
        return false;
    }
    board[y][x] = value;
    return true;
}

vector<pair<int, int>> Board::getPossibleMoves(){
    vector<pair<int, int>> result;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(board[i][j] == 0){
                result.push_back(make_pair(j, i));
            }
        }
    }
    return result;
}