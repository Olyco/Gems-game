#include "Game.h"

Game::Game() {
	textures.loadFromFile("gems.png");
	bonusTextures.loadFromFile("bonus.png");
	field->FillGrid(LEFT_INDENT, TOP_INDENT, &textures);

	font.loadFromFile("dolcevita.ttf");
	scoreLine.setFont(font);
	scoreLine.setCharacterSize(45);
	scoreLine.setFillColor(sf::Color(153, 153, 255, 255));
	scoreLine.setString(scoreText + std::to_string(score));
	scoreLine.setPosition(sf::Vector2f(2 * LEFT_INDENT, TOP_TEXT_INDENT));
}

Game::~Game() {
	delete field;
}

bool Game::ClickIsValid(const sf::Vector2i& coordinates)const {
	return coordinates.x >= LEFT_INDENT && coordinates.x <= (WND_WIDTH - RIGHT_INDENT) \
		&& coordinates.y >= TOP_INDENT && coordinates.y <= (WND_HEIGHT - BOTTOM_INDENT);
}

void Game::AddScore(const int numOfDeleted) {
	score += POINTS_FROM_ONE_GEM * numOfDeleted;
}

vector<sf::Vector2i> Game::GetBonusSpawnAreaCoordinates(sf::Vector2i& curGemCoordinates)const {
	vector<sf::Vector2i> bonusAreaCoordinates;
	
	for (int i = curGemCoordinates.y - BONUS_SPAWN_RADIUS; i <= curGemCoordinates.y + BONUS_SPAWN_RADIUS; ++i) {
		for (int j = curGemCoordinates.x - BONUS_SPAWN_RADIUS; j <= curGemCoordinates.x + BONUS_SPAWN_RADIUS; ++j) {
			if (i >= 0 && j >= 0 && i < field->GetFieldSize() && j < field->GetFieldSize()) {
				if (abs(curGemCoordinates.x - j) + abs(curGemCoordinates.y - i) <= 3 && !(field->fieldGrid[i][j]->comboMarker)) {
					sf::Vector2i curCoordinates(j, i);
					bonusAreaCoordinates.push_back(curCoordinates);
				}
			}
		}
	}

	return bonusAreaCoordinates;
}

sf::Vector2i Game::GenerateBonusCoordinates(sf::Vector2i& bonusGenerator)const {
	vector<sf::Vector2i> availableBonusCoordinates; //vector of random coordinates from each deleted gem's area
	int fieldSize = field->GetFieldSize();
	vector<sf::Vector2i> bonusGenerators;

	for (int i = 0; i < fieldSize; ++i) {
		for(int j = 0; j < fieldSize; ++j){
			if (field->fieldGrid[i][j]->comboMarker) { //can generate bonus
				sf::Vector2i curCoodinates(j, i);
				bonusGenerators.push_back(curCoodinates);
				vector<sf::Vector2i> curAreaCoordinates = GetBonusSpawnAreaCoordinates(curCoodinates); //spawn area from current gem

				availableBonusCoordinates.push_back(curAreaCoordinates[rand() % curAreaCoordinates.size()]); //choose one random and push
			}
		}
	}

	int numOfBonusGenerator = rand() % availableBonusCoordinates.size();
	bonusGenerator = bonusGenerators[numOfBonusGenerator];
	return availableBonusCoordinates[numOfBonusGenerator];
}

