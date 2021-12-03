#include "Player.h"
#include <iostream>

//Constructor for Player class
Player::Player() {
	cumulativeHits = 0;
	cumulativeMisses = 0;
}

void Player::set_hit_number(int hits) {
	cumulativeHits += hits;
}

void Player::set_miss_number(int misses) {
	cumulativeMisses += misses;
}

float Player::computer_damage_taken() {
	return cumulativeHits;
}

float Player::missed_shots() {
	return cumulativeMisses;
}

float Player::output_accuracy() {
	return cumulativeHits / (cumulativeHits + cumulativeMisses);
}