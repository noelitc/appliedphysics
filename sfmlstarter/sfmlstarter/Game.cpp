#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
#include "VectorAlgebra2D.h"


#include <iostream>
#include <stdlib.h> 
#include <time.h> 
#include <list>
enum PlayerState { ready, drawingLine, Jump };
class Game
{
public:
	//create Window
	sf::RenderWindow window;
	sf::View view;
	float randomNum = 0;

	sf::CircleShape playerShape;
	sf::Vector2f moveForce;
	PlayerState playerState = ready;
	float gravityScale = 20;
	sf::Vector2f velocity = { 0,0 };
	float playerMaxSpeed = 400;
	sf::Vector2f gravity{ 0.0f, 9.8f * gravityScale };
	
	sf::Vector2f normalised ;
	

	Game()
	{
		window.create(sf::VideoMode(800, 600), "Physics fights back.");
	}
	
	void init()
	{

		view = window.getDefaultView();
		playerShape.setRadius(20);
		playerShape.setPosition(160, 500);
		playerShape.setOrigin(sf::Vector2f(20, 20));
		sf::Vector2f normalised = thor::unitVector(playerShape.getPosition());
		float l = thor::length(gravity);
		float r = thor::toDegree(10.0);
		
		thor::setLength(normalised, 100.0f);
		thor::rotate(normalised, 10.0f);

	}
	void run()
	{
	
		sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);


		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		
		sf::Clock clock;

		clock.restart();

		while (window.isOpen())
		{
			
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			
			timeSinceLastUpdate += clock.restart();

			

			if (timeSinceLastUpdate > timePerFrame)
			{

				if (playerState == ready)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						playerState = drawingLine;
					}
				}
				if (playerState == drawingLine)
				{
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						sf::Vector2f localPosition = (sf::Vector2f)sf::Mouse::getPosition(window);
						moveForce = localPosition - playerShape.getPosition() ;
					
						if (thor::length(moveForce + velocity) < playerMaxSpeed)
						{
							velocity += moveForce;
						}
						else {
							thor::setLength(moveForce, playerMaxSpeed);
							velocity += moveForce;
						}
						if (thor::length(velocity) < playerMaxSpeed)
						{
							thor::setLength(velocity, playerMaxSpeed);
						}
						playerState = Jump;
						gravity.y = 9.8 * gravityScale;
					}
				}
				if (playerState == Jump)
				{

					
					playerState = ready;
				}



				
				
				velocity = velocity + (gravity * timeSinceLastUpdate.asSeconds());
				
				playerShape.move(velocity.x * timeSinceLastUpdate.asSeconds(), velocity.y * timeSinceLastUpdate.asSeconds());



				if (playerShape.getPosition().y > 600 - playerShape.getRadius())
				{
					gravity.y = 0;
					velocity = { 0,0 };
					
				}
				
				window.clear();
				if (playerState == drawingLine)
				{
					sf::Vector2i localPosition = sf::Mouse::getPosition(window);
					sf::Vertex line[] =
					{
						sf::Vertex(sf::Vector2f(localPosition.x, localPosition.y)),
						sf::Vertex(sf::Vector2f(playerShape.getPosition().x, playerShape.getPosition().y))
					};

					window.draw(line, 2, sf::Lines);
				}
				window.draw(playerShape);

				
				window.display();

				
				timeSinceLastUpdate = sf::Time::Zero;
			}
		}
	}
};


int main()
{


	Game game;
	

	game.init();

	game.run();



	return 0;
}