#include <iostream>

using namespace std;


class Piece {
public:
    void place_piece(char player); /* Sets the piece's colour to 'player'. */
    void set_neighbours(Piece board[8][8], int row, int col, Piece null);
    char get_colour() const;
    Piece get_neighbour(int dir) const;
    void get_neighbours(Piece *neighbours[8]) const; /* Assigns the piece's neighbour pointers to
						       the array of pointers '*neighbours[8]'. */  
private:
    char colour;   
    Piece *neighbour[8]; /* An array of pointers 
			    to each of the piece's eight surrounding neighbours, where
			    neighbour[0] is directly above, then they progress anticlockwise. */
};


class Move {
public:
    void set_score(float score_value);
    void set_player(char colour);
    void set_location(int row_no, int col_no);
    float get_score() const;
    char get_player() const;
    int get_row() const;
    int get_col() const;    
private:
    int row;
    int col;
    char player;
    float score; /* higher means better chance of winning */
};
    

void print_board(const Piece board[8][8]);

void player_move(Piece board[8][8], char &player);

bool check_legal_move(const Piece board[8][8], const Move move, int &dir, Piece &end_of_line);

/* Returns 'W' for 'B', and vice versa. */
char opponent(char player);

/* Detects whether an enemy piece is followed by an unbroken line of enemy pieces, followed by
   a friendly piece. If true, the function returns the address of the friendly piece, and 
   NULL otherwise.
   The enemy piece is 'piece', and 'dir' is its position relative to the attempted move. */
bool check_line(Piece piece, int dir, char player, Piece &end_of_line);

void swap_player(char &player);


int main()
{
    Piece board[8][8];
    char player = 'W';
    Piece null; /* The 'null' piece neighbours edge pieces which would otherwise have 
		   out-of-bounds neighbours. */
    null.place_piece(' ');

    /* Setting the board. */
    for (int rows=0; rows<8; rows++) {
	for(int cols=0; cols<8; cols++) {
	    board[rows][cols].place_piece(' ');
	    board[rows][cols].set_neighbours(board, rows, cols, null);
	}
    }
    board[3][3].place_piece('W');
    board[3][4].place_piece('B');
    board[4][3].place_piece('B');
    board[4][4].place_piece('W');

    print_board(board);
    while (1) {
	player_move(board, player);
	print_board(board);
    }
    
    return 0;
}


void Piece::place_piece(const char player) {
    colour = player; }

char Piece::get_colour() const {
    return colour; }

Piece Piece::get_neighbour(int dir) const {
    return *neighbour[dir]; }

void Piece::set_neighbours(Piece board[8][8], int row, int col, Piece null)
{
    if (row >= 1)
	neighbour[0] = &board[row-1][col];
    else neighbour[0] = &null;
    
    if (row>=1 && col>=1)
	neighbour[1] = &board[row-1][col-1];
    else neighbour[1] = &null;
    
    if (col >= 1)
	neighbour[2] = &board[row][col-1];
    else neighbour[2] = &null;
    
    if (row<=6 && col>=1)
	neighbour[3] = &board[row+1][col-1];
    else neighbour[3] = &null;
    	     
    if (row <= 6)
	neighbour[4] = &board[row+1][col];
    else neighbour[4] = &null;
    
    if (row<=6 && col<=6)
	neighbour[5] = &board[row+1][col+1];
    else neighbour[5] = &null;
    
    if (col <= 6)
	neighbour[6] = &board[row][col+1];
    else neighbour[6] = &null;
    
    if (row>=1 && col<=6)
	neighbour[7] = &board[row-1][col+1];
    else neighbour[7] = &null;
}

void Piece::get_neighbours(Piece *neighbours[8]) const {
    for (int i=0; i<8; i++)
	neighbours[i] = neighbour[i];
}


void Move::set_score(float score_value) {
    score = score_value; }

void Move::set_location(int row_no, int col_no) {
    row = row_no; col = col_no; }

void Move::set_player(char colour) {
    player = colour; }

float Move::get_score() const {
    return score; }

int Move::get_row() const {
    return row; }

int Move::get_col() const {
    return col; }

char Move:: get_player() const {
    return player; }


void print_board(const Piece board[8][8])
{
    cout << "\n   ";
    for (int i=1; i<=8; i++)
	cout << "  " << i << " ";
    cout << endl;
    
    for (int rows=0; rows<8; rows++) {
	cout << "   ";
	for (int i=0; i<8; i++)
	    cout << "+===";
	char row = rows + 'A';
	cout << "+\n " << row << " |";

	for (int cols=0; cols<8; cols++)
	    cout << " " << board[rows][cols].get_colour() << " |";	
	cout << "\n";
    }
	
	cout << "   ";
	for (int i=0; i<8; i++)
	    cout << "+===";
	cout << "+\n\n";	    
}


void player_move(Piece board[8][8], char &player)
{
    char tile[2];
    Move move;
    int dir;
    Piece end_of_line;

    move.set_player(player);

    cout << player << "'s move. Your turn.\n";
    cout << "Enter coordinates to make a move: ";
    cin >> tile;
    move.set_location(tile[0]-'A', tile[1]-'1');
    
    while (!check_legal_move(board, move, dir, end_of_line)) {
	cout << "Illegal move. Please enter another location: ";
	cin >> tile;
	move.set_location(tile[0]-'A', tile[1]-'1');
    }

    board[ move.get_row() ][ move.get_col() ].place_piece(player);

    Piece start = board[ move.get_row() ][ move.get_col() ].get_neighbour(dir);
    while(end_of_line.get_neighbour((dir+4)%8).get_colour() != player) {
	; // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    }
    
    swap_player(player);
}
   


bool check_legal_move(const Piece board[8][8], const Move move, int &dir, Piece &end_of_line)
{
    /* Gets the coordinates, player, and neighbours of the given move and stores them locally. */
    int row = move.get_row();
    int col = move.get_col();
    char player = move.get_player();
    Piece *neighbour[8];
    board[row][col].get_neighbours(neighbour);

    if (row<0 || row>7 || col<0 || col>7)
	return 0;
    if (board[row][col].get_colour() != ' ')
	return 0;

    /* In each direction, checks if there is a neighbouring enemy piece, and if there is,
       checks if it is succeeded by an unbroken line of them, followed by a friendly piece. */
    for (dir=0; dir<8; dir++) {
	if ( (*neighbour[dir]).get_colour() == opponent(player) ) {
	    if (check_line(*neighbour[dir], dir, player, end_of_line))
		return 1;
	}
    }
    
    return 0;
}


char opponent(char player)
{
    if (player == 'W')
	return 'B';
    else if (player == 'B')
	return 'W';
}


bool check_line(Piece piece, int dir, char player, Piece &end_of_line)
{
    /* Stores the neighbours of the enemy piece, and the colour of the immediately following 
       piece in that same direction. */
    Piece *neighbour[8];
    piece.get_neighbours(neighbour);
    char colour = (*neighbour[dir]).get_colour();

    /* If another enemy piece is detected, this function is recursively called. */
    if (colour == opponent(player))
	return check_line(*neighbour[dir], dir, player, end_of_line);

    /* If a friendly piece is detected, the recursion terminates and its address is returned. */
    if (colour == player) {
	end_of_line = *neighbour[dir];
	return 1;
    }
    /* Otherwise, the function returns false and the move is not legal. */
    return 0;
}


void swap_player(char &player)
{
    if (player == 'W')
	player = 'B';
    else if (player == 'B')
	player = 'W';
}
