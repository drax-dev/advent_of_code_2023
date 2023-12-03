#include <iostream>
#include <numeric>
#include <vector>
#include "Utils.h"

struct GridNode
{
	GridNode() : x(0), y(0), value('.'), isPartNumber(false), isGear(false)
	{

	}

	GridNode(const std::size_t inputX, const std::size_t inputY, const char inputValue = '.')
		: x(inputX), y(inputY), value(inputValue), isPartNumber(false), isGear(false)
	{

	}

	std::size_t x;
	std::size_t y;

	char value;
	bool isPartNumber;
	bool isGear;
};

struct Grid
{
	Grid(const std::size_t inputXSize, const std::size_t inputYSize) : gridXSize(inputXSize), gridYSize(inputYSize)
	{
		for (std::size_t y = 0; y < inputYSize; ++y)
		{
			gridNodes.emplace_back(inputXSize, nullptr);
		}
	}

	std::vector<std::vector<std::shared_ptr<GridNode>>> gridNodes;
	std::size_t gridXSize;
	std::size_t gridYSize;


	std::shared_ptr<GridNode> getLeftTopNeighbour(const std::size_t x, const std::size_t y)
	{
		if (x > 0 && y > 0)
		{
			return gridNodes[y - 1][x - 1];
		}
		return nullptr;
	}

	std::shared_ptr<GridNode> getLeftMiddleNeighbour(const std::size_t x, const std::size_t y)
	{
		if (x > 0)
		{
			return gridNodes[y][x - 1];
		}
		return nullptr;
	}

	std::shared_ptr<GridNode> getLeftBottomNeighbour(const std::size_t x, const std::size_t y)
	{
		if (x > 0 && y < gridYSize - 1)
		{
			return gridNodes[y + 1][x - 1];
		}
		return nullptr;
	}

	std::shared_ptr<GridNode> getMiddleTopNeighbour(const std::size_t x, const std::size_t y)
	{
		if (y > 0)
		{
			return gridNodes[y - 1][x];
		}
		return nullptr;
	}

	std::shared_ptr<GridNode> getMiddleBottomNeighbour(const std::size_t x, const std::size_t y)
	{
		if (y < gridYSize - 1)
		{
			return gridNodes[y + 1][x];
		}
		return nullptr;
	}

	std::shared_ptr<GridNode> getRightTopNeighbour(const std::size_t x, const std::size_t y)
	{
		if (x < gridXSize - 1 && y > 0)
		{
			return gridNodes[y - 1][x + 1];
		}
		return nullptr;
	}

	std::shared_ptr<GridNode> getRightMiddleNeighbour(const std::size_t x, const std::size_t y)
	{
		if (x < gridXSize - 1)
		{
			return gridNodes[y][x + 1];
		}
		return nullptr;
	}

	std::shared_ptr<GridNode> getRightBottomNeighbour(const std::size_t x, const std::size_t y)
	{
		if (x < gridXSize - 1 && y < gridYSize - 1)
		{
			return gridNodes[y + 1][x + 1];
		}
		return nullptr;
	}

	std::vector<std::shared_ptr<GridNode>> getAllNeighbours(const std::shared_ptr<GridNode>& gridNode)
	{
		std::vector<std::shared_ptr<GridNode>> neighbours;
		if (gridNode != nullptr)
		{
			const auto nodeX = gridNode->x;
			const auto nodeY = gridNode->y;

			neighbours.emplace_back(getLeftTopNeighbour(nodeX, nodeY));
			neighbours.emplace_back(getLeftMiddleNeighbour(nodeX, nodeY));
			neighbours.emplace_back(getLeftBottomNeighbour(nodeX, nodeY));

			neighbours.emplace_back(getMiddleTopNeighbour(nodeX, nodeY));
			neighbours.emplace_back(getMiddleBottomNeighbour(nodeX, nodeY));

			neighbours.emplace_back(getRightTopNeighbour(nodeX, nodeY));
			neighbours.emplace_back(getRightMiddleNeighbour(nodeX, nodeY));
			neighbours.emplace_back(getRightBottomNeighbour(nodeX, nodeY));
		}

		return neighbours;
	}


	bool isNodeAdjacentToSymbol(const std::shared_ptr<GridNode>& gridNode)
	{
		if (gridNode != nullptr)
		{
			for (const auto neighbours = getAllNeighbours(gridNode);
				const auto & neighbour : neighbours)
			{
				if (neighbour != nullptr)
				{
					const auto neighbourValue = neighbour->value;

					if (!isdigit(neighbourValue) && neighbourValue != '.')
					{
						return true;
					}
				}
			}
		}
		return false;
	}
};

Grid initGrid(const std::vector<std::string>& splitLines)
{
	const auto ySize = splitLines.size();
	std::size_t xSize = 0;
	if (ySize > 0)
	{
		xSize = splitLines.begin()->size();
	}
	else
	{
		throw std::runtime_error("Grid y size is zero!");
	}

	Grid grid{ xSize, ySize };

	for (std::size_t y = 0; y < ySize; ++y)
	{
		for (std::size_t x = 0; x < xSize; ++x)
		{
			grid.gridNodes[y][x] = std::make_shared<GridNode>(x, y, splitLines[y][x]);
		}
	}

	return grid;
}

void setPartNumberFlagForAllValidNodes(Grid& grid)
{
	for (auto& nodesRow : grid.gridNodes)
	{
		for (auto& node : nodesRow)
		{
			if (isdigit(node->value))
			{
				if (grid.isNodeAdjacentToSymbol(node))
				{
					node->isPartNumber = true;
				}
			}
		}
	}
}

