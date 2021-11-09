#pragma once
#include "Gem.h"
#include <cmath>

class Field {
	static const int FIELD_SIZE = 8; //num of gems in one row or column
	static const int GEM_SIZE = 100;

	bool gridIsFilled = false;

	bool IsInCombination(const sf::Vector2i& coordinates)const;
	bool CheckHorizontalCombination(const sf::Vector2i& coordinates)const;
	bool CheckVerticalCombination(const sf::Vector2i& coordinates)const;
	void SwapClickedGems(const sf::Vector2i& prevClickCoordinates, const sf::Vector2i& curClickCoordinates);
	bool MarkVerticalCombinations(const sf::Vector2i& gemCoordinates);
	bool MarkHorizontalCombinations(const sf::Vector2i& gemCoordinates);

	bool FindCombinationsWithGem(const sf::Vector2i& gemCoordinates);
	sf::Vector2i FindFirstVoidInColumn(const sf::Vector2i& beginOfColumn)const;
	int CountMaxNumOfVoidInColumn(vector<sf::Vector2i>& coordinatesOfFirstVoidGem)const;
	void DropColumn(sf::Vector2i& firstVoidCoordinates);

public:
	vector<vector<std::shared_ptr<Gem>>> fieldGrid{ FIELD_SIZE, vector<std::shared_ptr<Gem>>{FIELD_SIZE, nullptr} };

	void FillGrid(const int leftIndent, const int topIndent, sf::Texture* textures);
	void DisplayField(sf::RenderWindow& window) const;
	bool IsNeighbourOfPrevGem(const sf::Vector2i& prevClickCoordinates, const sf::Vector2i& clickCoordinates);
	bool ProcessSwappingGems(const sf::Vector2i& prevClickCoordinates, const sf::Vector2i& curClickCoordinates, sf::RenderWindow& window);
	int DeleteMarkedGems();
	void DropGems(sf::RenderWindow& window, sf::Text scoreLine);
	int DeleteAllCombinationsOnField();
	int GetGemSize()const;
	int GetFieldSize()const;
};