#pragma once
#include "Field.h"
#include "Bonus.h"

class Game {
	static const int WND_HEIGHT = 875;
	static const int WND_WIDTH = 820;
	static const int LEFT_INDENT = 10;
	static const int TOP_INDENT = 65; //score rectangle
	static const int TOP_TEXT_INDENT = 10;
	static const int RIGHT_INDENT = 10;
	static const int BOTTOM_INDENT = 10;
	static const int POINTS_FROM_ONE_GEM = 10;
	static const int BONUS_SPAWN_RADIUS = 3;

	sf::Texture textures;
	sf::Texture bonusTextures;

	unsigned long long score = 0;
	const std::string scoreText = "Score: ";
	sf::Text scoreLine;
	sf::Font font;

	Field* field = new Field();

	bool ClickIsValid(const sf::Vector2i& clickCoordinates)const;
	void ProcessCombinations(const sf::Vector2i& prevClickCoordinates, const sf::Vector2i& clickCoordinates);

	sf::Vector2i GenerateBonusCoordinates(sf::Vector2i& bonusGenerator)const;
	vector<sf::Vector2i> GetBonusSpawnAreaCoordinates(sf::Vector2i& curGemCoordinates)const;
public:
	Game();
	~Game();

	sf::RenderWindow window{ sf::VideoMode(WND_WIDTH, WND_HEIGHT), "Gems", sf::Style::Close };

	void Run(int& countOfClicks, sf::Vector2i& prevClickCoordinates);
	void DrawInWindow();
	void AddScore(const int numOfDeleted);

};