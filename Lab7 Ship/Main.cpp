/*Samuel Trice
* C++ 2021 Fall
* Lab 7
* Make the game battleship. For placing the user’s ships, the user should be able 
to choose a xy coordinate and a direction to place the ship. Again, the program 
should check to make sure the ship placement is valid. The grid and coordinate system 
should be setup with x in the horizontal direction and y in the vertical direction. 
For each turn of the game, the computer should randomly choose locations to fire and 
the results should be shared and tracked on the screen. An example output on the 
computer’s turn would show the computer firing and giving the coordinates, display the 
result of “HIT!” and display an updated grid with ships and the locations of all of the 
computer’s hits and misses. When displaying the grid, you must have the grid coordinates 
displayed on the boarder for reference. You should give the user the option during each turn 
to fire, view user’s ship grid, or  surrender. When the fire function is called, display 
the user’s guess grid for reference. The program must ensure the computer and user only 
fire at valid locations that have not been checked. When one of the fleets is destroyed, 
the program should recognized it, communicate the results to the users, given an update 
on wins and losses, and give them the option to play again or quit. Make sure to use good 
programing practices for this lab. I expect that you use functions for each action the program 
takes, your code is well documented with pre/post conditions, and your code is reasonably efficient
*/

#include "Player.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <iomanip>

/*Player orders or questions are printed in RED
* Normal outputs are printed in WHITE
* The Water is printed in CYAN
* The boats are MAGENTA
* The hits are MAGENTA
* The misses are YELLOW
*/

using namespace std;

//Specified in Player.h and implemented in Player.cpp
Player user, computer;

enum compas { North, South, East, West };

enum ship_type { Destroyer, Submarine, Cruiser, Battleship, Carrier };

/*Precondition:
	The grid has already been declaired and initialized
Postcondition:
	The grid is outputed
*/
void output_board(char array[][10]);

/*Precondition:
	Computer's grid has already been declaired and initialized
Postcondition:
	A ship is added North or South on the computer's grid
*/
void add_north_south(int shipLength);

/*Precondition:
	Computer's grid has already been declaired and initialized
Postcondition:
	A ship is added East or West on the computer's grid
*/
void add_east_west(int shipLength);

/*Precondition:
	Computer's grid has already been declaired and initialized
Postcondition:
	Function has verified if there is ship overlap on the computers's ship array
*/
bool coputer_overlap_check(int ranRowsorColumns, int shipLength, compas shipDirection);

/*Precondition:
	Players's grid has already been declaired and initialized
	Function is passed player input for location
Postcondition:
	Function has verified if there is ship overlap on the players's ship array
*/
bool player_overlap_check(int row, int column, int shipLength, compas shipDirection);

/*Precondition:
	Player ship array has been declaired and initialized
	Function is passed player ship location data
Postcondition:
	Function has verified if there is ship out of bounds on the Player's ship array
*/
bool ship_outofbounds_check(int row, int column, int shipLength, compas shipDirection);

/*Precondition:
	Players's grid has already been declaired and initialized
	Function has checked overlap and out of bounds
Postcondition:
	The players ships have been placed 
*/
void place_player_ship(int row, int column, int shipLength, string ship_direction);

/*Precondition:
	Player entered valid coordinates and direction
Postcondition:
	A message tells the player that their ship is placed
*/
void board_update_messege();

/*Precondition:
	Player chose to not play again
Postcondition:
	A scoreboard with all the statistics is outputed to the screen
*/
void output_statistics(int numOfGames, int numOfLosses, int numOfWins);

/*Precondition: 
	Game must be set upand boards must be gennerated.
	Computer statistics must be initialized
Postcondition: 
	Prints the computers attempted hit or miss to the output screen.
	Adds the result to computer statistics. 
*/
void computer_turn(int& computerHits, int& computerMisses);

/*Precondition:
	Game must be set upand boards must be gennerated.
	Player statistics must be initialized
Postcondition:
	Player compleates their turn 
*/
void user_turn(int& userHits, int& userMisses);

