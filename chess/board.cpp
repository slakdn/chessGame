#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
using namespace std;

class Pieces{

private:
    char piece;

public:
    Pieces(char sembol) : piece(sembol) { }                         //Constructor
    char getSymbol() const {
        return piece;
    }
    void setSymbol(char s) {
        piece = s;
    }
};

class Board{    

private:
    vector<vector<Pieces>> board;

public:

    Board(){                                                    //Constructor
        for(int i=0; i<8; ++i){
            vector<Pieces> row;

            for(int j=0 ; j<8 ; ++j){
                row.push_back(Pieces('.'));
            }
            board.push_back(row);
        }
    }

    void placePieces(int row, int col, const Pieces& piece);
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    void printBoard(const string& filename ) const ;
    void readBoard(const string& filename) ;
    bool underAttack(int row, int col , bool Turn );
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool Turn );
    double calculateScore(bool turn , vector<vector<Pieces>> newBoard );
    void suggestMove(bool turn );
    bool checkmate(bool turn);
    vector<vector<Pieces>> nextMove();

};