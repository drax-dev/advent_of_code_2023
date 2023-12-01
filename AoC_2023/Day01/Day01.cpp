#include <iostream>
#include <numeric>

#include "Utils.h"
#include <vector>
#include <string>

std::vector<int> processData(const std::vector<std::string>& splitLines)
{
	std::vector<int> results;
	int firstDigit = -1;
	int secondDigit = -1;
	for (const auto& line : splitLines)
	{
		const auto lineLength = line.length();
		for (std::size_t i = 0; i < lineLength; ++i)
		{
			if (const auto charFromLeft = line[i];
				firstDigit == -1 && isdigit(charFromLeft))
			{
				firstDigit = Utils::strToInt(charFromLeft);
			}
			if (const auto charFromRight = line[lineLength - 1 - i];
				secondDigit == -1 && isdigit(charFromRight))
			{
				secondDigit = Utils::strToInt(charFromRight);
			}

			if(firstDigit != -1 && secondDigit != -1)
			{
				results.emplace_back(firstDigit * 10 + secondDigit);
				firstDigit = -1;
				secondDigit = -1;
				break;
			}
		}
	}
	return results;
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day01_input.txt");
	const auto splitLines = Utils::split_string(data);
	const auto results = processData(splitLines);
	const auto sum = std::accumulate(results.begin(), results.end(), 0);
	std::cout << "result is: " << sum << std::endl;
}
