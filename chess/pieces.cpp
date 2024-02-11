#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
using namespace std;
#include "board.cpp"

    void Board::placePieces(int row, int col, const Pieces& piece){             //Puts pieces to the array
        if(row>= 0 && row < 8 && col>= 0 && col < 8){
            board[row][col]=piece;
        }
    }

    void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol){         //Moves pieces in the game
       
        if(fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
            toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8){
                board[toRow][toCol] = board[fromRow][fromCol];
                board[fromRow][fromCol] = Pieces('.');
            }
    }

    void Board::printBoard(const string& filename ) const {                         //Prints array to the file
        std::ofstream output(filename);

        if(output.is_open()){
            for(int i=7; i>=0 ; --i){
                output << i+1 << "|";
                for(int j=0 ; j<8 ; ++j){
                    output << board[i][j].getSymbol() << ' ' ;
                }
                output << '\n';
            }
            output << "  ------------------\n";
            output << "  a b c d e f g h\n";
            output.close();
            cout << "Printed to " << filename << endl ;
        }else{
            std::cerr << "Unable to open file" << endl ;
        }
    }

    void Board::readBoard(const string& filename) {                                        //Reads file and saves to the board
        std::ifstream input(filename);

        if (input.is_open()) {
            
            board.clear();
            board.resize(8, vector<Pieces>(8, '.'));

            char symbol;
            for (int i = 7; i >= 0; --i) {
                input >> std::skipws >> symbol; 
                for (int j = 0; j < 8; ++j) {
                    input >> std::skipws >> symbol;
                    board[i][j].setSymbol(symbol);
                }
                input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            }

            input.close();
            cout << "Read from " << filename << endl;
        } else {
            std::cerr << "Unable to open file" << endl;
        }
    }

    bool Board::underAttack(int row, int col , bool Turn ) {               //It returns true if there is any attack for the piece that locates in the row and the col.
        char opponentKing = (Turn) ? 'k' : 'K';
        char opponentPawn = (Turn) ? 'p' : 'P';
        char oppenentBishop = (Turn) ? 'b' : 'B';
        char oppenentQueen = (Turn) ? 'q' : 'Q';
        char oppenentRook =  (Turn) ? 'r' : 'R' ;
        char opponentKnight = (Turn) ? 'n' : 'N';

        auto printAttackingPiece = [Turn](char piece, int attackingRow, int attackingCol) {             //Just for checking the attacking
        //cout << (Turn ? "White's " : "Black's ") << piece << " at " << char('a' + attackingCol) << attackingRow + 1 << " is attacking.\n";            
        };

        // Checks for opponent's pawns
        int pawnRow = (Turn) ? row +1  : row - 1;
        if (pawnRow >= 0 && pawnRow < 8) {
            if (col - 1 >= 0  && board[pawnRow][col - 1].getSymbol() == opponentPawn) {
                printAttackingPiece(opponentPawn, pawnRow, col - 1);
                return true;
            }
            if (col + 1 < 8 && board[pawnRow][col + 1].getSymbol() == opponentPawn) {
                printAttackingPiece(opponentPawn, pawnRow, col + 1);
                return true;
            }
        }

        // Checks for opponent's knights
        int knightMoves[8][2] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1},
                                 {2, 1},   {1, 2},  {-1, 2}, {-2, 1}};
        for (int i = 0; i < 8; ++i) {
            int knightRow = row + knightMoves[i][0];
            int knightCol = col + knightMoves[i][1];
            if (knightRow >= 0 && knightRow < 8 && knightCol >= 0 && knightCol < 8 &&
                board[knightRow][knightCol].getSymbol() == opponentKnight) {
                return true;
            }
        }

        // Checks for opponent's bishops or queens (diagonal)S
        int diagonalMoves[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        for (int i = 0; i < 4; ++i) {
            int step = 1;
            while (true) {
                int bishopRow = row + step * diagonalMoves[i][0];
                int bishopCol = col + step * diagonalMoves[i][1];
                if (bishopRow < 0 || bishopRow >= 8 || bishopCol < 0 || bishopCol >= 8) {
                    break;
                }
                char piece = board[bishopRow][bishopCol].getSymbol();
                if (piece == oppenentBishop || piece == oppenentQueen) {
                    return true;
                }else if(piece != '.'){
                    break; 
                }
                ++step;
            }
        }

        // Checks for opponent's rooks or queens (horizontal and vertical)
        int straightMoves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < 4; ++i) {
            int step = 1;
            while (true) {
                int straightRow = row + step * straightMoves[i][0];
                int straightCol = col + step * straightMoves[i][1];
                if (straightRow < 0 || straightRow >= 8 || straightCol < 0 || straightCol >= 8) {
                    break;
                }
                char piece = board[straightRow][straightCol].getSymbol();
                if (piece == oppenentRook || piece == oppenentQueen) {
                    return true;
                } else if (piece != '.') {
                    break; 
                }
                ++step;
            }
        }

        // Checks for opponent's kings (adjacent squares)
        int kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                               {0, 1},   {1, -1}, {1, 0},  {1, 1}};
        for (int i = 0; i < 8; ++i) {
            int kingRow = row + kingMoves[i][0];
            int kingCol = col + kingMoves[i][1];
            if (kingRow >= 0 && kingRow < 8 && kingCol >= 0 && kingCol < 8 &&
                board[kingRow][kingCol].getSymbol() == opponentKing) {
                return true;
            }
        }

        return false;
    }
    //This function checks that move is valid or not. If it is valid, it returns true otherwise it returns false.
    bool Board::isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool Turn ){

        if(fromCol < 0 || fromCol >= 8 || fromRow < 0 || fromRow >= 8 ||
        toCol < 0 || toCol >= 8 || toRow < 0 || toRow >= 8) {
            return false;
        }

        char start = board[fromRow][fromCol].getSymbol();
        char ch= board[toRow][toCol].getSymbol();

        if( start == '.'){
            return false;
        }

        switch (start)
        {
        case 'P':
            if( Turn && (fromRow == 1 && toRow == 3)){                                     //At the beginning pawns can go 2 step further
                if (ch != '.') {
                    return false;                                           
                }
                Turn = false;
                return true;
            } 
            else if( Turn && ( ((toRow  == fromRow +1 && toCol == fromCol+1 )|| (toRow == fromRow + 1 && toCol + 1 == fromCol ))           //Eating the cross piece
                && (ch != '.') && (islower(ch)))){
                    Turn = false;
                    return true;
            }
            else if ( Turn &&  (toRow == fromRow + 1 && toCol == fromCol)){                     //Basic move which is going 1 step further 
                if(ch != '.' ){                 
                    return false;
                }else{
                    Turn = false;
                    return true;
                }
            }
            break;

        case 'p':       
            if( !Turn && (fromRow == 6 && toRow == 4)){                                                      
                if (ch != '.') {
                    return false;                                   
                }
                Turn = true;
                return true;                                             
            }
            else if( !Turn &&  ((toRow + 1 == fromRow && toCol -1 == fromCol )|| (toRow + 1 == fromRow && toCol + 1 == fromCol ))      
                && (ch != '.' ) && (isupper(ch)) ){
                    Turn= true;
                    return true;
            }
            else if( !Turn && (toRow == fromRow - 1 && toCol == fromCol)){
                if(ch != '.' ){  
                    return false;
                }
                else{
                    Turn = true;
                    return true;
                }
            }
            break;

        case 'R':
            
            if (Turn && (fromRow == toRow && fromCol != toCol)) {                           //Valid horizontal move
                int step;
                if (toCol > fromCol) step = 1;
                else  step = -1;

                for (int col = fromCol + step; col != toCol; col += step) {         
                    if (board[fromRow][col].getSymbol() != '.') {                       
                        return false;
                    }
                }
                if(ch != '.' && (islower(ch))){                                         
                    Turn = false;
                    return true;
                }
                else if(ch != '.' && !(islower(ch))){                                   
                    return false;
                }   
                Turn = false;                                                       
                return true;                                                            

            } else if (Turn && ( fromCol == toCol && fromRow != toRow)) {               //Valid vertical move
                int step;
                if (toRow > fromRow) step = 1;
                else  step = -1;
                
                for (int row = fromRow + step; row != toRow; row += step) {
                    if (board[row][fromCol].getSymbol() != '.') {
                        return false;
                    }
                }
                if(ch != '.' && (islower(ch))){                                                  
                    Turn = false;
                    return true; 
                }
                else if(ch != '.' && !(islower(ch))){                                       
                    return false;
                }                                                                         
                Turn = false;
                return true;                                                               
            }
            break;

        case 'r':
            if ( !Turn && (fromRow == toRow && fromCol != toCol)) {
                int step;
                if (toCol > fromCol) step = 1;
                else  step = -1;

                for (int col = fromCol + step; col != toCol; col += step) {             
                    if ( board[fromRow][col].getSymbol() != '.') {
                        return false;
                    }
                }
                if(ch != '.' && (isupper(ch))){                                        
                    Turn = true;
                    return true;
                }
                else if(ch != '.' && !(isupper(ch))){                                   
                    return false;
                }                                                                          
                Turn = true;
                return true;                                                            

            } else if (!Turn && (fromCol == toCol && fromRow != toRow)) {
                int step;
                if (toRow > fromRow) step = 1;
                else  step = -1;

                for (int row = fromRow + step; row != toRow; row += step) {         
                    if (board[row][fromCol].getSymbol() != '.') {
                        return false;
                    }
                }
                if(ch != '.' && (isupper(ch))){                                             
                    Turn = true;
                    return true;
                }
                else if(ch != '.' && !(isupper(ch))){                                       
                    return false;
                }                                                                           
                Turn = true;
                return true;                                                               
            }
            break;


        case 'N':

            if( Turn && ((std::abs(toRow - fromRow) == 2 && std::abs(toCol - fromCol) == 1) ||
             (std::abs(toRow - fromRow) == 1 && std::abs(toCol - fromCol) == 2))){
                if(ch!= '.' && islower(ch)){                                                     
                    Turn=false;
                    return true;
                }
                else if(ch != '.' && !(islower(ch))){
                    return false;
                }
                Turn=false;
                return true;
            }
            break;
        
        case 'n':
            
            if(!Turn && ((std::abs(toRow - fromRow) == 2 && std::abs(toCol - fromCol) == 1)
             || (std::abs(toRow - fromRow) == 1 && std::abs(toCol - fromCol) == 2))){
                if(ch!= '.' && isupper(ch)){
                    Turn=true;
                    return true;    
                }
                else if(ch != '.' && !(isupper(ch))){
                    return false;
                }
                Turn=true;
                return true;
            }
            break;

        case 'B':
            if( Turn &&(abs(fromRow - toRow) == abs(fromCol - toCol)) ){
                int rowStep = (toRow > fromRow) ? 1 : -1;
                int colStep = (toCol > fromCol) ? 1 : -1;
                    
                for(int row = fromRow + rowStep , col = fromCol + colStep ; row != toRow && col != toCol ; row += rowStep, col += colStep ){
                    if(board[row][col].getSymbol() != '.') return false;
                }
                if(ch != '.' && islower(ch)){
                    Turn = false;
                    return true;
                }
                else if(ch != '.' && !(islower(ch))){                                   
                    return false;
                } 
                Turn = false;
                return true; 
            }
            break;

        case 'b':
            if( !Turn && (abs(fromRow - toRow) == abs(fromCol - toCol))){
                int rowStep = (toRow > fromRow) ? 1 : -1;
                int colStep = (toCol > fromCol) ? 1 : -1;
                    
                for(int row = fromRow + rowStep , col = fromCol + colStep ; row != toRow && col != toCol ; row += rowStep, col += colStep ){
                    if(board[row][col].getSymbol() != '.') return false;
                }
                if(ch != '.' && isupper(ch)){
                    Turn = true;
                    return true;
                }
                else if(ch != '.' && !(isupper(ch))){                                   
                    return false;
                } 
                Turn = true;
                return true; 
            }
            break;
        

        case 'Q':
            if( Turn && (fromRow == toRow || fromCol == toCol || abs(toRow - fromRow) == abs(toCol - fromCol))){
                int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
                int colStep = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;

                for (int row = fromRow + rowStep, col = fromCol + colStep;
                    row != toRow && col != toCol;
                    row += rowStep, col += colStep) {
                    if (board[row][col].getSymbol() != '.') {
                        return false;
                    }
                }

                if (ch != '.' && islower(ch)) {
                    Turn = false;
                    return true;
                } else if (ch != '.' && !islower(ch)) {
                    return false;
                }

                Turn = false;
                return true;
            }
            break;
        
        case 'q' :
            if( !Turn && (fromRow == toRow || fromCol == toCol || abs(toRow - fromRow) == abs(toCol - fromCol))){
                int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
                int colStep = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;

                for (int row = fromRow + rowStep, col = fromCol + colStep;
                    row != toRow && col != toCol;
                    row += rowStep, col += colStep) {
                    if (board[row][col].getSymbol() != '.') {
                        return false;
                    }
                }

                if (ch != '.' && isupper(ch)) {
                    Turn = true;
                    return true;
                } else if (ch != '.' && !isupper(ch)) {
                    return false;
                }

                Turn = true;
                return true;
            }
            break;

        case 'K':

            if( Turn &&  (abs(toRow - fromRow) <=1 && abs(toCol - fromCol) <= 1)){
                if(ch != '.' && islower(ch)){
                    Turn=false;
                    return true;
                }
                else if(ch != '.' && !(islower(ch))){
                    return false;
                }
                Turn=false;
                return true;
            }
            break;
        
        case 'k' :

            if( !Turn && ((toRow - fromRow) <=1 && abs(toCol - fromCol) <= 1)){
                if(ch != '.' && isupper(ch)){
                    Turn=true;
                    return true;
                }
                else if(ch != '.' && !(isupper(ch))){              
                    return false;
                }
                Turn=true;
                return true;
            }
            break;

        default:
            return false;
        }
    
        return false;
    }

    //It calculates the score according to the under attacking . I use this function in suggestion part and also in regular movements.
    double Board::calculateScore(bool turn , vector<vector<Pieces>> newBoard ){
        
        double score = 0;
        if(turn == true){
            for(int i=0  ; i<8 ; ++i){
                for(int j=0 ; j<8; ++j){
                    char ch = newBoard[i][j].getSymbol();
                    if(isupper(ch)){
                        bool under = underAttack(i, j, turn);
                        double current = 0;
                        switch (ch)
                        {
                        case 'P':
                            if(under) current +=0.5;
                            else current += 1.0;
                            break;
                        case 'R':
                            if(under) current += 2.5;
                            else current += 5.0;
                            break;
                        case 'N':
                            if(under) current += 1.5;
                            else current += 3.0;
                            break;
                        case 'B':
                            if(under) current += 1.5;
                            else current += 3.0;
                            break;
                        case 'Q':
                            if(under) current += 4.5;
                            else current += 9.0;
                            break;
                        default:
                            break;
                        }
                        score +=current;
                    }
                }
            }
        }
        else if(turn == false){
            for(int i=0 ; i<8 ; ++i){
                for(int j=0; j<8 ; ++j){
                    char ch = newBoard[i][j].getSymbol();
                    if(islower(ch)){
                        bool under = underAttack(i, j, turn );
                        double current = 0;
                        switch (ch)
                        {
                        case 'p':
                            if(under) current +=0.5;
                            else current += 1.0;
                            break;
                        case 'r':
                            if(under) current +=2.5;
                            else current +=5.0;
                            break;
                        case 'n':
                            if(under) current +=1.5;
                            else current +=3.0;
                            break;
                        case 'b':
                            if(under) current +=1.5;
                            else current +=3.0;
                            break;
                        case 'q':
                            if(under) current +=4.5;
                            else current +=9.0;
                            break;
                        default:
                            break;
                        }
                        score+=current;
                    }
                }
            } 
        }

        return score; 
    }

    void Board::suggestMove(bool turn ){               //This function suggest a valid move for the given turn .

        int bestScore=0;
        int bestScore2 = 0;
        string bestMove;

        int WhitePawnMoves[6][2] = {{1 ,-1}, {1,1}, {1,0}};
        int diagonalMoves[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        int QueenMoves[8][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}, {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        int RookMoves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int KnightMoves[8][2] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1},   {1, 2},  {-1, 2}, {-2, 1}};
        int kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1},   {1, -1}, {1, 0},  {1, 1}};
        int BlackPawnMoves[3][2] = { {-1,1}, {-1,-1}, {-1,0}};

        if(turn == true){
        for(int i=0 ; i<8 ; i++){
            for(int j=0 ; j<8 ;  j++){
                char ch = board[i][j].getSymbol();
                //if(isupper(ch)){
                    switch (ch){

                        case 'P':

                        for(int x=0 ; x<3 ; ++x){
                            int pawnRow =  i + WhitePawnMoves[x][0];
                            int pawnCol = j+ WhitePawnMoves[x][1];

                            if(  (isValidMove(i, j , pawnRow , pawnCol, turn)) ){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[pawnRow][pawnCol] = ch;
                                newBoard[i][j]='.';
                                int current = calculateScore(turn , newBoard);
                                if(current > bestScore){
                                    bestScore = current;
                                    bestMove = char('a' + j )  + to_string(i + 1) + char( 'a' + pawnCol) + to_string(pawnRow + 1 );
                                }
                            }
                            
                        }
                        break;

                        case 'B':

                        for(int x=0 ; x<4 ; ++x){
                            int step=1;                                     
                            int diagonalRow= i + step*diagonalMoves[x][0];
                            int diagonalCol = j + step*diagonalMoves[x][1];
                            if( (isValidMove(i , j , diagonalRow , diagonalCol, turn))){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[diagonalRow][diagonalCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i + 1) + char('a' + diagonalCol) + to_string(diagonalRow + 1);
                                }
                            }
                            step++;
                        }
                        break;

                        case 'Q':

                        for(int x=0 ; x<8 ; ++x ){
                            int step=1;
                            int queenRow = i + step*QueenMoves[x][0];
                            int queenCol = j + step*QueenMoves[x][1];
                            if( (isValidMove(i ,j , queenRow , queenCol, turn)) ){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[queenRow][queenCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i + 1) + char('a' + queenCol) + to_string(queenRow + 1);
                                }
                            }
                            step++; 
                        }
                        break;

                        case 'R':

                        for(int x=0 ; x<4 ; ++x){
                            int step=1;
                            int rookRow = i + step * RookMoves[x][0];
                            int rookCol= j + step * RookMoves[x][1];
                            if( (isValidMove(i,j,rookRow,rookCol, turn)) ){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[rookRow][rookCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i+1) + char( 'a' + rookCol) + to_string(rookRow + 1);
                                }
                            }
                            step++;
                        }
                        break;

                        case 'N':

                        for(int x=0 ; x<8 ; ++x){
                            int step=1;
                            int knightRow = i + KnightMoves[x][0];
                            int knightCol = j + KnightMoves[x][1];
                            if( (isValidMove(i,j,knightRow ,knightCol, turn))){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[knightRow][knightCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i + 1) + char('a' + knightCol) + to_string(knightRow + 1);
                                }
                            }
                            step++;
                        }
                        break;

                        case 'K':
                        
                        for(int x=0; x < 8 ; ++x){
                            int step=1;
                            int kingRow= i + kingMoves[x][0];
                            int kingCol = j + kingMoves[x][1];
                            if( (isValidMove(i,j,kingRow, kingCol, turn)) ){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[kingRow][kingCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i +1) + char('a' + kingCol) + to_string(kingRow + 1);
                                }
                            }
                            step++;
                        }
                        
                    default:
                        break;
                    } 
                
            }   
        }
        }else if(turn == false){
            for(int i=0 ; i<8 ; i++){
                for(int j=0 ; j<8 ;  j++){
                    char ch = board[i][j].getSymbol();
                    if(islower(ch)){
                        switch (ch){

                        case 'p':                                                       

                        for(int x=0 ; x<3 ; ++x){
                            int pawnRow =  i + BlackPawnMoves[x][0];
                            int pawnCol = j+ BlackPawnMoves[x][1];

                            if(  (isValidMove(i, j , pawnRow , pawnCol, turn)) ){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[pawnRow][pawnCol] = ch;
                                newBoard[i][j]='.';
                                int current = calculateScore(turn , newBoard);
                                if(current > bestScore2){
                                    bestScore = current;
                                    bestMove = char('a' + j )  + to_string(i + 1) + char( 'a' + pawnCol) + to_string(pawnRow + 1 );
                                }
                            } 
                        }
                        break;

                        case 'b':

                        for(int x=0 ; x<4 ; ++x){
                            int step=1;                                     
                            int diagonalRow= i + step*diagonalMoves[x][0];
                            int diagonalCol = j + step*diagonalMoves[x][1];
                            if( (isValidMove(i , j , diagonalRow , diagonalCol, turn))){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[diagonalRow][diagonalCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore2){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i + 1) + char('a' + diagonalCol) + to_string(diagonalRow + 1);
                                }
                            }
                            step++;
                        }
                        break;

                        case 'q':

                        for(int x=0 ; x<8 ; ++x ){
                            int step=1;
                            int queenRow = i + step*QueenMoves[x][0];
                            int queenCol = j + step*QueenMoves[x][1];
                            if( (isValidMove(i ,j , queenRow , queenCol, turn)) ){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[queenRow][queenCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore2){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i + 1) + char('a' + queenCol) + to_string(queenRow + 1);
                                }
                            }
                            step++; 
                        }
                        break;

                        case 'r':

                        for(int x=0 ; x<4 ; ++x){
                            int step=1;
                            int rookRow = i + step * RookMoves[x][0];
                            int rookCol= j + step * RookMoves[x][1];
                            if( (isValidMove(i,j,rookRow,rookCol ,turn)) ){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[rookRow][rookCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore2){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i+1) + char( 'a' + rookCol) + to_string(rookRow + 1);
                                }
                            }
                            step++;
                        }
                        break;

                        case 'n':

                        for(int x=0 ; x<8 ; ++x){
                            int step=1;
                            int knightRow = i + KnightMoves[x][0];
                            int knightCol = j + KnightMoves[x][1];
                            if( (isValidMove(i,j,knightRow ,knightCol, turn))){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[knightRow][knightCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore2){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i + 1) + char('a' + knightCol) + to_string(knightRow + 1);
                                }
                            }
                            step++;
                        }
                        break;

                        case 'k':
                        
                        for(int x=0; x < 8 ; ++x){
                            int step=1;
                            int kingRow= i + kingMoves[x][0];
                            int kingCol = j + kingMoves[x][1];
                            if( (isValidMove(i,j,kingRow, kingCol, turn)) ){
                                vector<vector<Pieces>> newBoard;
                                newBoard = board;
                                newBoard[kingRow][kingCol] = ch;
                                newBoard[i][j] = '.';
                                int current= calculateScore(turn , newBoard);
                                if(current > bestScore2){
                                    bestScore = current;
                                    bestMove = char('a' + j) + to_string(i +1) + char('a' + kingCol) + to_string(kingRow + 1);
                                }
                            }
                            step++;
                        }
                        break;
                        
                        default:
                            break;
                        } 
                    }
                }   
            }
        }
        cout <<  bestMove << endl;
    }

    bool Board::checkmate(bool turn){                               //Returns true if king is under attack otherwise it returns false.
        bool check ; 

        if(turn == true){                                   
            for(int i=0 ; i<8 ; ++i){
                for(int j=0 ; j<8 ; ++j){
                    if(board[i][j].getSymbol() == 'K'){
                        int kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1},   {1, -1}, {1, 0},  {1, 1}};
                        int danger=0;

                        for( int x=0 ; x<8 ; x++){
                            check = underAttack(i,j, turn);
                            if(check == true ) danger++;
                        }
                        if(danger == 8) return true;
                    }
                }
            }
        }else{                                                  
            for(int i=0 ; i<8 ; ++i){
                for(int j=0 ; j<8 ; ++j){
                    if(board[i][j].getSymbol() == 'k'){
                        int kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1},   {1, -1}, {1, 0},  {1, 1}};
                        int danger=0;

                        for( int x=0 ; x<8 ; x++){
                            check = underAttack(i,j, turn);
                            if(check == true ) danger++;
                        }
                        if(danger == 8) return true;
                    }
                }
            }
        }
        return false;
    }

    vector<vector<Pieces>> Board::nextMove(){                  //Next move function is the function that game is playing. It gets moves from user until the checkmate is happening. 
        string move;
        bool turn = true; 

        while(true){

            cout << (turn ? "White" : "Black") << "'s score is: " << calculateScore(turn, board) << endl;
            cout << (turn ? "White" : "Black") << "'s move: " ;
            cin >> move;

            if( move == "suggest"){
                suggestMove(turn);
                turn = !turn;
            }
            else if(move.length() != 4){
                cerr << "Invalid input !!" << endl;
                exit(0);
            }
            else{
                int fromCol = move[0] - 'a';
                int fromRow =( move[1] - '1');
                int toCol = move[2] -'a';
                int toRow = (move[3]- '1'); 

                if(isValidMove(fromRow, fromCol, toRow, toCol, turn)){

                    movePiece(fromRow, fromCol, toRow, toCol);
                    printBoard("chessboard.txt");
                    
                    if(checkmate(turn)){
                        cout << (turn ? "White" : "Black") << " is in checkmate. Game over!" << endl;
                        break;
                    }
                    turn = ! turn;
                }
                else{
                    cout << "Invalid!! \n";
                }
            }
        }
        return board;
    }

