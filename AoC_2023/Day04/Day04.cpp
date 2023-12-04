#include "Utils.h"
#include <iostream>
#include <set>

std::set<int> parseNumbers(const std::string& text)
{
	std::set<int> results;
	std::string temp;
	for (const char ch : text)
	{
		if(isdigit(ch))
		{
			temp += ch;
		}
		else
		{
			if(!temp.empty())
			{
				results.insert(Utils::strToInt(temp));
				temp.clear();
			}
		}
	}
	if (!temp.empty())
	{
		results.insert(Utils::strToInt(temp));
		temp.clear();
	}
	return results;
}

unsigned long long processDataPart1(const std::vector<std::string>& splitLines)
{
	unsigned long long result = 0;
	for (const auto& line : splitLines)
	{
		if (auto temp = Utils::split_string(line, ':');
			temp.size() == 2)
		{
			temp = Utils::split_string(temp.at(1), '|');
			if (temp.size() == 2)
			{
				const auto winningNumbers = parseNumbers(temp[0]);
				const auto cardNumbers = parseNumbers(temp[1]);
				int points = 0;
				for (const int winningNumber : winningNumbers)
				{
					for (const int cardNumber : cardNumbers)
					{
						if (cardNumber == winningNumber)
						{
							points++;
						}
					}
				}
				const auto singleCardResults = points ? 1 << (points - 1) : 0;
				result += singleCardResults;
			}
			else
			{
				throw std::runtime_error("Parser failed on parsing \'|\'");
			}
		}
		else
		{
			throw std::runtime_error("Parser failed on parsing \':\'");
		}
	}
	return result;
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day04_input.txt");
	const auto splitLines = Utils::split_string(data);
	const auto result = processDataPart1(splitLines);
	std::cout << "Total number of points is " << result << std::endl;

}
