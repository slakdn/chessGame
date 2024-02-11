#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
using namespace std;
#include "pieces.cpp"


int main(){

    Board chess;

    for(int i=0 ; i<8 ; ++i){
        chess.placePieces(6, i, Pieces('p'));
    }
    
    chess.placePieces(7, 0, Pieces('r'));            //rook
    chess.placePieces(7, 7, Pieces('r'));            //rook
    chess.placePieces(7, 1, Pieces('n'));            //knight
    chess.placePieces(7, 6, Pieces('n'));            //knight
    chess.placePieces(7, 2, Pieces('b'));            //bishop
    chess.placePieces(7, 5, Pieces('b'));            //bishop
    chess.placePieces(7, 3, Pieces('q'));            //queen
    chess.placePieces(7, 4, Pieces('k'));            //king

    for(int i=0 ; i<8 ; ++i){
        chess.placePieces(1, i, Pieces('P'));
    }

    chess.placePieces(0, 0, Pieces('R'));
    chess.placePieces(0, 7, Pieces('R'));
    chess.placePieces(0, 1, Pieces('N'));
    chess.placePieces(0, 6, Pieces('N'));
    chess.placePieces(0, 2, Pieces('B'));
    chess.placePieces(0, 5, Pieces('B'));
    chess.placePieces(0, 3, Pieces('Q'));
    chess.placePieces(0, 4, Pieces('K'));

    chess.printBoard("chessboard.txt");
    chess.nextMove();

    return 0;
    
}