/*Precondition:
	Computer turn is complete
	Player turn is complete
Postcondition:
	Checks win conditions to see if the game is over
*/
void check_win(int computerHits, int computerMisses, int userHits, int userMisses, bool hasSurrendered, bool& isEndOfGame, int& numOfLosses, int& numOfWins);

/*Precondition:
	Reference to computers grid is available
	Two counters initialized
Postcondition:
	Usser is given options once their turn is over
*/
void Menu(bool& hasSurrendered, int& userHits, int& userMisses);

/*Precondition:
	At least one game has finished
Postcondition:
	A new game is started or the program closes
*/
bool new_game();

/*Precondition:
	Player chose to surrender.
	Player confirmed to surrender
Postcondition:
	Player surrenders
*/
void Surrender(bool& hasSurrendered);

/*Precondition:
	Computers and Players grids can be referenced
	Fuction checks where can be targeted
Postcondition:
	Functions determins a hit or miss and updates the grid
*/
bool Fire(char shipGrid[][10], char guessGrid[][10], int xcoordinate, int ycoordinate);

/*Precondition:
	Computers grids can be updated
Postcondition:
	Ships are added randomly to the grid
*/
void computer_ship_grid();

/*Precondition:
	Program usses an empty array
Postcondition:
	The array is initialized in a 10x10 and populated with "water"(~)
*/
void board_default(char emptyArray[][10]);

/*Precondition:
	Player's grid can be updated
Postcondition:
	The player chooses where they want their ships to be placed
*/
void player_ship_adder();

/*Precondition:
	The player's ship and guess arrays have been successfully initialized
Postcondition:
	The players's input has been validated and ships have been placed on the user's ship grid.
*/
void check_input(int& shipLength);

/*Precondition:
	Player's input has been checked
	Player's grid has been declaired and initialized
Postcondition:
	Player's ship directions have been checked and verified
*/
bool validate_direction(int row, int column, string ship_direction, int shipLength);

int main()
{
	//Seeds clock for random number generation.
	srand(unsigned int(time(NULL)));

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//These identifiers track game progress amd control loops
	int numOfGames = 1;
	int numOfWins = 0;
	int numOfLosses = 0;
	int userHits;
	int userMisses;
	int computerHits;
	int computerMisses;
	bool isNewGame;
	bool isEndOfGame;
	bool hasSurrendered;


	//Begin game loop.
	do
	{
		system("cls");

		//Tracks how many hits have been made
		computerHits = 0;
		computerMisses = 0;
		userHits = 0;
		userMisses = 0;

		//Sets boards and ships
		board_default(computer.guessArray);
		board_default(computer.shipArray);
		computer_ship_grid();
		board_default(user.guessArray);
		board_default(user.shipArray);
		player_ship_adder();

		isEndOfGame = false;
		hasSurrendered = false;

		//Interactive portion of the game.
		do
		{
			computer_turn(computerHits, computerMisses);//Computer fires and updates guess grid.
			Menu(hasSurrendered, userHits, userMisses);//User fires and updates guess grid.

			//Game end conditions are checked to verify if it is the end of the game.
			check_win(computerHits, computerMisses, userHits, userMisses, hasSurrendered, isEndOfGame, numOfLosses, numOfWins);

		} while (!isEndOfGame);
	} while (new_game());

	//End of game loop.
	output_statistics(numOfGames, numOfLosses, numOfWins);

	SetConsoleTextAttribute(hConsole, 12);
	cout << "\n\nGoodbye...\n";
	SetConsoleTextAttribute(hConsole, 15);
	return 0;
}

