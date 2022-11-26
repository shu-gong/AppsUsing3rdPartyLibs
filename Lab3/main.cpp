/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 10/25/2022

Description:

Write a C++ application that uses SFML to create a simple pacman game.

*/

#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace sf;

vector<vector<int>> rectangleDataSet = {
	{60, 40, 84, 77},
	{80, 40, 185, 78},
	{20, 80, 309, 41},
	{80, 40, 370, 80},
	{60, 40, 493, 77},
	{60, 20, 84, 157},
	{20, 140, 184, 157},
	{60, 20, 210, 221},
	{140, 20, 251,161},
	{20, 136,432, 162},
	{20, 60,307, 182},
	{60, 20,372, 222},
	{60, 20,496, 161},
	{140, 80,247, 283},
	{20, 80,185, 346},
	{139, 20,251, 406},
	{20, 60,309, 432},
	{20, 80,432, 346},
	{60, 20,83, 469},
	{20, 60,123, 492},
	{80, 20,187, 469},
	{80, 20,373, 469},
	{60, 20,495, 469},
	{20, 60,493,491},
	{40, 20,43,528},
	{180, 20,84,591},
	{20, 60,186,531},
	{140, 20,250,529},
	{20, 60,308,552},
	{180, 20,372,593},
	{20, 60,432,532},
	{40, 20,556,532},
	{550, 10,43,27},
	{10, 180,32,40},
	{100, 10,43,221},
	{13, 60,130,225},
	{100, 10,43,293},
	{110, 10,33,344},
	{13, 60,130,350},
	{110, 10,34,418},
	{10, 100,32,430},
	{10, 111,32,552},
	{550, 10,44,651},
	{10, 221,594,429},
	{200, 82,494,345},
	{ 115,12 ,492, 220},
	{13, 60,492, 232},
	{115, 12,492, 292},
	{14, 195,593, 25},
};

vector<vector<string>> coinDataSet = {
	{"000000000000  000000000000"},
	{"0    0     0  0     0    0"},
	{"2    0     0  0     0    2"},
	{"0    0     0  0     0    0"},
	{"00000000000000000000000000"},
	{"0    0  0        0  0    0"},
	{"0    0  0        0  0    0"},
	{"000000  0000  0000  000000"},
	{"     0     1  1     0     "},
	{"     0     1  1     0     "},
	{"     0  1111111111  0     "},
	{"     0  1        1  0     "},
	{"     0  1        1  0     "},
	{"111110111        111011111"},
	{"     0  1        1  0     "},
	{"     0  1        1  0     "},
	{"     0  1111111111  0     "},
	{"     0  1        1  0     "},
	{"     0  1        1  0     "},
	{"000000000000  000000000000"},
	{"0    0     0  0     0    0"},
	{"0    0     0  0     0    0"},
	{"200  0000000@@0000000  002"},
	{"  0  0  0        0  0  0  "},
	{"  0  0  0        0  0  0  "},
	{"000000  0000  0000  000000"},
	{"0          0  0          0"},
	{"0          0  0          0"},
	{"00000000000000000000000000"},
};
//vector<vector<string>> coinDataSet = {
//	{"0002"}
//};

vector<vector<int>> tunnelDataSet = {
	{10,40,30,304},
	{10,40,600,304}
};


void setRectangle(RectangleShape& rectangle, vector<int> rectangleData);
void setCoins(vector<vector<string>>& coinDataSet, vector<CircleShape>& coinVector, vector<CircleShape>& powerupVector);
void moveGhost(Sprite& spriteGhost, int ghostDirectionNumber, float ghostSpeed);

