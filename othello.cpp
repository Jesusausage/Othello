#include <iostream>

using namespace std;


class Piece {
public:
    void placePiece(char player);
    void setNeighbours(Piece board[8][8], int row, int col, Piece null);
    void setLocation(int row, int col);
    /* Set a piece's colour, neighbours and coordinates. */
    
    char colour() const;
    Piece neighbour(int dir) const;
    int row() const;
    int col() const;
    /* Simple functions which return colour_, *neighbour_[dir] (dereferenced), row_ and col_. */
 
private:
    int row_, col_;
    char colour_;
    Piece *neighbour_[8]; /* An array of pointers 
			    to each of the piece's eight surrounding neighbours, where
			    neighbour[0] is directly above, then they progress anticlockwise. */
};


class Move {
public:
    void setScore(int score);
    void setPlayer(char colour);
    void setLocation(int row, int col);
    /* Set a move's score, player and coordinates. */
    void setVar(int score, char player, int row, int col);
    
    int score() const;
    char player() const;
    int row() const;
    int col() const;
    /* Return score_, player_, row_ and col_. */
    
private:
    int row_, col_;
    char player_;
    int score_; /* higher means better chance of winning */
};
    

void printBoard(const Piece board[8][8]);

/* Prompt human player move. */
void playerMove(Piece board[8][8], char &player);

bool checkLegalMove(const Piece board[8][8], const Move move);

/* Returns 'O' for 'X', and vice versa. */
char opponent(char player);

/* Detection for an unbroken line of opponent pieces, 
   immediately followed by a friendly piece (in direction 'dir'). */
bool checkLine(Piece piece, int dir, char player);

void swapPlayer(char &player);

/* Sets the initial board state, with out-of-bounds neighbours set to null. */
void setBoard(Piece board[8][8], Piece null);

/* Flip the required lines of pieces after a legal move. */
void transformBoard(Piece board[8][8], Move move);

/* Flip a single line of opponent's pieces in direction 'dir'  after a legal move. */
bool transformLine(Piece piece, int dir, char player, Piece board[8][8]);

char checkWinner(const Piece board[8][8], char player);

/* Count piece differential. */
int countDiff(const Piece board[8][8], char player);

void setBoardState(Piece copy[8][8], Piece paste[8][8]);

Move minimax(Piece board[8][8], char player, int depth,
	     bool maximise, int inf, int sup, bool &cut);

void cpuMove(Piece board[8][8], char &player);

int calculateScore(const Piece board[8][8], char player);


int main()
{
    Piece board[8][8];
    char player = 'O';
    Piece null; /* The 'null' piece neighbours edge pieces which would otherwise have 
		   out-of-bounds neighbours. */
    null.placePiece(' ');

    setBoard(board, null);

    printBoard(board);
    while (!checkWinner(board, player)) {
	if (player == 'O')
	    playerMove(board, player);
	else
	    cpuMove(board, player);
	
	printBoard(board);
    }

    cout << "The winner is " << checkWinner(board, player) << ".\n";
    
    return 0;
}


void Piece::placePiece(const char player) {
    colour_ = player; }

void Piece::setLocation(int row, int col) {
    row_ = row; col_ = col; }

void Piece::setNeighbours(Piece board[8][8], int row, int col, Piece null)
{
    if (row >= 1)
	neighbour_[0] = &board[row-1][col];
    else neighbour_[0] = &null;
    
    if (row>=1 && col>=1)
	neighbour_[1] = &board[row-1][col-1];
    else neighbour_[1] = &null;
    
    if (col >= 1)
	neighbour_[2] = &board[row][col-1];
    else neighbour_[2] = &null;
    
    if (row<=6 && col>=1)
	neighbour_[3] = &board[row+1][col-1];
    else neighbour_[3] = &null;
    	     
    if (row <= 6)
	neighbour_[4] = &board[row+1][col];
    else neighbour_[4] = &null;
    
    if (row<=6 && col<=6)
	neighbour_[5] = &board[row+1][col+1];
    else neighbour_[5] = &null;
    
    if (col <= 6)
	neighbour_[6] = &board[row][col+1];
    else neighbour_[6] = &null;
    
    if (row>=1 && col<=6)
	neighbour_[7] = &board[row-1][col+1];
    else neighbour_[7] = &null;
    /* A piece's neighbours are set to the surrounding eight pieces, unless they are 
       out of bounds, in which case the neighbour is set to the null piece. */
}

char Piece::colour() const {
    return colour_; }

Piece Piece::neighbour(int dir) const {
    return *neighbour_[dir]; }

int Piece::row() const {
    return row_; }

int Piece::col() const {
    return col_; }


void Move::setScore(int score) {
    score_ = score; }

void Move::setLocation(int row, int col) {
    row_ = row; col_ = col; }

void Move::setPlayer(char colour) {
    player_ = colour; }

void Move::setVar(int score, char player, int row, int col) {
    score_ = score; player_ = player; row_ = row; col_ = col; }

int Move::score() const {
    return score_; }

int Move::row() const {
    return row_; }

int Move::col() const {
    return col_; }

char Move:: player() const {
    return player_; }