void output_board(char array[][10])
{//Grid must be a parameter because both the user's and computer's board share this function
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 1; i < 11; ++i)
	{
		SetConsoleTextAttribute(hConsole, 15);//Sets Console to white
		//This if-else statement prints the x-axis
		if (i == 1) 
		{
			cout << "   " << i << " ";
		}
		else if (i > 1) 
		{
			cout << i << " ";
		}

		//This statement prints a end line for the beginning of the array.
		if (i == 10) 
		{
			cout << endl;
		}

	}

	for (int i = 0; i < 10; ++i)
	{
		//This if-else block prints the y-axis
		if (i < 9)
		{
			SetConsoleTextAttribute(hConsole, 15);
			cout << i + 1 << "  ";
		}
		else 
		{
			SetConsoleTextAttribute(hConsole, 15);
			cout << i + 1 << " ";
		}


		for (int j = 0; j < 10; ++j)
		{
			//Sets the water, hit or miss, and ship icons
			if (array[i][j] == '~') 
			{
				SetConsoleTextAttribute(hConsole, 11);//Sets water as Cyan
				cout << array[i][j] << ' ';
			}
			else if (array[i][j] == '#' || array[i][j] == 'H')
			{ 
				SetConsoleTextAttribute(hConsole, 13);//Sets Hit or Ship as Magenta
				cout << array[i][j] << ' ';
			}
			else if (array[i][j] == 'M') 
			{ 
				SetConsoleTextAttribute(hConsole, 14);//Sets Miss as yellow
				cout << array[i][j] << ' ';
			}


			if (j == 9)
				cout << endl;
		}
	}
	SetConsoleTextAttribute(hConsole, 15);
}

void computer_ship_grid()
{
	//ship_type enum is used to iterate through each ship size
	for (ship_type ship = Destroyer; ship <= Carrier; ship = ship_type(ship + 1))
	{
		//Generate a 1 or 2 randomly to determine to orient verically or horizontally.
		int random_dicrection = rand() % 2 + 1;
		int shipLength;

		switch (ship)
		{
		case Destroyer:

			shipLength = 2;

			if (random_dicrection == 1)
				add_north_south(shipLength);
			else
				add_east_west(shipLength);

			break;

		case Submarine:

			shipLength = 3;

			if (random_dicrection == 1)
				add_north_south(shipLength);
			else
				add_east_west(shipLength);

			break;

		case Cruiser:

			shipLength = 3;

			if (random_dicrection == 1)
				add_north_south(shipLength);
			else
				add_east_west(shipLength);

			break;

		case Battleship:

			shipLength = 4;

			if (random_dicrection == 1)
				add_north_south(shipLength);
			else
				add_east_west(shipLength);

			break;

		case Carrier:

			shipLength = 5;

			if (random_dicrection == 1)
				add_north_south(shipLength);
			else
				add_east_west(shipLength);

			break;
		}
	}


}

void add_north_south(int shipLength)
{
	//This bool determines if the function body should be repeated with a new random number because the first random number caused overlap
	bool isShipOverlap;
	do
	{
		//Generate a random column index
		int ranColumn = rand() % 10;

		isShipOverlap = coputer_overlap_check(ranColumn, shipLength, North);

		if (isShipOverlap == false) //If no ship is found in the validation read
		{
			for (shipLength; shipLength > 0; --shipLength)
			{
				computer.shipArray[shipLength][ranColumn] = '#';
			}
		}

	} while (isShipOverlap);
}

void add_east_west(int shipLength)
{
	//This bool determines if the function body should be repeated with a new random number because the first random number caused overlap
	bool isShipOverlap;
	do
	{
		//Generate a random row index
		int ranRow = rand() % 10;

		//coputer_overlap_check() will return a bool value that determines if there is ship overlap or not
		isShipOverlap = coputer_overlap_check(ranRow, shipLength, East);

		//If there wasn't ship overlap
		if (isShipOverlap == false)
		{
			//creates ship horizontally
			for (shipLength; shipLength > 0; --shipLength)
			{
				computer.shipArray[ranRow][shipLength] = '#';
			}
		}
	} while (isShipOverlap);

}

bool coputer_overlap_check(int ranRowsorColumns, int shipLength, compas shipDirection)
{
	if (shipDirection == North) //Verifying if a column for a ship
	{
		//Does initial read of area where a ship will be added to check if a ship is already there.
		for (shipLength; shipLength > 0; --shipLength)
		{
			//Creates a ship vertically 
			if (computer.shipArray[shipLength][ranRowsorColumns] == '#')
			{
				return true;
			}
		}

		return false;
	}
	else if (shipDirection == East) //verifying if a row for a ship
	{
		for (shipLength; shipLength > 0; --shipLength)
		{
			if (computer.shipArray[ranRowsorColumns][shipLength] == '#')
			{
				return true;
			}
		}

		return false;
	}
}

