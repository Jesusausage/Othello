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
    void setScore(float score);
    void setPlayer(char colour);
    void setLocation(int row, int col);
    /* Set a move's score, player and coordinates. */
    
    float score() const;
    char player() const;
    int row() const;
    int col() const;
    /* Return score_, player_, row_ and col_. */
    
private:
    int row_, col_;
    char player_;
    float score_; /* higher means better chance of winning */
};
    

void printBoard(const Piece board[8][8]);

/* Prompt human player move. */
void playerMove(Piece board[8][8], char &player);

bool checkLegalMove(const Piece board[8][8], const Move move);

/* Returns 'W' for 'B', and vice versa. */
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


int main()
{
    Piece board[8][8];
    char player = 'W';
    Piece null; /* The 'null' piece neighbours edge pieces which would otherwise have 
		   out-of-bounds neighbours. */
    null.placePiece(' ');

    setBoard(board, null);

    printBoard(board);
    while (1) {
	playerMove(board, player);
	printBoard(board);
    }
    
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


void Move::setScore(float score) {
    score_ = score; }

void Move::setLocation(int row, int col) {
    row_ = row; col_ = col; }

void Move::setPlayer(char colour) {
    player_ = colour; }

float Move::score() const {
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
    cout << player << "'s move. Your turn.\n";
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
    if (player == 'W')
	return 'B';
    else if (player == 'B')
	return 'W';
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
    if (player == 'W')
	player = 'B';
    else if (player == 'B')
	player = 'W';
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
    
    board[3][3].placePiece('W');
    board[3][4].placePiece('B');
    board[4][3].placePiece('B');
    board[4][4].placePiece('W');
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
