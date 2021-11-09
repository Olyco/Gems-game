#include "Field.h"

bool Field::CheckHorizontalCombination(const sf::Vector2i& coordinates)const {
	int combinationLength = 1;
	Type curGemType = fieldGrid[coordinates.y][coordinates.x]->GetGemType();
	int columnIndex = coordinates.x;
	int rowIndex = coordinates.y;

	while (columnIndex > 0 && fieldGrid[rowIndex][--columnIndex]->GetGemType() == curGemType)
		++combinationLength;

	if (gridIsFilled) {
		columnIndex = coordinates.x;
		while (columnIndex < (FIELD_SIZE - 1) && fieldGrid[rowIndex][++columnIndex]->GetGemType() == curGemType)
			++combinationLength;
	}

	return combinationLength > 2;
}

bool Field::CheckVerticalCombination(const sf::Vector2i& coordinates)const {
	int combinationLength = 1;
	Type curGemType = fieldGrid[coordinates.y][coordinates.x]->GetGemType();
	int columnIndex = coordinates.x;
	int rowIndex = coordinates.y;

	while (rowIndex > 0 && fieldGrid[--rowIndex][columnIndex]->GetGemType() == curGemType)
		++combinationLength;

	if (gridIsFilled) {
		rowIndex = coordinates.y;
		while (rowIndex < (FIELD_SIZE - 1) && fieldGrid[++rowIndex][columnIndex]->GetGemType() == curGemType)
			++combinationLength;
	}

	return combinationLength > 2;
}

bool Field::IsInCombination(const sf::Vector2i& coordinates)const {
	return CheckHorizontalCombination(coordinates) || CheckVerticalCombination(coordinates);
}

void Field::FillGrid(const int leftIndent, const int topIndent, sf::Texture* textures) {
	int x;
	int y = topIndent;
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < FIELD_SIZE; ++i) {
		x = leftIndent;
		for (int j = 0; j < FIELD_SIZE; ++j) {
			auto gem = std::make_shared<Gem>(Gem(x, y, textures));
			fieldGrid[i][j] = gem;
			x += GEM_SIZE;

			sf::Vector2i coordinates(j,i); //j is co-directed with x
			while (IsInCombination(coordinates)) {
				fieldGrid[i][j]->UpdateGemType();
			}
		}
		y += GEM_SIZE;
	}

	gridIsFilled = true;
}

void Field::DisplayField(sf::RenderWindow& window) const {
	for (int i = 0; i < FIELD_SIZE; ++i) {
		for (int j = 0; j < FIELD_SIZE; ++j) {
			window.draw(*fieldGrid[i][j]);
		}
	}
}

bool Field::IsNeighbourOfPrevGem(const sf::Vector2i& prevClickCoordinates, const sf::Vector2i& curClickCoordinates) {
	return curClickCoordinates.x == prevClickCoordinates.x - 1 && curClickCoordinates.y == prevClickCoordinates.y || curClickCoordinates.x == prevClickCoordinates.x + 1 && curClickCoordinates.y == prevClickCoordinates.y \
		|| curClickCoordinates.y == prevClickCoordinates.y - 1 && curClickCoordinates.x == prevClickCoordinates.x || curClickCoordinates.y == prevClickCoordinates.y + 1 && prevClickCoordinates.x == curClickCoordinates.x;
}

void Field::SwapClickedGems(const sf::Vector2i& prevClickCoordinates, const sf::Vector2i& curClickCoordinates) {
	Type prevType = fieldGrid[prevClickCoordinates.y][prevClickCoordinates.x]->GetGemType();
	fieldGrid[prevClickCoordinates.y][prevClickCoordinates.x]->SetType(fieldGrid[curClickCoordinates.y][curClickCoordinates.x]->GetGemType());
	fieldGrid[curClickCoordinates.y][curClickCoordinates.x]->SetType(prevType);
}

bool Field::MarkVerticalCombinations(const sf::Vector2i& gemCoordinates) {
	Type verifiable = fieldGrid[gemCoordinates.y][gemCoordinates.x]->GetGemType();
	int columnIndex = gemCoordinates.x;
	int rowIndex = gemCoordinates.y;
	int numOfMarked = 0;

	if (CheckVerticalCombination(gemCoordinates)) {
		while (rowIndex > 0 && fieldGrid[--rowIndex][columnIndex]->GetGemType() == verifiable) {
			fieldGrid[rowIndex][columnIndex]->comboMarker = true;
			++numOfMarked;
		}

		rowIndex = gemCoordinates.y;
		while (rowIndex < (FIELD_SIZE - 1) && fieldGrid[++rowIndex][columnIndex]->GetGemType() == verifiable) {
			fieldGrid[rowIndex][columnIndex]->comboMarker = true;
			++numOfMarked;
		}

		fieldGrid[gemCoordinates.y][gemCoordinates.x]->comboMarker = true;
		return true;
	}

	return false;
}

bool Field::MarkHorizontalCombinations(const sf::Vector2i& gemCoordinates) {
	Type verifiable = fieldGrid[gemCoordinates.y][gemCoordinates.x]->GetGemType();
	int columnIndex = gemCoordinates.x;
	int rowIndex = gemCoordinates.y;
	int numOfMarked = 0;

	if (CheckHorizontalCombination(gemCoordinates)) {
		while (columnIndex > 0 && fieldGrid[rowIndex][--columnIndex]->GetGemType() == verifiable) {
			fieldGrid[rowIndex][columnIndex]->comboMarker = true;
			++numOfMarked;
		}

		columnIndex = gemCoordinates.x;
		while (columnIndex < (FIELD_SIZE - 1) && fieldGrid[rowIndex][++columnIndex]->GetGemType() == verifiable) {
			fieldGrid[rowIndex][columnIndex]->comboMarker = true;
			++numOfMarked;
		}

		fieldGrid[gemCoordinates.y][gemCoordinates.x]->comboMarker = true;
		return true;
	}

	return false;
}

