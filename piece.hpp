#include "coord.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "color.hpp"

const std::vector<std::vector<coord>> all_tetrinos 
	= {{{0, 4}, {0, 5}, {0, 6}, {0, 7}}, // Bar
	   {{1, 4}, {1, 5}, {1, 6}, {0, 6}}, // L
	   {{1, 4}, {1, 5}, {1, 6}, {0, 4}}, // Backwards L
	   {{0, 4}, {0, 5}, {1, 5}, {1, 6}}, // N
	   {{1, 4}, {1, 5}, {0, 5}, {0, 6}}, // Backwards N
	   {{1, 4}, {0, 5}, {1, 5}, {1, 6}}, // T
	   {{0, 4}, {1, 4}, {0, 5}, {1, 5}}  // Block
	};

const std::vector<std::string> tetrino_colors 
	= {BAR_COLOR, 
	   L_COLOR,
	   BL_COLOR, 
	   N_COLOR, 
	   BN_COLOR, 
	   T_COLOR, 
	   BLOCK_COLOR};

class piece {
	private:
		std::vector<coord> pos; // The components of the pieces starting
					// at (0,0)
		std::string color; // Color of the piece
		//coord rot; // The component that pieces will rotate around, this should change whenever the pos is changed
		
	public:
		piece() : pos({}), color("") {}
		piece(std::vector<coord> pos, std::string color/*coord rot*/) :
			pos(pos), color(color)/*rot(rot)*/ {}
		int get_lowest_pos();
		std::vector<coord> get_pos();
		std::string get_color();
		std::vector<coord> get_bottom_coords();

		void set_random_tetrino();

		void update_coord(std::vector<coord> c);
		void decrement_coord();
};

int piece::get_lowest_pos() {
	std::cout << "Hello\n";
	int lowest_x = this->pos.at(0).x;

	for (int i = 1; i < this->pos.size(); i++) {
		if (this->pos.at(i).x > lowest_x) {
			lowest_x = this->pos.at(i).x;
		}
	}
	return lowest_x;
}

std::vector<coord> piece::get_pos() {
	return this->pos;
}

std::string piece::get_color() {
	return this->color;
}

// Gets the coords that are at the bottom of a piece
std::vector<coord> piece::get_bottom_coords() { 
	std::vector<coord> bottom_coords;

	for (int i = 0; i < this->pos.size(); i++) {
		int add = true;
		for (int j = 0; j < bottom_coords.size(); j++) { // This loop checks if an entry of bottom_coords already holds 
			if (bottom_coords.at(j).y == this->pos.at(i).y) { // If on same y level, check which is above
				if (bottom_coords.at(j).x > this->pos.at(i).x) { // If there is already a block lower
					add = false;
					break;
				} else {
					bottom_coords.erase(bottom_coords.begin() + j);
					break;
				}
			}
		}
		if (add) {
			bottom_coords.push_back(this->pos.at(i));
		}
	}
	return bottom_coords;
}

// Sets the current piece to a random tetrino
void piece::set_random_tetrino() {
	int new_t_idx = rand() % 7;
	
	this->pos = all_tetrinos.at(new_t_idx);
	this->color = tetrino_colors.at(new_t_idx);
}

// Updates the coords of the piece to c
void piece::update_coord(std::vector<coord> c) {
	this->pos = c;
}

// Decrements all of the tetrinos pos to simulate it going down 1 tile on board
void piece::decrement_coord() {
	for (int i = 0; i < this->pos.size(); i++) {
		this->pos.at(i).x++;
	}
}