void Game::ProcessCombinations(const sf::Vector2i& prevClickCoordinates, const sf::Vector2i& curClickCoordinates) {
	bool needToCorrectField = field->ProcessSwappingGems(prevClickCoordinates, curClickCoordinates, window);

	if (needToCorrectField) {
		int chanceOfBonus = rand() % 100;
		if (chanceOfBonus >= 50) {
			int bonusType = rand() % 2;
			sf::Vector2i bonusGenerator;
			sf::Vector2i bonusCoordinates = GenerateBonusCoordinates(bonusGenerator);
			if (bonusType == static_cast<int>(BonusType::BOMB)) {
				Bomb bomb(LEFT_INDENT + field->GetGemSize() * bonusCoordinates.x, TOP_INDENT + field->GetGemSize() * bonusCoordinates.y,\
					&textures, &bonusTextures);
				Bonus& bombB = bomb;
				bombB.bonusCoordinates = bonusCoordinates;
				Gem& bombG = bombB;
				auto bombGem = std::make_shared<Gem>(bombG);
				field->fieldGrid[bonusCoordinates.y][bonusCoordinates.x] = bombGem;

				DrawInWindow();
				sf::sleep(sf::seconds(0.4f));

				bomb.BonusAction(field, &textures);

				auto newGem = std::make_shared<Gem>(Gem(LEFT_INDENT + field->GetGemSize() * bonusCoordinates.x, TOP_INDENT + field->GetGemSize() * bonusCoordinates.y, &textures));
				newGem->SetDeletedGem();
				field->fieldGrid[bonusCoordinates.y][bonusCoordinates.x] = newGem;
			}
			else if(bonusType == static_cast<int>(BonusType::REPAINTER)){
				Repainter repainter(LEFT_INDENT + field->GetGemSize() * bonusCoordinates.x, TOP_INDENT + field->GetGemSize() * bonusCoordinates.y,\
					&textures, &bonusTextures, field->fieldGrid[bonusGenerator.y][bonusGenerator.x]->GetGemType());
				Bonus& repainterB = repainter;
				repainterB.bonusCoordinates = bonusCoordinates;
				Gem& repainterG = repainterB;
				auto repainterGem = std::make_shared<Gem>(repainterG);
				field->fieldGrid[bonusCoordinates.y][bonusCoordinates.x] = repainterGem;
				
				DrawInWindow();
				sf::sleep(sf::seconds(0.7f));

				repainter.BonusAction(field, &textures);

				auto newGem = std::make_shared<Gem>(Gem(LEFT_INDENT + field->GetGemSize() * bonusCoordinates.x, TOP_INDENT + field->GetGemSize() * bonusCoordinates.y, &textures));
				newGem->SetType(repainterGem->GetGemType());
				field->fieldGrid[bonusCoordinates.y][bonusCoordinates.x] = newGem;

				DrawInWindow();
				sf::sleep(sf::seconds(0.3f));
			}
		}
		int numOfDeleted = field->DeleteMarkedGems();

		DrawInWindow();
		sf::sleep(sf::seconds(0.3f));

		AddScore(numOfDeleted);

		field->DropGems(window, scoreLine);
		sf::sleep(sf::seconds(0.2f));
		DrawInWindow();
		sf::sleep(sf::seconds(0.3f));

		while ((numOfDeleted = field->DeleteAllCombinationsOnField()) != 0) { //checking new combinations after deleting
			DrawInWindow();
			sf::sleep(sf::seconds(0.3f));

			AddScore(numOfDeleted);

			field->DropGems(window, scoreLine);
			sf::sleep(sf::seconds(0.2f));
			DrawInWindow();
			sf::sleep(sf::seconds(0.3f));
		}
	}
}

void Game::Run(int& countOfClicks, sf::Vector2i& prevClickCoordinates) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();

		else if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
			sf::Vector2i clickCoordinates = sf::Mouse::getPosition(window);

			if (ClickIsValid(clickCoordinates)) {
				sf::Vector2i curClickCoordinates;
				curClickCoordinates.x = (clickCoordinates.x - LEFT_INDENT) / field->GetGemSize();
				curClickCoordinates.y = (clickCoordinates.y - TOP_INDENT) / field->GetGemSize();
				++countOfClicks;
				if (countOfClicks == 1) {
					field->fieldGrid[curClickCoordinates.y][curClickCoordinates.x]->SetClickedGem();
					prevClickCoordinates.x = curClickCoordinates.x;
					prevClickCoordinates.y = curClickCoordinates.y;
				}
				else if (countOfClicks == 2) {
					field->fieldGrid[prevClickCoordinates.y][prevClickCoordinates.x]->SetUnClickedGem();
					if (curClickCoordinates == prevClickCoordinates)
						countOfClicks = 0;
					else if (!field->IsNeighbourOfPrevGem(prevClickCoordinates, curClickCoordinates)) {
						field->fieldGrid[curClickCoordinates.y][curClickCoordinates.x]->SetClickedGem();
						prevClickCoordinates.x = curClickCoordinates.x;
						prevClickCoordinates.y = curClickCoordinates.y;
						countOfClicks = 1;
					}
					else {
						ProcessCombinations(prevClickCoordinates, curClickCoordinates);
						countOfClicks = 0;
						prevClickCoordinates.x = 0;
						prevClickCoordinates.y = 0;
					}
				}
			}
		}

	}
}

void Game::DrawInWindow() {
	window.clear(sf::Color(34, 23, 40, 0));
	field->DisplayField(window);

	scoreLine.setString(scoreText + std::to_string(score));
	window.draw(scoreLine);

	window.display();
}