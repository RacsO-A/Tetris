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
};

void tetris_board::tick() {
	coord current_pos = this->current_piece.get_pos();
	if (current_pos.x == 19) { // If piece is already at the bottom of the board
		std::cout << "entered bottom";
		this->board.at(current_pos.x).at(current_pos.y) = 
			color_char(this->current_piece.get_color(), ' '); // Updates the square under to be current piece
		this->current_piece.update_coord(coord(0, rand() % 10)); // Generates a new piece at the top randomly
		return;
	}

	std::string square_under_color =
		this->board.at(current_pos.x + 1).at(current_pos.y).get_color();

	if (square_under_color == "") { // If square under is empty, move the current down
		this->board.at(current_pos.x + 1).at(current_pos.y) = 
			color_char(this->current_piece.get_color(), ' '); // Update square under
		this->board.at(current_pos.x).at(current_pos.y) = 
			color_char("", ' ');// Update previous square to be empty
		this->current_piece.decrement_coord(); // Moves the current piece down
	} else {
		this->board.at(current_pos.x).at(current_pos.y) = 
			color_char(this->current_piece.get_color(), ' '); // Updates the square under to be current piece
		this->current_piece.update_coord(coord(0, rand() % 10)); // Generates a new piece at the top randomly
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

	tetris_board t_board(board, piece(coord(0,0), BAR_COLOR, 'n'));

	while (true) {
		t_board.tick();
		print_board(t_board.board);
		std::this_thread::sleep_for(0.02s);
		std::cout << " ";
	}



	/*
	for (int i = 0; i < 8; i++) {
		colors.at(0).print();
	}
	std::cout << " ";
	for (int i = 0; i < 4; i++) {
		colors.at(3).print();
	}
	std::cout << "     ";
	for (int i = 0; i < 4; i++) {
		colors.at(4).print();
	}
	std::cout << " ";
	for (int i = 0; i < 2; i++) {
		colors.at(2).print();
	}
	std::cout << "         ";
	for (int i = 0; i < 2; i++) {
		colors.at(1).print();
	}
	std::cout << "   ";
	for (int i = 0; i < 2; i++) {
		colors.at(5).print();
	}
	std::cout << "   ";
	for (int i = 0; i < 4; i++) {
		colors.at(6).print();
	}
	std::cout << "\n";

	std::cout << "           ";
	for (int i = 0; i < 4; i++) {
		colors.at(3).print();
	}
	std::cout << " ";
	for (int i = 0; i < 4; i++) {
		colors.at(4).print();
	}
	std::cout << "   ";
	for (int i = 0; i < 6; i++) {
		colors.at(2).print();
	}
	std::cout << " ";
	for (int i = 0; i < 6; i++) {
		colors.at(1).print();
	}
	std::cout << " ";
	for (int i = 0; i < 6; i++) {
		colors.at(5).print();
	}
	std::cout << " ";
	for (int i = 0; i < 4; i++) {
		colors.at(6).print();
	}
	std::cout << "\n";*/
}
