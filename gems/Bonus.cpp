#include "Bonus.h"
#include "Bonus.h"

Bomb::Bomb(int x, int y, const sf::Texture* gemsTextures, const sf::Texture* bonusTextures): Bonus(x, y, gemsTextures){
	SetBonusTexture(BonusType::BOMB, bonusTextures);
}

vector<sf::Vector2i> Bomb::GetBonusActionCoordinates(Field* field) {
	vector<sf::Vector2i> actionCoordinates;
	actionCoordinates.push_back(bonusCoordinates); //bonus gem is deleted too

	int fieldSize = field->GetFieldSize();
	int randX, randY;
	bool needToChange = false;
	for (int i = 0; i < numOfDeleted; ++i) {
		do {
			randX = rand() % fieldSize;
			randY = rand() % fieldSize;
			needToChange = false;
			for (int j = 0; j < i + 1; ++j) {
				if (randX == actionCoordinates[j].x && randY == actionCoordinates[j].y || field->fieldGrid[randY][randX]->comboMarker) {
					needToChange = true;
					break;
				}
			}
		} while (needToChange);
		sf::Vector2i curDeleted(randX, randY);
		actionCoordinates.push_back(curDeleted);
	}
	return actionCoordinates;
}

Repainter::Repainter(int x, int y, const sf::Texture* gemsTextures, const sf::Texture* bonusTextures, Type bonusGemType) : Bonus(x, y, gemsTextures) {
	SetTypeWithoutTexture(bonusGemType);
	SetBonusTexture(BonusType::REPAINTER, bonusTextures);
}

vector<sf::Vector2i> Repainter::GetBonusActionCoordinates(Field* field) {
	vector<sf::Vector2i> actionCoordinates;
	actionCoordinates.push_back(bonusCoordinates);
	vector<sf::Vector2i> actionAreaCoordinates;

	int fieldSize = field->GetFieldSize();

	for (int i = bonusCoordinates.y - radiusOfAction; i <= bonusCoordinates.y + radiusOfAction; ++i) { //y
		for (int j = bonusCoordinates.x - radiusOfAction; j <= bonusCoordinates.x + radiusOfAction; ++j) { //x
			if (i >= 0 && j >= 0 && i < fieldSize && j < fieldSize) {
				if (abs(bonusCoordinates.x - j) + abs(bonusCoordinates.y - i) <= 3 && !(field->fieldGrid[i][j]->comboMarker)\
					&& field->fieldGrid[i][j]->GetGemType() != this->GetGemType()) {
					sf::Vector2i curCoordinates(j, i);
					if (!field->IsNeighbourOfPrevGem(bonusCoordinates, curCoordinates))
						actionAreaCoordinates.push_back(curCoordinates);
				}
			}
		}
	}

	bool needToChange = false;
	int numberOfRepainted;
	for (int i = 0; i < numOfRepainted; ++i) {
		do {
			numberOfRepainted = rand() % actionAreaCoordinates.size();
			needToChange = false;
			for (int j = 0; j < i + 1; ++j) {
				if (actionCoordinates[j] == actionAreaCoordinates[numberOfRepainted]) {
					needToChange = true;
					break;
				}
			}
		} while (needToChange);
		actionCoordinates.push_back(actionAreaCoordinates[numberOfRepainted]);
	}

	return actionCoordinates;
}

void Bonus::SetBonusTexture(BonusType bonusType, const sf::Texture* bonusTextures) {
	setTexture(bonusTextures);
	setTextureRect(sf::IntRect(this->GetGemSize() * static_cast<int>(bonusType), 0, this->GetGemSize(), this->GetGemSize()));
}

void Bonus::DeleteBonusTexture(const sf::Texture* gemsTextures) {
	setTexture(gemsTextures);
}

void Bomb::BonusAction(Field* field, const sf::Texture* gemsTextures) {
	vector<sf::Vector2i> coordinatesOfDeletedGems = GetBonusActionCoordinates(field);

	for (int i = 0; i < coordinatesOfDeletedGems.size(); ++i) {
		field->fieldGrid[coordinatesOfDeletedGems[i].y][coordinatesOfDeletedGems[i].x]->SetDeletedGem();
	}
}

void Repainter::BonusAction(Field* field, const sf::Texture* gemsTextures) {
	vector<sf::Vector2i> coordinatesOfRepaintedGems = GetBonusActionCoordinates(field);

	for (int i = 0; i < coordinatesOfRepaintedGems.size(); ++i) {
		field->fieldGrid[coordinatesOfRepaintedGems[i].y][coordinatesOfRepaintedGems[i].x]->SetType(this->GetGemType());
	}
}