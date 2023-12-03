#include <iostream>
#include <numeric>
#include <vector>
#include "Utils.h"

struct GridNode
{
	GridNode() : x(0), y(0), value('.'), isPartNumber(false)
	{

	}

	GridNode(const std::size_t inputX, const std::size_t inputY, const char inputValue = '.')
		: x(inputX), y(inputY), value(inputValue), isPartNumber(false)
	{

	}

	std::size_t x;
	std::size_t y;

	char value;
	bool isPartNumber;
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

void setPartNumberFlagForAllNodes(Grid& grid)
{
	for (auto& nodesRow : grid.gridNodes)
	{
		for (auto& node : nodesRow)
		{
			if (isdigit(node->value))
			{
				if(grid.isNodeAdjacentToSymbol(node))
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

int main()
{
	// part 1
	const auto data = Utils::read_file("day03_input.txt");
	const auto splitLines = Utils::split_string(data);
	auto grid = initGrid(splitLines);
	setPartNumberFlagForAllNodes(grid);
	const auto results = processDataPart1(grid);
	const auto sum = std::accumulate(results.begin(), results.end(), 0);
	std::cout << "sum of all the part numbers is " << sum << std::endl;
}
