#include "coord.hpp"
#include <vector>
#include <string>
#include "color.hpp"

class piece {
	private:
		coord pos; // The components of the pieces starting
					// at (0,0)
		std::string color; // Color of the piece
		//coord rot; // The component that pieces will rotate around
		char type;
		
	public:
		piece(coord pos, std::string color, /*coord rot,*/ char type) :
			pos(pos), color(color), /*rot(rot),*/ type(type) {}
		coord get_pos();
		std::string get_color();

		void update_coord(coord c);
		void decrement_coord();
		bool check_colision(coord that);
};

coord piece::get_pos() {
	return this->pos;
}

std::string piece::get_color() {
	return this->color;
}

void piece::update_coord(coord c) {
	this->pos = c;
}

void piece::decrement_coord() {
	this->pos.x++;
}

bool piece::check_colision(coord that) {
	return (that.x == this->pos.x) && (that.y == this->pos.y);
}
