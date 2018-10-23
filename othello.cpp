#include <iostream>

using namespace std;

class Piece {
public:
    void place_piece(char player);
    char get_piece_colour() const;
    void set_neighbours(Piece board[8][8], int row, int col);
private:
    char colour;
    Piece *neighbour[8];
};

void Piece::place_piece(const char player)
{
    colour = player;
}

char Piece::get_piece_colour() const
{
    return colour;
}

void Piece::set_neighbours(Piece board[8][8], int row, int col)
{
    if (row >= 1)
	neighbour[0] = &board[row-1][col];
    if (row <= 6)
	neighbour[4] = &board[row+1][col];
    if (col >= 1)
	neighbour[2] = &board[row][col-1];
    if (col <= 6)
	neighbour[6] = &board[row][col+1];
    if (row>=1 && col>=1)
	neighbour[1] = &board[row-1][col-1];
    if (row>=1 && col<=6)
	neighbour[3] = &board[row-1][col+1];
    if (row<=6 && col<=6)
	neighbour[5] = &board[row+1][col+1];
    if (row<=6 && col>=1)
	neighbour[7] = &board[row+1][col-1];
}
    

void print_board(const Piece board[8][8]);
void player_move(char board[8][8], char &player);
bool check_legal_move(char board[8][8], char player, char tile[2]);


int main()
{
    Piece board[8][8];
    char player = 'W';

    for (int rows=0; rows<8; rows++) {
	for(int cols=0; cols<8; cols++) {
	    board[rows][cols].place_piece(' ');

	    board[rows][cols].set_neighbours(board, rows, cols);
	}
    }

    print_board(board);
    
    return 0;
}


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
	    cout << " " << board[rows][cols].get_piece_colour() << " |";	
	cout << "\n";
    }
	
	cout << "   ";
	for (int i=0; i<8; i++)
	    cout << "+===";
	cout << "+\n\n";	    
}

/*
void player_move(char board[8][8], char &player)
{
    char tile[2];

    cout << player << "'s move. Your turn.\n";
    cout << "Enter coordinates to make a move: ";
    cin >> tile;
    int row = tile[0] - 'A';
    int col = tile[1] - '1';

    while (!check_legal_move(board, player, tile)) {
	cout << "Illegal move. Please enter another location: ";
	cin >> tile;
	row = tile[0] - 'A';
	col = tile[1] - '1';
    }

     board[row][col].place_piece(player);
}


bool check_legal_move(const char board[8][8], char player, const char tile[2])
{
    
}
*/
