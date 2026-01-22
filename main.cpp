#include "color.hpp"
#include "piece.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

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
		void tick(); // Does a game tick to the board

		void update_board_piece(color_char color, int offset);
		bool cp_will_collide();
};

// Updates the board to match the current piece with some up/down offset
// offset is how many to go up or down. +1 is down 1 and -1 is up 1 on the board
void tetris_board::update_board_piece(color_char color, int offset) {
	std::vector<coord> current_pos = this->current_piece.get_pos();

	for (int i = 0; i < current_pos.size(); i++) {
		coord block = current_pos.at(i);

		board.at(block.x + offset).at(block.y) = color;
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

void tetris_board::tick() {
	int bottom_cp = this->current_piece.get_lowest_pos();
	std::vector<coord> current_pos = this->current_piece.get_pos();


	if (bottom_cp == 19) { // If piece is already at the bottom of the board
		std::cout << "entered bottom";
		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0); // "Freezes" the piece in place
		
		this->current_piece.set_random_tetrino(); // Generates a new random piece at the top
		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0); // Renders it at the top of the board
		
		return;
	}

	bool current_piece_will_collide = this->cp_will_collide();

	if (!current_piece_will_collide) { // If current piece will not colide with a piece on the grid, decrement its position
		std::cout << "No collide\n";
		this->update_board_piece(
			color_char("", ' '), 0); // Update where the piece is to be empty

		this->current_piece.decrement_coord(); // Moves the current piece down
		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0); // Renders the current piece
	} else {
		std::cout << "Yes collide\n";

		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0); // "Freezes" the piece in place
		
		this->current_piece.set_random_tetrino(); // Generates a new random piece at the top
		this->update_board_piece(
			color_char(this->current_piece.get_color(), ' '), 0); // Renders it at the top of the board
		
		return;
	}
}

int main() {
	using namespace std::chrono_literals;
	std::srand(std::time(nullptr));

	std::vector<color_char> colors;
	colors.push_back(color_char(BAR_COLOR, ' '));
	colors.push_back(color_char(L_COLOR, ' '));
	colors.push_back(color_char(BL_COLOR, ' '));
	colors.push_back(color_char(N_COLOR, ' '));
	colors.push_back(color_char(BN_COLOR, ' '));
	colors.push_back(color_char(T_COLOR, ' '));
	colors.push_back(color_char(BLOCK_COLOR, ' '));
	
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

	
	while (true) {
		t_board.tick();
		print_board(t_board.board);
		std::this_thread::sleep_for(0.1s);
		std::cout << " ";
	}
}

