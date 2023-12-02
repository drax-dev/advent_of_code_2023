#include <iostream>
#include <numeric>

#include "Utils.h"
#include <vector>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, int> mapTextToNumber{{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4},
	{"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"zero", 0} };


std::vector<int> processDataPartOne(const std::vector<std::string>& splitLines)
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

			if (firstDigit != -1 && secondDigit != -1)
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

std::vector<int> getAllDigitsFromText(const std::string& text)
{
	std::vector<int> results;
	const auto textLength = text.length();
	std::string temp;
	for (std::size_t i = 0; i < textLength; ++i)
	{
		temp += text[i];
		for (std::size_t j = i + 1; j < textLength; ++j)
		{
	
			temp += text[j];
			if (mapTextToNumber.contains(temp) || temp.length() >= 5)
			{
				break;
			}
		}

		if (mapTextToNumber.contains(temp))
		{
			results.emplace_back(mapTextToNumber[temp]);
		}
		temp.clear();
	}
	return results;
}

std::vector<int> processDataPartTwo(const std::vector<std::string>& splitLines)
{
	std::vector<int> results;
	std::vector<int> digits;
	std::string text;
	for (const auto& line : splitLines)
	{
		digits.clear();
		text.clear();
		const auto lineLength = line.length();
		for (std::size_t i = 0; i < lineLength; ++i)
		{
			if (const auto singleChar = line[i];
				isdigit(singleChar))
			{
				if (!text.empty())
				{
					if (mapTextToNumber.contains(text))
					{
						digits.emplace_back(mapTextToNumber[text]);
					}
					else
					{
						const auto digitsFromLetters = getAllDigitsFromText(text);
						digits.insert(digits.end(), digitsFromLetters.begin(), digitsFromLetters.end());
					}
				}
				digits.emplace_back(Utils::strToInt(singleChar));
				text.clear();
			}
			else
			{
				text += singleChar;
			}
		}
		if (!text.empty())
		{
			const auto digitsFromLetters = getAllDigitsFromText(text);
			digits.insert(digits.end(), digitsFromLetters.begin(), digitsFromLetters.end());
		}

		if (!digits.empty())
		{
			results.emplace_back(digits.front() * 10 + digits.back());
		}
		else
		{
			throw std::runtime_error("digits vector should never be empty");
		}
	}
	return results;
}

int main()
{

	// part 1
	const auto data = Utils::read_file("day01_input.txt");
	const auto splitLines = Utils::split_string(data);
	const auto results = processDataPartOne(splitLines);
	const auto sum = std::accumulate(results.begin(), results.end(), 0);
	std::cout << "result is: " << sum << std::endl;

	// part 2
	const auto dataPartTwo = Utils::read_file("day01_input.txt");
	const auto splitLinesPartTwo = Utils::split_string(dataPartTwo);
	const auto resultsPartTwo = processDataPartTwo(splitLinesPartTwo);
	const auto sumPartTwo = std::accumulate(resultsPartTwo.begin(), resultsPartTwo.end(), 0);
	std::cout << "result is: " << sumPartTwo << std::endl;
}
