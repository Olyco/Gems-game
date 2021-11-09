#pragma once
#include "Gem.h"
#include "Field.h"

enum class BonusType {
	BOMB,
	REPAINTER,
};

class Bonus : public Gem {
private:
	virtual vector<sf::Vector2i> GetBonusActionCoordinates(Field* field) = 0;
public:
	sf::Vector2i bonusCoordinates;
	Bonus(int x, int y, const sf::Texture* gemsTextures) : Gem(x, y, gemsTextures) {};
	virtual void BonusAction(Field* field, const sf::Texture* gemsTextures) = 0;
	void SetBonusTexture(BonusType bonusType, const sf::Texture* bonusTextures);
	void DeleteBonusTexture(const sf::Texture* gemsTextures);
};

class Bomb : public Bonus {
	static const int numOfDeleted = 4;
	vector<sf::Vector2i> GetBonusActionCoordinates(Field* field)override;
public:
	void BonusAction(Field* field, const sf::Texture* gemsTextures)override;
	Bomb(int x, int y, const sf::Texture* gemsTextures, const sf::Texture* bonusTextures);
};

class Repainter : public Bonus {
	static const int radiusOfAction = 3;
	static const int numOfRepainted = 2;
	vector<sf::Vector2i> GetBonusActionCoordinates(Field* field)override;
public:
	void BonusAction(Field* field, const sf::Texture* gemsTextures)override;
	Repainter(int x, int y, const sf::Texture* gemsTextures, const sf::Texture* bonusTextures, Type bonusGemType);
};