void check_input(int& shipLength)
{
	//Handle is used to communicate with the windows console which the game is running in
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//This bool is a loop control that is set to true if the user has overlapping ships or out-of-bounds ships
	bool isValidInput;

	//Identifiers for user input
	int row;
	int column;
	string ship_direction;

	do
	{
		isValidInput = false;
		SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
		cout << "\nEnter the starting x-coordinate for your ship: ";
		SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
		cin >> column;

		//Loops while user provides invalid input
		while (!cin || column < 0 || column > 10)
		{
			cin.clear();
			cin.ignore(1000, '\n');
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << endl << "Invalid input. Your input should be an integer between or equal to 1 and 10\a\n";
			cout << endl << "Try again: ";
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
			cin >> column;
		}
		SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
		cout << "\nEnter the starting y-coordinate for your ship: ";
		SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
		cin >> row;

		//Loops while user provides invalid input
		while (!cin || row < 0 || row > 10)
		{
			cin.clear();
			cin.ignore(1000, '\n');
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << endl << "Invalid input. Your input should be an integer between or equal to 1 and 10\a\n";
			cout << endl << "Try again: ";
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
			cin >> row;
		}
		SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
		cout << "\nEnter the direction you want your ship to have (\"north\", \"south\", \"east\", or \"west\"): ";
		SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
		cin >> ship_direction;

		//While the first letter of ship_direction string is NOT equal to U, D, R, or L
		while (toupper(ship_direction[0]) != 'N' && toupper(ship_direction[0]) != 'S' && toupper(ship_direction[0]) != 'E' && toupper(ship_direction[0]) != 'W')
		{
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << "\n\nInvalid input - input should be one of the following (\"north\", \"south\", \"east\", or \west\")\a\n";
			cout << "Try again: ";
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
			cin >> ship_direction;
		}

		//These are decremented by one from the values the user entered (1 - 10). This way they function as indexes for the gameboard array which has valid indexes of 0 - 9 for the coming function calls.
		row--;
		column--;

		//Checks for out of array bounds or ship overlap
		isValidInput = validate_direction(row, column, ship_direction, shipLength);

		//If input array indexes are not invalid
		if (isValidInput == true)
			place_player_ship(row, column, shipLength, ship_direction);

	} while (!isValidInput); //If overlap or out-of-array bounds then this function loops until the user enters valid data
}

bool validate_direction(int row, int column, string ship_direction, int shipLength)
{
	//Handle is used to communicate with the windows console which the game is running in
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//Switches off of the capitalized first letter of string ship_direction that the user provides
	switch (toupper(ship_direction[0]))
	{
	case 'N':

		//If the user's input is out of array bounds or overlaps with another ship on the board facing north
		if (ship_outofbounds_check(row, column, shipLength, North) == true || player_overlap_check(row, column, shipLength, North) == true)
		{
			//Print error message and board 
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << endl << "\nYour Ship is outside the game board or overlapping with another ship and must be moved.\a\n";
			cout << "Re-enter a new set of coordinates to try again.\n" << endl;
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
			output_board(user.shipArray);

			return false;
		}

		return true;

	case 'S':

		//If the user's input is out of array bounds or overlaps with another ship on the board facing south
		if (ship_outofbounds_check(row, column, shipLength, South) == true || player_overlap_check(row, column, shipLength, South) == true)
		{
			//Print error message and board 
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << endl << "\nYour Ship is outside the game board or overlapping with another ship and must be moved.\a\n";
			cout << "Re-enter a new set of coordinates to try again.\n" << endl;
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
			output_board(user.shipArray);

			return false;
		}

		return true;

	case 'East':

		//If the user's input is out of array bounds or overlaps with another ship on the board facing east
		if (ship_outofbounds_check(row, column, shipLength, East) == true || player_overlap_check(row, column, shipLength, East) == true)
		{
			//Print error message and board 
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << endl << "\nYour Ship is outside the game board or overlapping with another ship and must be moved\a\n";
			cout << "Re-enter a new set of coordinates to try again.\n" << endl;
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
			output_board(user.shipArray);

			return false;
		}

		return true;

	case 'West':

		//If The user's input is out of array bounds or overlaps with another ship on the board facing west
		if (ship_outofbounds_check(row, column, shipLength, West) == true || player_overlap_check(row, column, shipLength, West) == true)
		{
			//Print error message and board 
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << endl << "\nYour Ship is outside the game board or overlapping with another ship and must be moved\a\n";
			cout << "Re-enter a new set of coordinates to try again.\n" << endl;
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
			output_board(user.shipArray);

			return false;
		}

		return true;
	}
}

