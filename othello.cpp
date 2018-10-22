#include <iostream>

using namespace std;

void print_board(const char board[8][8]);
void player_move(char board[8][8], char &player);
bool check_legal_move(char board[8][8], char player, char tile[2]);


int main()
{
    char board[8][8];
    char player = 'W';

    for (int rows=0; rows<8; rows++) {
	for(int cols=0; cols<8; cols++)
	    board[rows][cols] = ' ';
    }

    print_board(board);
    
    return 0;
}


void print_board(const char board[8][8])
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
	    cout << " " << board[rows][cols] << " |";	
	cout << "\n";
    }
	
	cout << "   ";
	for (int i=0; i<8; i++)
	    cout << "+===";
	cout << "+\n\n";	    
}


void player_move(char board[8][8], char &player)
{
    char tile[2];

    cout << player << "'s move. Your turn.\n";
    cout << "Enter coordinates to make a move: ";
    cin >> tile;

    while (!check_legal_move(board, player, tile)) {
	cout << "Illegal move. Please enter another location: ";
	cin >> tile;
    }

    board[tile[0]-'A'][tile[1]-'1'] = player;
}


bool check_legal_move(const char board[8][8], char player, const char tile[2])
{
    
