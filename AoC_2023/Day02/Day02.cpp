#include <iostream>
#include <numeric>
#include <unordered_map>

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
		if (color == "red")
		{
			return count <= MAX_RED;
		}
		if (color == "green")
		{
			return count <= MAX_GREEN;
		}
		return count <= MAX_BLUE;
	}

};


std::vector<int> processDataPart1(const std::vector<std::string>& splitLines)
{
	std::vector<int> results;
	std::size_t lineNum = 0;
	for (const auto& line : splitLines)
	{
		lineNum++;
		bool isLineValid = true;
		if (auto temp = Utils::split_string(line, ':');
			temp.size() == 2)
		{
			temp = Utils::split_string(temp.at(1), ';');
			if (!temp.empty())
			{
				for (const auto& tempValues : temp)
				{
					if (!isLineValid)
					{
						break;
					}
					auto tempCubes = Utils::split_string(tempValues, ' ');
					const auto tempCubesSize = tempCubes.size();
					for (std::size_t i = 0; i < tempCubesSize - 1; i += 2)
					{
						if (!isLineValid)
						{
							break;
						}
						const auto num = Utils::strToInt(tempCubes[i]);
						auto color = tempCubes[i + 1];
						std::erase(color, ',');

						if (Cubes cubes{ static_cast<std::size_t>(num), color };
							!cubes.isValid())
						{
							isLineValid = false;
							break;
						}
					}
				}
			}
			else
			{
				throw std::runtime_error("Parser failed on parsing \';\' in line " + std::to_string(lineNum));
			}
		}
		else
		{
			throw std::runtime_error("Parser failed on parsing \':\' in line " + std::to_string(lineNum));
		}

		if (isLineValid)
		{
			results.emplace_back(lineNum);
		}
	}
	return results;
}


std::vector<unsigned long long> processDataPart2(const std::vector<std::string>& splitLines)
{
	std::vector<unsigned long long> results;
	std::size_t lineNum = 0;
	std::unordered_map<std::string, unsigned long long> colorToMaxAmountNeeded;
	for (const auto& line : splitLines)
	{
		lineNum++;
		colorToMaxAmountNeeded.clear();
		if (auto temp = Utils::split_string(line, ':');
			temp.size() == 2)
		{
			temp = Utils::split_string(temp.at(1), ';');
			if (!temp.empty())
			{
				for (const auto& tempValues : temp)
				{
					auto tempCubes = Utils::split_string(tempValues, ' ');
					const auto tempCubesSize = tempCubes.size();
					for (std::size_t i = 0; i < tempCubesSize - 1; i += 2)
					{
						const unsigned long long num = Utils::strToInt(tempCubes[i]);
						auto color = tempCubes[i + 1];
						std::erase(color, ',');
						if(colorToMaxAmountNeeded.contains(color))
						{
							colorToMaxAmountNeeded[color] = std::max(colorToMaxAmountNeeded[color], num);
						}
						else
						{
							colorToMaxAmountNeeded[color] = num;
						}
					}
				}
				const unsigned long long power = colorToMaxAmountNeeded["red"] * colorToMaxAmountNeeded["blue"] * colorToMaxAmountNeeded["green"];
				results.emplace_back(power);
			}
			else
			{
				throw std::runtime_error("Parser failed on parsing \';\' in line " + std::to_string(lineNum));
			}
		}
		else
		{
			throw std::runtime_error("Parser failed on parsing \':\' in line " + std::to_string(lineNum));
		}
	}
	return results;
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day02_input.txt");
	const auto splitLines = Utils::split_string(data);
	const auto results = processDataPart1(splitLines);
	const auto sum = std::accumulate(results.begin(), results.end(), 0);
	std::cout << "sum of all games IDs is " << sum << std::endl;

	//part2
	const auto dataPart2 = Utils::read_file("day02_input.txt");
	const auto splitLinesPart2 = Utils::split_string(dataPart2);
	const auto resultsPart2 = processDataPart2(splitLinesPart2);
	const unsigned long long sumPart2 = std::accumulate(resultsPart2.begin(), resultsPart2.end(), 0ull);
	std::cout << "sum of all powers of the sets is " << sumPart2 << std::endl;
}