bool player_overlap_check(int row, int column, int shipLength, compas shipDirection)
{
	//Limit is used to control the following for loops
	int limit;
	switch (shipDirection)
	{

	case North:
		limit = row - shipLength;

		for (row; row > limit; --row)
		{
			if (user.shipArray[row][column] == '#')
				return true;
		}

		return false;

	case South:
		limit = row + shipLength;

		for (row; row <= limit; ++row)
		{
			if (user.shipArray[row][column] == '#')
				return true;
		}

		return false;

	case East:
		limit = column - shipLength;

		for (column; column > limit; column--)
		{
			if (user.shipArray[row][column] == '#')
				return true;
		}

		return false;

	case West:
		limit = column + shipLength;

		for (column; column < limit; column++)
		{
			if (user.shipArray[row][column] == '#')
				return true;
		}

		return false;
	}
}

bool ship_outofbounds_check(int row, int column, int shipLength, compas shipDirection)
{
	switch (shipDirection)
	{
	case North:
		if (row - shipLength < -1)
			return true;

		return false;

	case South:
		if (row + shipLength > 10)
			return true;

		return false;

	case East:
		if (column - shipLength < -1)
			return true;

		return false;

	case West:
		if (column + shipLength > 10)
			return true;

		return false;
	}
}

void place_player_ship(int row, int column, int shipLength, string ship_direction)
{
	int limit;

	switch (toupper(ship_direction[0]))
	{
	case 'N':
		limit = row - shipLength;

		for (row; row > limit; --row)
		{
			user.shipArray[row][column] = '#';
		}
		break;

	case 'S':
		limit = row + shipLength;

		for (row; row < limit; row++)
		{
			user.shipArray[row][column] = '#';
		}
		break;

	case 'E':
		limit = column - shipLength;

		for (column; column > limit; --column)
		{
			user.shipArray[row][column] = '#';
		}
		break;

	case 'W':
		limit = column + shipLength;

		for (column; column < limit; ++column)
		{
			user.shipArray[row][column] = '#';
		}
		break;
	}

}

void computer_turn(int& computerHits, int& computerMisses)
{
	//Random x,y coordinate is choses
	int com_x = rand() % 10 + 1;
	int com_y = rand() % 10 + 1;

	if (Fire(user.shipArray, computer.guessArray, com_x, com_y) == true) 
	{ 
		cout << "\nThe computer landed a hit!\n\n";
		computerHits++;
	}
	else
	{
		cout << "\nThe computer missed!\n";
		computerMisses++;
	}

}

void user_turn(int& userHits, int& userMisses)
{
	//Handle is used to communicate with the windows console which the game is running in
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int xcoordinate;//Identifiers for user's input coordinates
	int ycoordinate;
	SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
	cout << "\n\nEnter target x coordinate: ";
	SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
	cin >> xcoordinate;

	while (!cin || xcoordinate < 1 || xcoordinate > 10)//While user has entered invalid data or out-of-range values
	{
		cin.clear();
		cin.ignore(1000, '\n');
		SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
		cout << endl << "Invalid input. Your input should be an integer between or equal to 1 and 10\a\n";
		cout << endl << "Try again: ";
		SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
		cin >> xcoordinate;
	}
	SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
	cout << "\n\nEnter target y coordinate: ";
	SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
	cin >> ycoordinate;

	while (!cin || ycoordinate < 1 || ycoordinate > 10)//While user has entered invalid data or out-of-range values
	{
		cin.clear();
		cin.ignore(1000, '\n');
		SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
		cout << endl << "Invalid input. Your input should be an integer between or equal to 1 and 10\a\n";
		cout << endl << "Try again: ";
		SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
		cin >> ycoordinate;
	}

	if (Fire(computer.shipArray, user.guessArray, xcoordinate, ycoordinate) == true)
	{
		cout << "\n\nYou landed a ";
		SetConsoleTextAttribute(hConsole, 12); //Sets console color to red
		cout << "hit!\n";
		userHits++;
	}
	else 
	{
		cout << "\n\nYou";
		SetConsoleTextAttribute(hConsole, 10); //Sets console color to green
		cout << " missed!\n";
		userMisses++;
	}

	SetConsoleTextAttribute(hConsole, 15); //Sets console text back to white
}