int main()
{
	// Global Virable
	bool paused = true;
	bool acceptInput = false;
	float moveSpeed = 0.1;
	float ghostSpeed = 0.05;
	bool powerupStatus = false;

	// Welcome
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	Text messageText;
	messageText.setFont(font);
	messageText.setString("PRESS ENTER TO START");
	messageText.setCharacterSize(40);
	messageText.setFillColor(Color::White);
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);
	messageText.setPosition(641 / 2.0f, 728 / 2.0f);
	
	// Author
	Text authorText;
	authorText.setFont(font);
	authorText.setString("AUTHOR: SHU GONG @ GT");
	authorText.setCharacterSize(20);
	authorText.setFillColor(Color::White);
	FloatRect authorRect = authorText.getLocalBounds();
	authorText.setOrigin(authorRect.left +
		authorRect.width / 2.0f,
		authorRect.top +
		authorRect.height / 2.0f);
	authorText.setPosition(641 / 2.0f, 728 / 2.0f+40);

	// Background
	RenderWindow window(VideoMode(641, 728), "Pacman Go!");
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/maze.bmp");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Score
	int score = 0;
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setString(to_string(score));
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(Color::White);
	FloatRect scoreRect = scoreText.getLocalBounds();
	scoreText.setOrigin(scoreRect.left +
		scoreRect.width / 2.0f,
		scoreRect.top +
		scoreRect.height / 2.0f);
	scoreText.setPosition(190, 693);

	// Wall
	RectangleShape rectangle;
	vector<RectangleShape> rectangleVector;
	vector<FloatRect> rectangleBoundsVector;
	for (auto rectangleData : rectangleDataSet)
	{
		setRectangle(rectangle, rectangleData);
		rectangleVector.push_back(rectangle);
	}

	for (auto rectangle : rectangleVector)
	{
		FloatRect rectangleBounds;
		rectangleBounds = rectangle.getGlobalBounds();
		rectangleBoundsVector.push_back(rectangleBounds);
	}

	// Middle Tunnel
	RectangleShape tunnel;
	vector<RectangleShape> tunnelVector;
	for (auto tunnelData : tunnelDataSet)
	{
		setRectangle(tunnel, tunnelData);
		tunnelVector.push_back(tunnel);
	}

	// Coin
	vector<CircleShape> coinVector;
	vector<CircleShape> powerupVector;
	setCoins(coinDataSet, coinVector, powerupVector);
	int coinEatten = 0;
	int coinCount = coinVector.size();

	// Powerup
	chrono::time_point<chrono::system_clock> powerupTime, hitTime;
	hitTime = chrono::system_clock::now();
	powerupTime = chrono::system_clock::now();
	chrono::duration<double> elapsedSeconds = hitTime - powerupTime;

	// Ghost Vector
	Texture textureBlueGhost;
	Texture textureOrangeGhost;
	Texture texturePinkGhost;
	Texture textureRedGhost;

	Sprite spriteBlueGhost;
	Sprite spriteOrangeGhost;
	Sprite spritePinkGhost;
	Sprite spriteRedGhost;
	vector<Sprite> spriteGhostVector;

	// Blue Ghost
	textureBlueGhost.loadFromFile("graphics/blue_ghost.bmp");
	spriteBlueGhost.setTexture(textureBlueGhost);
	spriteGhostVector.push_back(spriteBlueGhost);

	// Orange Ghost
	textureOrangeGhost.loadFromFile("graphics/orange_ghost.bmp");
	spriteOrangeGhost.setTexture(textureOrangeGhost);
	spriteGhostVector.push_back(spriteOrangeGhost);

	// Pink Ghost
	texturePinkGhost.loadFromFile("graphics/pink_ghost.bmp");
	spritePinkGhost.setTexture(texturePinkGhost);
	spriteGhostVector.push_back(spritePinkGhost);

	// Red Ghost
	textureRedGhost.loadFromFile("graphics/red_ghosts.bmp");
	spriteRedGhost.setTexture(textureRedGhost);
	spriteGhostVector.push_back(spriteRedGhost);

	// Ghost Move Para
	srand(time(NULL));
	vector<int> ghostDirection;
	for (auto& spriteGhost : spriteGhostVector)
	{
		spriteGhost.setPosition(300, 250);
		ghostDirection.push_back(rand() % 4);
	}

	// Pacman
	Texture texturePacman;
	texturePacman.loadFromFile("graphics/pacman.bmp");
	Sprite spritePacman;
	spritePacman.setTexture(texturePacman);
	spritePacman.setPosition(300, 490);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == !paused)
			{
				// Listen for key presses again
				acceptInput = true;
			}

			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			setCoins(coinDataSet, coinVector, powerupVector);
			coinEatten = 0;

			paused = false;
			spritePacman.setPosition(300, 490);
			acceptInput = true;
			score = 0;

			for (auto& spriteGhost : spriteGhostVector)
			{
				spriteGhost.setPosition(300, 250);
				//spriteGhost.setPosition(555, 310);
			}
		}

		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				spritePacman.move(moveSpeed, 0);

				// Detect Collision
				for (auto rectangleBounds : rectangleBoundsVector)
				{
					if (rectangleBounds.intersects(spritePacman.getGlobalBounds()))
					{
						spritePacman.move(-moveSpeed, 0);
					}
				}
				for (auto& coin : coinVector)
				{
					if (coin.getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
					{
						coin.setPosition(1000, 0);
						coinEatten++;
						score += 10;
					}
				}
				for (auto& powerup : powerupVector)
				{
					if (powerup.getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
					{
						powerup.setPosition(1000, 0);
						powerupTime = chrono::system_clock::now();
						powerupStatus = true;
					}
				}

			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				spritePacman.move(-moveSpeed, 0);

				// Detect Collision
				for (auto rectangleBounds : rectangleBoundsVector)
				{
					if (rectangleBounds.intersects(spritePacman.getGlobalBounds()))
					{
						spritePacman.move(moveSpeed, 0);
					}
				}
				for (auto& coin : coinVector)
				{
					if (coin.getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
					{
						coin.setPosition(1000, 0);
						coinEatten++;
						score += 10;
					}
				}
				for (auto& powerup : powerupVector)
				{
					if (powerup.getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
					{
						powerup.setPosition(1000, 0);
						powerupTime = chrono::system_clock::now();
						powerupStatus = true;
					}
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				spritePacman.move(0, -moveSpeed);

				// Detect Collision
				for (auto rectangleBounds : rectangleBoundsVector)
				{
					if (rectangleBounds.intersects(spritePacman.getGlobalBounds()))
					{
						spritePacman.move(0, moveSpeed);
					}
				}
				for (auto& coin : coinVector)
				{
					if (coin.getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
					{
						coin.setPosition(1000, 0);
						coinEatten++;
						score += 10;

					}
				}
				for (auto& powerup : powerupVector)
				{
					if (powerup.getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
					{
						powerup.setPosition(1000, 0);
						powerupTime = chrono::system_clock::now();
						powerupStatus = true;
					}
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				spritePacman.move(0, moveSpeed);

				// Detect Collision
				for (auto rectangleBounds : rectangleBoundsVector)
				{
					if (rectangleBounds.intersects(spritePacman.getGlobalBounds()))
					{
						spritePacman.move(0, -moveSpeed);
					}
				}
				for (auto& coin : coinVector)
				{
					if (coin.getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
					{
						coin.setPosition(1000, 0);
						coinEatten++;
						score += 10;
					}
				}
				for (auto& powerup : powerupVector)
				{
					if (powerup.getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
					{
						powerup.setPosition(1000, 0);
						powerupTime = chrono::system_clock::now();
						powerupStatus = true;
					}
				}
			}
		}
		
		if (coinCount == coinEatten)
		{
			acceptInput = false;
			paused = true;
			messageText.setString("YOU WIN");
			messageText.setCharacterSize(40);
			messageText.setFillColor(Color::White);
			FloatRect textRect = messageText.getLocalBounds();
			messageText.setOrigin(textRect.left +
				textRect.width / 2.0f,
				textRect.top +
				textRect.height / 2.0f);
			messageText.setPosition(641 / 2.0f, 728 / 2.0f);
			coinEatten = 0;
		}

		if (!paused)
		{
			for (auto rectangleBounds : rectangleBoundsVector)
			{
				while (rectangleBounds.intersects(spritePacman.getGlobalBounds()))
				{
					spritePacman.move(0, moveSpeed);
				}
				for (auto& spriteGhost : spriteGhostVector)
				{
					int randomDirection = rand() % 4;
					while (rectangleBounds.intersects(spriteGhost.getGlobalBounds()))
					{
						moveGhost(spriteGhost, 0, 0.01);
					}
				}
			}

			// Ghost Move
			int ghostID = 0;
			for (auto& spriteGhost : spriteGhostVector)
			{
				moveGhost(spriteGhost, ghostDirection[ghostID], ghostSpeed);
				for (auto rectangleBounds : rectangleBoundsVector)
				{
					if (rectangleBounds.intersects(spriteGhost.getGlobalBounds()))
					{
						moveGhost(spriteGhost, ghostDirection[ghostID], -ghostSpeed);
						ghostDirection[ghostID] = rand() % 4;
					}
				}
				ghostID++;
			}
		}

		// Middle Tunnel
		for (auto& spriteGhost : spriteGhostVector)
		{
			if (tunnelVector[0].getGlobalBounds().intersects(spriteGhost.getGlobalBounds()))
			{
				spriteGhost.setPosition(555, 310);
			}
		}
		if (tunnelVector[0].getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
		{
			spritePacman.setPosition(555, 310);
		}

		for (auto& spriteGhost : spriteGhostVector)
		{
			if (tunnelVector[1].getGlobalBounds().intersects(spriteGhost.getGlobalBounds()))
			{
				spriteGhost.setPosition(55, 310);
			}
		}
		if (tunnelVector[1].getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
		{
			spritePacman.setPosition(55, 310);
		}

		// Pacman Ghost Collision
		for (auto& spriteGhost : spriteGhostVector)
		{
			if(spriteGhost.getGlobalBounds().intersects(spritePacman.getGlobalBounds()))
			{
				hitTime = chrono::system_clock::now();
				elapsedSeconds = hitTime - powerupTime;
				
				if (powerupStatus == true && elapsedSeconds.count() > 0 && (int)elapsedSeconds.count() < 5)
				{
					spriteGhost.move(10000, 0);
				}
				else
				{
					acceptInput = false;
					paused = true;
					messageText.setString("YOU LOSE");
					messageText.setCharacterSize(40);
					messageText.setFillColor(Color::White);
					FloatRect textRect = messageText.getLocalBounds();
					messageText.setOrigin(textRect.left +
						textRect.width / 2.0f,
						textRect.top +
						textRect.height / 2.0f);
					messageText.setPosition(641 / 2.0f, 728 / 2.0f);
					coinEatten = 0;
				}
			}
		}

		scoreText.setString(to_string(score));

		window.clear();
		window.draw(spriteBackground);
		for (auto& rectangle : rectangleVector)
		{
			window.draw(rectangle);
		}
		for (auto& tunnel : tunnelVector)
		{
			tunnel.setFillColor(Color::Transparent);
			window.draw(tunnel);
		}
		for (auto& coin : coinVector)
		{
			window.draw(coin);
		}
		for (auto& powerup : powerupVector)
		{
			window.draw(powerup);
		}
		for (auto& spriteGhost : spriteGhostVector)
		{
			window.draw(spriteGhost);
		}
		window.draw(spritePacman);
		if (paused)
		{
			window.draw(messageText);
			window.draw(authorText);
		}
		window.draw(scoreText);

		window.display();
	}

	return 0;
}

/******************************************************************/
// @brief: set all transparent rectangles
// @para: rectangle->RectangleShape&, rectangleData->vector<int>
/******************************************************************/
void setRectangle(RectangleShape& rectangle, vector<int> rectangleData)
{
	int length = rectangleData[0];
	int height = rectangleData[1];
	int x = rectangleData[2];
	int y = rectangleData[3];
	rectangle.setSize(Vector2f(length, height));
	rectangle.setPosition(x, y);
	rectangle.setFillColor(Color::Transparent);
}

/******************************************************************/
// @brief: set all coins and powerups
// @para: coinDataSet->a map used to create coins, coinVector->store
//		  coins, powerupVector-> store powerups
/******************************************************************/
void setCoins(vector<vector<string>>& coinDataSet, vector<CircleShape>& coinVector, vector<CircleShape>& powerupVector)
{
	coinVector = {};
	powerupVector = {};
	CircleShape coin;
	CircleShape powerup;
	int currentLine = 0;
	for (auto coinData : coinDataSet)
	{
		for (int i = 0; i < coinData[0].length(); i++)
		{
			if (coinData[0][i] == '0')
			{
				coin.setRadius(1.5f);
				coin.setFillColor(Color::White);
				coin.setPosition(62 + i * 20, 57 + currentLine * 20);
				coinVector.push_back(coin);
			}
			else if (coinData[0][i] == '2')
			{
				powerup.setRadius(6.f);
				powerup.setFillColor(Color::White);
				powerup.setPosition(62 + i * 20, 57 + currentLine * 20);
				powerupVector.push_back(powerup);
			}
		}
		currentLine += 1;
	}
}

/******************************************************************/
// @brief: determine the direction of ghosts
// @para: spriteGhost->the reference of Sprite, ghostDirectionNumber
//		  ->a random number to determine direction, ghostSpeed->Speed
/******************************************************************/
void moveGhost(Sprite& spriteGhost, int ghostDirectionNumber, float ghostSpeed)
{
	if (ghostDirectionNumber == 0)
	{
		spriteGhost.move(0, -ghostSpeed);
	}
	else if (ghostDirectionNumber == 1)
	{
		spriteGhost.move(0, ghostSpeed);
	}
	else if (ghostDirectionNumber == 2)
	{
		spriteGhost.move(-ghostSpeed, 0);
	}
	else
	{
		spriteGhost.move(ghostSpeed, 0);
	}
}