bool Field::FindCombinationsWithGem(const sf::Vector2i& gemCoordinates) {
	bool verticalMarked = MarkVerticalCombinations(gemCoordinates);
	bool horizontalmarked = MarkHorizontalCombinations(gemCoordinates);
	return verticalMarked || horizontalmarked;
}

sf::Vector2i Field::FindFirstVoidInColumn(const sf::Vector2i& beginOfColumn)const {
	int columnIndex = beginOfColumn.x;
	int rowIndex = beginOfColumn.y;

	while (rowIndex >= 0 && fieldGrid[rowIndex][columnIndex]->GetGemType() != Type::VOID)
		--rowIndex;

	if (rowIndex < 0) { //column ended
		sf::Vector2i noVoidGem(-1, -1);
		return noVoidGem;
	}

	sf::Vector2i firstVoid(columnIndex, rowIndex);
	return firstVoid;
}

int Field::CountMaxNumOfVoidInColumn(vector<sf::Vector2i>& coordinatesOfFirstVoidGem)const {
	int maxNumOfVoidInColumn = 0;

	for (int i = 0; i < FIELD_SIZE; ++i) {
		int numOfVoidInColumn = 0;
		for (int j = coordinatesOfFirstVoidGem[i].y; j >= 0; --j) {
			if (fieldGrid[j][i]->GetGemType() == Type::VOID)
				++numOfVoidInColumn;
		}
		if (numOfVoidInColumn > maxNumOfVoidInColumn)
			maxNumOfVoidInColumn = numOfVoidInColumn;
	}

	return maxNumOfVoidInColumn;
}

void Field::DropColumn(sf::Vector2i& firstVoidCoordinates) {
	int voidGemRow = firstVoidCoordinates.y;
	int voidGemColumn = firstVoidCoordinates.x;
	int i = voidGemRow;
	while (i > 0) {
		fieldGrid[i][voidGemColumn]->SetType(fieldGrid[i - 1][voidGemColumn]->GetGemType());
		--i;
	}

	fieldGrid[i][voidGemColumn]->UpdateGemType();

}

bool Field::ProcessSwappingGems(const sf::Vector2i& prevClickCoordinates, const sf::Vector2i& curClickCoordinates, sf::RenderWindow& window) {
	SwapClickedGems(prevClickCoordinates, curClickCoordinates);
	DisplayField(window);
	window.display();

	sf::sleep(sf::seconds(0.3f));

	bool prevGemInCombination = FindCombinationsWithGem(prevClickCoordinates);
	bool curGemInCombination = FindCombinationsWithGem(curClickCoordinates);
	if (!prevGemInCombination && !curGemInCombination) {
		SwapClickedGems(prevClickCoordinates, curClickCoordinates);
		return false;
	}

	return true;
}

int Field::DeleteMarkedGems() {
	int numOfDeleted = 0;
	for (int i = 0; i < FIELD_SIZE; ++i) {
		for (int j = 0; j < FIELD_SIZE; ++j) {
			if (fieldGrid[i][j]->comboMarker || fieldGrid[i][j]->GetGemType() == Type::VOID) {
				fieldGrid[i][j]->comboMarker = false;
				++numOfDeleted;
				fieldGrid[i][j]->SetDeletedGem();
			}
		}
	}
	return numOfDeleted;
}

void Field::DropGems(sf::RenderWindow& window, sf::Text scoreLine) {
	vector<sf::Vector2i> coordinatesOfFirstVoidGem;
	sf::Vector2i noVoidGem(-1, -1); //default value if there are no void gems in the column
	for (int i = 0; i < FIELD_SIZE; ++i)
		coordinatesOfFirstVoidGem.push_back(noVoidGem);

	for (int i = 0; i < FIELD_SIZE; ++i) {//start from the first gem in the last row
		sf::Vector2i curLowestGem(i, FIELD_SIZE - 1);
		coordinatesOfFirstVoidGem[i] = FindFirstVoidInColumn(curLowestGem);
	}

	int maxNumOfVoidInColumn = CountMaxNumOfVoidInColumn(coordinatesOfFirstVoidGem);

	for (int numOfFilledVoids = 0; numOfFilledVoids < maxNumOfVoidInColumn; ++numOfFilledVoids) {
		for (int i = 0; i < FIELD_SIZE; ++i) { //dropping the entire line
			if (coordinatesOfFirstVoidGem[i] != noVoidGem) {

				DropColumn(coordinatesOfFirstVoidGem[i]);
				sf::Vector2i curLowestGem(i, FIELD_SIZE - 1);
				coordinatesOfFirstVoidGem[i] = FindFirstVoidInColumn(curLowestGem); //dropping the entire column by one gem
			}
		}
		window.clear(sf::Color(34, 23, 40, 0));
		DisplayField(window);
		window.display();
		sf::sleep(sf::seconds(0.1f));
	}
}

int Field::DeleteAllCombinationsOnField() {
	int numOfDeleted = 0;
	for (int i = 0; i < FIELD_SIZE; ++i) {
		for (int j = 0; j < FIELD_SIZE; ++j) {
			sf::Vector2i curGemCoordinates(j, i);
			FindCombinationsWithGem(curGemCoordinates);
		}
	}
	return numOfDeleted = DeleteMarkedGems();
}

int Field::GetGemSize()const {
	return GEM_SIZE;
}

int Field::GetFieldSize()const {
	return FIELD_SIZE;
}