void Menu(bool& hasSurrendered, int& userHits, int& userMisses)
{
	//hConsole is of type HANDLE, which allows access to manipulate the windows console for coloring and formatting output
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int selection;//Stores user input for following menu
	bool isEndOfTurn;//Bool controls whether or not the user has completed their turn

	SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
	cout << "Your guess grid: \n";
	output_board(user.guessArray);
	cout << "\nEnter your selection from the following menu: \n";
	SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white

	do
	{
		isEndOfTurn = false;

		cout << "1 - Fire\n";
		cout << "2 - View the computer's guess grid\n";
		cout << "3 - Review your ship grid\n";
		cout << "4 - Surrender\n";
		cout << "Enter your choice: ";
		cin >> selection;

		switch (selection)
		{
		case 1:
			system("cls");
			output_board(user.guessArray);
			user_turn(userHits, userMisses);
			isEndOfTurn = true;
			break;

		case 2:
			system("cls");
			cout << endl;
			cout << "Computer's guesses: \n";
			output_board(computer.guessArray);
			cout << endl;
			break;

		case 3:
			system("cls");
			cout << endl;
			cout << "Your ship grid: \n";
			output_board(user.shipArray);
			cout << endl;
			break;

		case 4:
			Surrender(hasSurrendered);
			isEndOfTurn = true;
			break;

		default:

			if (!cin) {
				cin.clear();
				cin.ignore(1000, '\n');
			}
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << "\n\nUnknown selection.\n";
			cout << "Try again.\n\n";
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
		}
	} while (!isEndOfTurn);
}

bool new_game()
{
	//hConsole is of type HANDLE, which allows access to manipulate the windows console for coloring and formatting output
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//String holds more than a char
	string input;
	SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
	cout << "\n\nStart a new game?\n";
	SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
	cout << "Y/N: ";

	cin >> input;

	//While the input stream is not in the failed state or the first letter of the input string is not a 'Y' or a 'N'.
	while (!cin || (toupper(input[0]) != 'N' && toupper(input[0]) != 'Y'))
	{
		cout << endl;
		SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
		cout << "\n\nYour input should either be a 'Y' or an 'N'\a\n";
		SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
		cout << endl;
		cout << "Try again: ";
		cin >> input;
	}

	//If user enters an N game loop will be exited because this function will return as a false condition for the loop control of the game
	if (input[0] == 'N')
		return false;
	else
		return true;
}

void output_statistics(int numOfGames, int numOfLosses, int numOfWins)
{
	system("cls");
	cout << "Here are your final stats: \n";
	cout << "\nTotal games played: " << numOfGames << endl;
	cout << "Wins: " << numOfWins << endl;
	cout << "Losses: " << numOfLosses << endl;
	cout << endl;
	cout << endl;
	cout << "Computer hits: " << computer.computer_damage_taken() << endl;
	cout << "Computer misses: " << computer.missed_shots() << endl;
	cout << setprecision(2) << "Accuracy: " << computer.output_accuracy() << '%' << endl;
	cout << endl;
	cout << "Your stats: \n";
	cout << "Player hits: " << user.computer_damage_taken() << endl;
	cout << "Player misses: " << user.missed_shots() << endl;
	cout << setprecision(2) << "Accuracy: " << user.output_accuracy() << '%' << endl;
}

