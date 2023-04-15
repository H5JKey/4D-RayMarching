#pragma once
#include <SFML/Graphics.hpp>


class Object4D {
public:
	float x, y, z, w;
	float xy, xz, xw, yz, yw, zw; // rotations
	float id;
	sf::Vector3f color;
	sf::Vector2f size;  

	Object4D() {
		this->id = 0;
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
		size.x = 2;
		size.y = 3;
		this->color = sf::Vector3f(0, 0, 0);
		xy = 0;
		xz = 0;
		xw = 0;
		yz = 0;
		yw = 0;
		zw = 0;
	}
};

