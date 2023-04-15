#pragma once
#include <SFML/Graphics.hpp>
#include "Object4D.h"
#include <vector>
#include "User.h"


class Render
{
	sf::Shader shader;

public:

	Render() {
		shader.loadFromFile("shader.glsl", sf::Shader::Fragment);
	}

	void drawScene(Object4D object, sf::RenderWindow& window, User& user, sf::Time time) {

		shader.setUniform("u_resolution", sf::Vector2f(window.getSize()));
		shader.setUniform("u_mouse", window.mapPixelToCoords(sf::Mouse::getPosition()));
		shader.setUniform("ro", user.pos);
		shader.setUniform("u_time", time.asSeconds());
		shader.setUniform("lookAt", user.lookAt);
		shader.setUniform("posX", object.x);
		shader.setUniform("posY", object.y);
		shader.setUniform("posZ", object.z);
		shader.setUniform("posW", object.w);
		shader.setUniform("id", object.id);
		shader.setUniform("rotateXY", object.xy);
		shader.setUniform("rotateXZ", object.xz);
		shader.setUniform("rotateXW", object.xw);
		shader.setUniform("rotateYZ", object.yz);
		shader.setUniform("rotateYW", object.yw);
		shader.setUniform("rotateZW", object.zw);
		shader.setUniform("size", object.size);
		shader.setUniform("color", object.color);
		
		sf::RenderTexture tex;
		tex.create(window.getSize().x, window.getSize().y);
		sf::Sprite spr(tex.getTexture());
		spr.setScale(1, -1);
		spr.setPosition(0, window.getSize().y);

		tex.draw(spr, &shader);
		window.draw(spr);


	}

};

