#include <iostream>
#include <numeric>

#include "Utils.h"


constexpr int MAX_RED = 12;
constexpr int MAX_GREEN = 13;
constexpr int MAX_BLUE = 14;

struct Cubes
{
	std::size_t count;
	std::string color;


	[[nodiscard]] bool isValid() const
	{
		if(color == "red")
		{
			return count  <= MAX_RED;
		}
		if(color == "green")
		{
			return count  <= MAX_GREEN;
		}
		return count  <= MAX_BLUE;
	}

};


std::vector<int> processData(const std::vector<std::string>& splitLines)
{
	std::vector<int> results;
	std::size_t lineNum = 0;
	for (const auto & line : splitLines)
	{
		lineNum++;
		bool isLineValid = true;
		if (auto temp = Utils::split_string(line, ':');
			temp.size() == 2)
		{
			temp = Utils::split_string(temp.at(1), ';');
			if (!temp.empty())
			{
				for (const auto & tempValues : temp)
				{
					if (!isLineValid)
					{
						break;
					}
					auto tempCubes = Utils::split_string(tempValues, ' ');
					const auto tempCubesSize = tempCubes.size();
					for (std::size_t i = 0; i < tempCubesSize - 1; i +=2)
					{
						if(!isLineValid)
						{
							break;
						}
						const auto num = Utils::strToInt(tempCubes[i]);
						auto color = tempCubes[i+1];
						std::erase(color, ',');

						if (Cubes cubes{static_cast<std::size_t>(num), color};
							!cubes.isValid())
						{
							isLineValid = false;
							break;
						}
					}
				}
			}
		}
		else
		{
			throw std::runtime_error("Parser failed on parsing \':\' in line " + std::to_string(lineNum));
		}

		if(isLineValid)
		{
			results.emplace_back(lineNum);
		}
	}
	return results;
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day02_input.txt");
	const auto splitLines = Utils::split_string(data);
	const auto results = processData(splitLines);
	const auto sum = std::accumulate(results.begin(), results.end(), 0);
	std::cout << "sum of all games IDs is " << sum << std::endl;
}
