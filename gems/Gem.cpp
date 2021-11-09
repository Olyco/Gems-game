#include "Gem.h"

Gem::Gem(int x, int y,const sf::Texture* textures) {
	setSize(sf::Vector2f(static_cast<float>(GEM_SIZE), static_cast<float>(GEM_SIZE)));
	setTexture(textures);

	gemType = static_cast<Type>(rand() % static_cast<int>(Type::NUM_OF_TYPES));
	setTextureRect(sf::IntRect(GEM_SIZE * static_cast<int>(gemType), 0, GEM_SIZE, GEM_SIZE));
	setPosition(static_cast<float>(x), static_cast<float>(y));
}

void Gem::UpdateGemType() {
	setFillColor(sf::Color(255, 255, 255, 255));
	gemType = static_cast<Type>(rand() % static_cast<int>(Type::NUM_OF_TYPES));
	setTextureRect(sf::IntRect(GEM_SIZE * static_cast<int>(gemType), 0, GEM_SIZE, GEM_SIZE));
}

void Gem::SetClickedGem() {
	setFillColor(sf::Color(200, 162, 200, 200));
}

void Gem::SetUnClickedGem() {
	setFillColor(sf::Color(255, 255, 255, 255));
	setTextureRect(sf::IntRect(GEM_SIZE * static_cast<int>(gemType), 0, GEM_SIZE, GEM_SIZE));
}

void Gem::SetType(Type newType) {
	gemType = newType;
	setFillColor(sf::Color(255, 255, 255, 255));
	setTextureRect(sf::IntRect(GEM_SIZE * static_cast<int>(newType), 0, GEM_SIZE, GEM_SIZE));
}

void Gem::SetDeletedGem() {
	gemType = Type::VOID;
	setFillColor(sf::Color(34, 23, 40, 0));
}

void Gem::SetTypeWithoutTexture(Type newType) {
	gemType = newType;
}

void Gem::BonusInsteadOfGem(Gem& bonus) {
	setTexture(bonus.getTexture());
	setTextureRect(bonus.getTextureRect());
}