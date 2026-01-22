#include "color.hpp"
#include "piece.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <conio.h>

const piece empty_piece{{}, ""};

char get_user_input() {
	char c = '+';
	while(_kbhit()) {
		c = _getch();
	}
	return c;
}

class color_char {
	private:
		std::string color;
		char c;
	public:
		color_char(std::string color, char c) : 
			color(color), c(c) {}
		std::string get_color();

		void print();
		void append_to_str(std::string* str);

};

std::string color_char::get_color() {
	return this->color;
}

void color_char::print() {
	std::cout << this->color << this->c << "\033[0m";
}

void color_char::append_to_str(std::string* str) {
	*str += this->color;
	*str += this->c;
	*str += "\033[0m";
}

void print_board(std::vector<std::vector<color_char>> board) {
	std::string board_str;
	board_str += "+--------------------+\n";
	for (int i = 0; i < board.size(); i++) {
		board_str += "|";
		for (int j = 0; j < board.at(0).size(); j++) {
			board.at(i).at(j).append_to_str(&board_str);
			board.at(i).at(j).append_to_str(&board_str);
		}
		board_str += "|\n";
	}
	board_str += "+--------------------+\n";
	std::cout << "\033[2J\033[H";
	std::cout << board_str;
}

class tetris_board {
	public:
		std::vector<std::vector<color_char>> board;
		piece current_piece;
	public:
		tetris_board(std::vector<std::vector<color_char>> board, piece current_piece) :
			board(board), current_piece(current_piece) {}
		int decend_tick(); // Does a decend tick to the board
		void move_piece();
		bool cp_can_move_left();
		bool cp_can_move_right();

		void update_board_piece(color_char color, int offset_x, int offset_y);
		bool cp_will_collide();
		void set_new_rand_cp();
};

// Updates the board to match the current piece with some up/down offset
// offset_y is how many to go left or right. +1 is right and -1 is left on the board
// offset_x is the same but with up down, up is -1, down is +1
void tetris_board::update_board_piece(color_char color, int offset_x, int offset_y) {
	std::vector<coord> current_pos = this->current_piece.get_pos();

	for (int i = 0; i < current_pos.size(); i++) {
		coord block = current_pos.at(i);

		board.at(block.x + offset_x).at(block.y + offset_y) = color;
	}
}

bool tetris_board::cp_will_collide() {
	std::vector<coord> cp_pos = this->current_piece.get_bottom_coords();

	for (int i = 0; i < cp_pos.size(); i++) {
		coord block = cp_pos.at(i);

		if (this->board.at(block.x + 1).at(block.y).get_color() != "") {
			return true;
		}
	}
	return false;
}

// return value is how many frames to freeze
int tetris_board::decend_tick() {
	int bottom_cp = this->current_piece.get_lowest_pos();
	std::vector<coord> current_pos = this->current_piece.get_pos();

	if (bottom_cp == 19) { // If piece is already at the bottom of the board
		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0, 0); // "Freezes" the piece in place

		return 5;
	}

	bool current_piece_will_collide = this->cp_will_collide();

	if (!current_piece_will_collide) { // If current piece will not colide with a piece on the grid, decrement its position
		this->update_board_piece(
			color_char("", ' '), 0, 0); // Update where the piece is to be empty

		this->current_piece.decrement_coord(); // Moves the current piece down
		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0, 0); // Renders the current piece

		return 0;
	} else {
		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0, 0); // "Freezes" the piece in place
		
		return 5;
	}
}

bool tetris_board::cp_can_move_left() {
	std::vector<coord> left_coords = this->current_piece.get_left_coords();
	
	if (this->current_piece.left_most_coord() == 0) { // If at left side of board
		return false;
	}

	for (int i = 0; i < left_coords.size(); i++) {
		coord cur = left_coords.at(i);

		if (this->board.at(cur.x).at(cur.y - 1).get_color() != "") {
			return false;
		}
	}

	return true;
}

bool tetris_board::cp_can_move_right() {
	std::vector<coord> right_coords = this->current_piece.get_right_coords();

	if (this->current_piece.right_most_coord() == this->board.at(0).size() - 1) { // If at right side of board
		return false;
	}


	for (int i = 0; i < right_coords.size(); i++) {
		coord cur = right_coords.at(i);

		if (this->board.at(cur.x).at(cur.y + 1).get_color() != "") {
			return false;
		}
	}
	
	return true;
}

void tetris_board::move_piece() {
	char input = get_user_input();

	if (input == 'a' && this->cp_can_move_left()) { 
		this->update_board_piece(
			color_char("", ' '), 0, 0); // Update where the piece is to be empty

		this->current_piece.move_left();

		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0, 0); // Renders the piece the piece in place
	} else if (input == 'd' && this->cp_can_move_right()) {
		this->update_board_piece(
			color_char("", ' '), 0, 0); // Update where the piece is to be empty

		this->current_piece.move_right();

		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0, 0); // Renders the piece the piece in place
	}

	print_board(this->board);
}

void tetris_board::set_new_rand_cp() {
	this->current_piece.set_random_tetrino(); // Generates a new random piece at the top
	
	this->update_board_piece(
		color_char(this->current_piece.get_color(), ' '), 0, 0); // Renders it at the top of the board
}

int main() {
	using namespace std::chrono_literals;
	std::srand(std::time(nullptr));
	
	std::vector<std::vector<color_char>> board;
	for (int i = 0; i < 20; i++) {
		std::vector<color_char> row;
		for (int j = 0; j < 10; j++) {
			row.push_back(color_char("", ' '));
		}
		board.push_back(row);
	}

	piece new_tetrino;
	new_tetrino.set_random_tetrino();
	tetris_board t_board(board, new_tetrino);

	int skip = 0;	
	while (true) {
		if (skip == 0) {
			t_board.move_piece();
			skip = t_board.decend_tick();
		} else {
			skip--;
			if (skip == 0) {
				t_board.set_new_rand_cp();
			}
		}

		print_board(t_board.board);
		std::this_thread::sleep_for(0.1s);
		std::cout << " ";
	}
}

