#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using std::vector;

enum class Type {
	BLUE,
	RED,
	YELLOW,
	GREEN,
	ORANGE,
	PURPLE,
	NUM_OF_TYPES,
	VOID,
};

class Gem: public sf::RectangleShape {
	static const int GEM_SIZE = 100;
	Type gemType;
public:
	bool comboMarker = false;
	Gem(int x, int y, const sf::Texture* textures);
	void UpdateGemType();
	void SetClickedGem();
	void SetUnClickedGem();
	void SetType(Type newType);
	void SetDeletedGem();

	void SetTypeWithoutTexture(Type newType);
	Type GetGemType() { return gemType; }
	int GetGemSize() { return GEM_SIZE; }

	void BonusInsteadOfGem(Gem& bonus);
};