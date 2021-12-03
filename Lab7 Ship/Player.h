#ifndef PLAYER_H
#define PLAYER_H

typedef char gameboard[10][10];

class Player
{
	float cumulativeHits;
	float cumulativeMisses;

public:
	//Two 10 x 10 arrays are generated for both the player and the computer
	gameboard guessArray;
	gameboard shipArray;

	//Class constructor
	Player();

	/*Precondition:
		The game has been terminated
		The number of hits have been passed
	Postcondition:
		The total number of hits has been updated
	*/
	void set_hit_number(int hits);

	/*Precondition:
		The game has been terminated
		The number of misses have been passed
	Postcondition:
		The total number of misses has been updated
	*/
	void set_miss_number(int misses);

	/*Precondition:
		None nessesary
	Postcondition:
		The number of hits by the player is printed
	*/
	float computer_damage_taken();

	/*Precondition:
		None nessesary
	Postcondition:
		The number of missed shots is printed
	*/
	float missed_shots();

	/*Precondition:
		None nessesary
	Postcondition:
		The ratio of hits to misses is printed
	*/
	float output_accuracy();

};

#endif