std::vector<int> processDataPart1(const Grid& grid)
{
	std::vector<int> results;
	const auto ySize = grid.gridYSize;
	const auto xSize = grid.gridXSize;
	std::string temp;
	for (std::size_t y = 0; y < ySize; ++y)
	{
		temp.clear();
		bool isPartNumber = false;
		for (std::size_t x = 0; x < xSize; ++x)
		{
			const auto node = grid.gridNodes[y][x];

			if (const auto nodeValue = node->value;
				isdigit(nodeValue))
			{
				temp += nodeValue;
				if (node->isPartNumber)
				{
					isPartNumber = true;
				}
			}
			else if (!temp.empty())
			{
				if (isPartNumber)
				{
					const auto tempNum = Utils::strToInt(temp);
					results.emplace_back(tempNum);
				}
				temp.clear();
				isPartNumber = false;
			}
		}
		if (!temp.empty())
		{
			if (isPartNumber)
			{
				const auto tempNum = Utils::strToInt(temp);
				results.emplace_back(tempNum);
			}
		}
	}
	return results;
}

void setIsGridFlagForAllValidNodes(Grid& grid)
{
	const auto ySize = grid.gridYSize;
	const auto xSize = grid.gridXSize;
	for (std::size_t y = 0; y < ySize; ++y)
	{
		for (std::size_t x = 0; x < xSize; ++x)
		{
			if (const auto node = grid.gridNodes[y][x];
				node->value == '*')
			{
				const auto neighbours = grid.getAllNeighbours(node);

				std::size_t partNumberAmount = 0;
				for (std::size_t i = 0; i < 8; ++i)
				{
					const auto& neighbour = neighbours[i];
					if (i == 3 || i == 7)
					{
						if (isdigit(neighbour->value))
						{
							if (!isdigit(neighbours[i - 3]->value))
							{
								partNumberAmount++;
							}
						}
					}
					else if (i == 4 || i == 5)
					{
						if (isdigit(neighbour->value))
						{
							if (!isdigit(neighbours[i - 2]->value))
							{
								partNumberAmount++;
							}
						}
					}
					else
					{
						if (isdigit(neighbour->value))
						{
							partNumberAmount++;
						}
					}
				}
				if (partNumberAmount == 2)
				{
					node->isGear = true;
				}
			}
		}
	}
}

int getWholePartNumber(Grid& grid, const std::shared_ptr<GridNode>& gridNode)
{
	std::string temp{ gridNode->value };
	std::shared_ptr<GridNode> predecessor = gridNode;
	while (predecessor != nullptr)
	{
		predecessor = grid.getLeftMiddleNeighbour(predecessor->x, predecessor->y);
		if (predecessor != nullptr && isdigit(predecessor->value))
		{
			temp.insert( 0, std::string{predecessor->value});
		}
		else
		{
			break;
		}
	}

	std::shared_ptr<GridNode> successor = gridNode;
	while (successor != nullptr && isdigit(successor->value))
	{
		successor = grid.getRightMiddleNeighbour(successor->x, successor->y);
		if (successor != nullptr && isdigit(successor->value))
		{
			temp += successor->value;
		}
		else
		{
			break;
		}
	}
	return Utils::strToInt(temp);
}

std::vector<int> processDataPart2(Grid& grid)
{
	std::vector<int> results;
	const auto ySize = grid.gridYSize;
	const auto xSize = grid.gridXSize;
	for (std::size_t y = 0; y < ySize; ++y)
	{
		for (std::size_t x = 0; x < xSize; ++x)
		{
			if (const auto node = grid.gridNodes[y][x];
				node->isGear)
			{
				const auto neighbours = grid.getAllNeighbours(node);
				std::vector<int> gearRatios;
				for (std::size_t i = 0; i < 8; ++i)
				{
					const auto& neighbour = neighbours[i];
					if (i == 3 || i == 7)
					{
						if (isdigit(neighbour->value))
						{
							if (!isdigit(neighbours[i - 3]->value))
							{
								gearRatios.emplace_back(getWholePartNumber(grid, neighbour));
							}
						}
					}
					else if (i == 4 || i == 5)
					{
						if (isdigit(neighbour->value))
						{
							if (!isdigit(neighbours[i - 2]->value))
							{
								gearRatios.emplace_back(getWholePartNumber(grid, neighbour));
							}
						}
					}
					else
					{
						if (isdigit(neighbour->value))
						{
							gearRatios.emplace_back(getWholePartNumber(grid, neighbour));
						}
					}
				}
				if (gearRatios.size() == 2)
				{
					results.emplace_back(gearRatios.front() * gearRatios.back());
				}
				else
				{
					throw std::runtime_error("Wrong number of gear ratios for gear at pos: [" + std::to_string(x) + ", " + std::to_string(y) + ']');
				}
			}
		}
	}
	return results;
}


int main()
{
	// part 1
	const auto data = Utils::read_file("day03_input.txt");
	const auto splitLines = Utils::split_string(data);
	auto grid = initGrid(splitLines);
	setPartNumberFlagForAllValidNodes(grid);
	const auto results = processDataPart1(grid);
	const auto sum = std::accumulate(results.begin(), results.end(), 0);
	std::cout << "sum of all the part numbers is " << sum << std::endl;

	// part 2
	const auto dataPart2 = Utils::read_file("day03_input.txt");
	const auto splitLinesPart2 = Utils::split_string(dataPart2);
	auto gridPart2 = initGrid(splitLinesPart2);
	setIsGridFlagForAllValidNodes(gridPart2);
	const auto resultsPart2 = processDataPart2(gridPart2);
	const auto sumPart2 = std::accumulate(resultsPart2.begin(), resultsPart2.end(), 0);
	std::cout << "sum of all gear ratios is " << sumPart2 << std::endl;
}