void printBoard(const Piece board[8][8])
{
    cout << "\n   ";
    for (int i=1; i<=8; i++)
	cout << "  " << i << " ";
    cout << endl;
    /* Prints the column labels. */
    
    for (int rows=0; rows<8; rows++) {
	cout << "   ";
	for (int i=0; i<8; i++)
	    cout << "+---";
	char row = rows + 'A';
	cout << "+\n " << row << " |";

	for (int cols=0; cols<8; cols++)
	    cout << " " << board[rows][cols].colour() << " |";	
	cout << "\n";
    }
    /* Prints a horizontal board line, then each row and their contents. */
	
    cout << "   ";
    for (int i=0; i<8; i++)
	cout << "+---";
    cout << "+\n\n";
    /* Prints the bottom line of the board. */
}


void playerMove(Piece board[8][8], char &player)
{
    char tile[2];
    Move move;

    move.setPlayer(player);
    cout << player << "'s turn to move (player).\n";		    
    cout << "Enter coordinates to make a move: ";
    cin >> tile;
    move.setLocation(tile[0]-'A', tile[1]-'1');
    
    while (!checkLegalMove(board, move)) {
	cout << "Illegal move. Please enter another location: ";
	cin >> tile;
	move.setLocation(tile[0]-'A', tile[1]-'1');
    }
    /* Continue asking for a move until a legal position is chosen. */

    transformBoard(board, move);    
    swapPlayer(player);
    /* Places the required pieces on the board, then swaps player. */
}


void cpuMove(Piece board[8][8], char &player)
{
    cout << player << "'s turn to move (CPU).\n";
    cout << "Calculating move...\n\n";

    bool cut = 0;
    int inf = -9999, sup = 9999;
    Move move = minimax(board, player, 7, 1, inf, sup, cut);
    move.setPlayer(player);
    char bestrow = move.row()+'A';
    char bestcol = move.col()+'1';
    cout << "CPU chooses " << bestrow << bestcol << ".\n";
    
    transformBoard(board, move);
    swapPlayer(player);
}
   

bool checkLegalMove(const Piece board[8][8], const Move move)
{
    if (move.row()<0 || move.row()>7 || move.col()<0 || move.col()>7)
	return 0;
    if (board[move.row()][move.col()].colour() != ' ')
	return 0;
    /* Basic out of bounds check and empty-tile check first. */

    for (int dir=0; dir<8; dir++) {
	if ( board[move.row()][move.col()].neighbour(dir).colour() == opponent(move.player()) ) {
	    if ( checkLine(board[move.row()][move.col()].neighbour(dir), dir, move.player()) )
		return 1;
	}
    }
    /* In each direction,
       if the neighbour in that direction is an opponent's piece, then:
           if the checkLine condition is satisfied, then return true. */
    
    return 0;
}


char opponent(char player)
{
    if (player == 'O')
	return 'X';
    else if (player == 'X')
	return 'O';

    return ' ';
}


bool checkLine(Piece piece, int dir, char player)
{
    if (piece.neighbour(dir).colour() == opponent(player))
	return checkLine(piece.neighbour(dir), dir, player);
    /* If the neighbour in direction 'dir' is an opponent's piece, then:
           recursively call checkLine on that neighbour. */

    if (piece.neighbour(dir).colour() == player) {
	return 1;
    /* If the neighbour in direction 'dir' is a friendly piece, then:
           terminate the recursion and return true. */
    }

    return 0;
    /* If no friendly piece is ever detected, terminate the recursion and return false. */
}


void swapPlayer(char &player)
{
    if (player == 'O')
	player = 'X';
    else if (player == 'X')
	player = 'O';
}


void setBoard(Piece board[8][8], Piece null)
{   
    for (int rows=0; rows<8; rows++) {
	for(int cols=0; cols<8; cols++) {
	    board[rows][cols].placePiece(' ');
	    board[rows][cols].setNeighbours(board, rows, cols, null);
	    board[rows][cols].setLocation(rows, cols);
	}
    }
    /* Initialises the board state to empty and sets each piece's neighbours and coordinates. */
    
    board[3][3].placePiece('O');
    board[3][4].placePiece('X');
    board[4][3].placePiece('X');
    board[4][4].placePiece('O');
    /* Places the beginning four pieces on the board. */
}


void transformBoard(Piece board[8][8], Move move)
{
    for (int dir=0; dir<8; dir++) {
	if ( board[move.row()][move.col()].neighbour(dir).colour() == opponent(move.player()) )
	    transformLine(board[move.row()][move.col()], dir, move.player(), board);
    }
    /* In each direction,
       if the neighbour in that direction is an opponent's piece, then:
           flip that line of pieces (only occurs if line is followed by a friendly piece). */
}


bool transformLine(Piece piece, int dir, char player, Piece board[8][8])
{
    if (piece.neighbour(dir).colour() == opponent(player)) {
	if (transformLine(piece.neighbour(dir), dir, player, board)) {
	    board[piece.row()][piece.col()].placePiece(player);
	    return 1;
	}
    }
    /* If the piece's neighbour is an opponent's piece, then:
           recursively call this function, and if it terminates true,
	   flip all pieces along the way. */

    if (piece.neighbour(dir).colour() == player) {
	board[piece.row()][piece.col()].placePiece(player);
	return 1;
    }
    /* Only terminates true if a friendly piece is detected at the end of the line. */

    return 0;
}