void Surrender(bool& hasSurrendered)
{
	//hConsole is of type HANDLE, which allows access to manipulate the windows console for coloring and formatting output
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
	string input;
	cout << "\n\nAre you sure you want to surrender?\n";
	cout << "Y/N: ";
	SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white
	cin >> input;
	if (toupper(input[0]) == 'Y')
		hasSurrendered = true;

}

void check_win(int computerHits, int computerMisses, int userHits, int userMisses, bool hasSurrendered, bool& isEndOfGame, int& numOfLosses, int& numOfWins)
{
	//If the computer sinks all of the user's ships or the user surrenders mid-game
	if (computerHits == 17 || hasSurrendered == true) {

		isEndOfGame = true;
		system("cls");
		cout << "\n\nYou lose!\n";
		cout << "Computer's ship board: \n";
		numOfLosses++;

		//Update values in player objects
		computer.set_hit_number(computerHits);
		computer.set_miss_number(computerMisses);
		user.set_hit_number(userHits);
		user.set_miss_number(userMisses);
		output_board(computer.shipArray);
	}
	else if (userHits == 17) { //If user sinks all the computer's ships

		isEndOfGame = true;
		system("cls");
		cout << "\n\nYou win!\n";
		cout << "Computer's ship board: \n";
		numOfWins++;

		//Update values in player objects
		computer.set_hit_number(computerHits);
		computer.set_miss_number(computerMisses);
		user.set_hit_number(userHits);
		user.set_miss_number(userMisses);


		output_board(computer.shipArray);
	}
}

bool Fire(char shipGrid[][10], char guessGrid[][10], int xcoordinate, int ycoordinate)
{//shipGrid[][10] and guessGrid[][10] are parameters here because both the player and computer share this function
	if (shipGrid[ycoordinate - 1][xcoordinate - 1] == '#')
	{
		guessGrid[ycoordinate - 1][xcoordinate - 1] = 'H';
		return true;
	}
	else
	{
		guessGrid[ycoordinate - 1][xcoordinate - 1] = 'M';
		return false;
	}
}

void player_ship_adder()
{
	//hConsole is of type HANDLE, which allows access to manipulate the windows console for coloring and formatting output
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


	SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
	cout << "The computer has finished its set-up\n";
	cout << "You need to do the same with the following board: \n";
	SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white

	output_board(user.shipArray);

	int shipLength;
	//Following for-loop and switch guide player through gameboard set up.
	for (ship_type ship = Destroyer; ship <= Carrier; ship = ship_type(ship + 1))
	{
		switch (ship)
		{
		case Destroyer:
			shipLength = 2;
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << "\n\nPlace your destroyer (2 places)";
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white

			check_input(shipLength);

			//Clears board on the console screen after the user has placed a ship.
			system("cls");

			board_update_messege();
			output_board(user.shipArray);
			break;

		case Submarine:
			shipLength = 3;
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << "\n\nPlace your submarine (3 places)";
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white

			check_input(shipLength);

			system("cls");

			board_update_messege();
			output_board(user.shipArray);
			break;

		case Cruiser:
			shipLength = 3;
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << "\n\nPlace your crusier (3 places)";
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white

			check_input(shipLength);

			system("cls");

			board_update_messege();
			output_board(user.shipArray);
			break;

		case Battleship:
			shipLength = 4;
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << "\n\nPlace your battleship (4 places)";
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white

			check_input(shipLength);

			system("cls");

			board_update_messege();
			output_board(user.shipArray);
			break;

		case Carrier:
			shipLength = 5;
			SetConsoleTextAttribute(hConsole, 12);//Sets console color to Red
			cout << "\n\nPlace your carrier (5 places)";
			SetConsoleTextAttribute(hConsole, 15);//Sets console color back to white

			check_input(shipLength);

			system("cls");

			break;
		}
	}


}

void board_update_messege()
{
	cout << endl << "This is now your board: \n" << endl;
}

void board_default(char emptyArray[][10])
{//Array must be a parameter of this function because it is shared by both the user and the computer's gameboards
	//Initializes this array to water '~'
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			emptyArray[i][j] = '~';
		}
	}
}