char checkWinner(const Piece board[8][8], char player)
{
    Move move;
    move.setPlayer(player);

    for (int rows=0; rows<8; rows++) {
	for (int cols=0; cols<8; cols++) {	    
	    move.setLocation(rows, cols);
	    if (checkLegalMove(board, move))
		return 0;
	}
    }
    /* If a legal move is detected anywhere on the board, return false. */

    int counter = countDiff(board, player);
    if (counter > 0)
	return player;
    else if (counter < 0)
	return opponent(player);
    else
	return 'D';
    /* Return the player with more pieces in the end. */
}


int countDiff(const Piece board[8][8], char player)
{
    int counter=0;
    
    for (int rows=0; rows<8; rows++) {
	for (int cols=0; cols<8; cols++) {
	    if (board[rows][cols].colour() == player)
		counter++;
	    else if (board[rows][cols].colour() == opponent(player))
		counter--;
	}
    }
    /* Counts piece difference between the player and the opponent (positive is better). */

    return counter;
}


void setBoardState(Piece copy[8][8], Piece paste[8][8])
{
    for (int rows=0; rows<8; rows++) {
	for (int cols=0; cols<8; cols++) {
	    paste[rows][cols] = copy[rows][cols];
	}
    }
}


Move minimax(Piece board[8][8], char player, int depth,
	     bool maximise, int inf, int sup, bool &cut)
{
    Move best_move;

    if (depth  == 0) {
	best_move.setScore(calculateScore(board, opponent(player)));
	return best_move;
    }
    
    Piece state[8][8];
    setBoardState(board, state);
    		
    if (maximise) {
	best_move.setScore(-9999);
		    
	for (int rows=0; rows<8; rows++) {
	    if (cut) { cut = 0;	break; }
	    for (int cols=0; cols<8; cols++) {
		if (cut) break; 
		
		Move move;
		move.setVar(0, player, rows, cols);
		if (checkLegalMove(board, move)) {
		    
		    transformBoard(board, move);	    
		    move = minimax(board, opponent(player), depth-1, 0, inf, sup, cut);
		    if (move.score() > best_move.score())
			best_move.setVar(move.score(), player, rows, cols);
		    if (best_move.score() > inf)
			inf = best_move.score();
		    if (inf >= sup)
			cut = 1;
		    setBoardState(state, board);

		}
		
	    }
	}
	return best_move;
		    
    } else {
	best_move.setScore(9999);

	for (int rows=0; rows<8; rows++) {
	    if (cut) { cut = 0;	break; }
	    for (int cols=0; cols<8; cols++) {
		if (cut) break;
		
		Move move;
		move.setVar(0, player, rows, cols);
		if (checkLegalMove(board, move)) {
		    
		    transformBoard(board, move);		    
		    move = minimax(board, opponent(player), depth-1, 1, inf, sup, cut);
		    if (move.score() < best_move.score())
			best_move.setVar(move.score(), player, rows, cols);
		    if (best_move.score() < sup)
			sup = best_move.score();
		    if (inf >= sup)
			cut = 1;
		    setBoardState(state, board);
		    
		}
		
	    }
	}
	return best_move;

    }
}


int calculateScore(const Piece board[8][8], char player)
{
    int counter=0;

    char winner = checkWinner(board, player);
    if (winner == player)
	return 9999;
    else if (winner == opponent(player))
	return -9999;
    
    for (int rows=1; rows<7; rows++) {
	for (int cols=1; cols<7; cols++) {
	    if (board[rows][cols].colour() == player)
		counter++;
	    else if (board[rows][cols].colour() == opponent(player))
		counter--;
	}
    }

    if (board[0][0].colour() == player)
	counter += 28;
    else if (board[0][0].colour() == opponent(player))
	counter -= 28;
    if (board[7][7].colour() == player)
	counter += 28;
    else if (board[7][7].colour() == opponent(player))
	counter -= 28;
    if (board[0][7].colour() == player)
	counter += 28;
    else if (board[0][7].colour() == opponent(player))
	counter -= 28;
    if (board[7][0].colour() == player)
	counter += 28;
    else if (board[7][0].colour() == opponent(player))
	counter -= 28;

    for (int i=1; i<7; i++) {
	if (board[0][i].colour() == player)
	    counter += 4;
	else if (board[0][i].colour() == opponent(player))
	    counter -= 4;
	if (board[7][i].colour() == player)
	    counter += 4;
	else if (board[7][i].colour() == opponent(player))
	    counter -= 4;
	if (board[i][0].colour() == player)
	    counter += 4;
	else if (board[i][0].colour() == opponent(player))
	    counter -= 4;
	if (board[i][7].colour() == player)
	    counter += 4;
	else if (board[i][7].colour() == opponent(player))
	    counter -= 4;
    }
    
    return counter;
}
