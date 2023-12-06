#include <iostream>
#include "Utils.h"


std::vector<int> parseInput(const std::string& line)
{
	std::vector<int> results;
	if (auto splitResults = Utils::split_string(line, ':');
		splitResults.size() == 2)
	{
		splitResults = Utils::split_string(splitResults.back(), ' ');
		for (const auto& input : splitResults)
		{
			if (!input.empty())
			{
				results.emplace_back(Utils::strToInt(input));
			}
		}
	}
	else
	{
		throw std::runtime_error("parse input goes wrong");
	}
	return results;
}

unsigned long long parseInputPart2(const std::string& line)
{
	if (auto splitResults = Utils::split_string(line, ':');
		splitResults.size() == 2)
	{
		std::string temp;
		splitResults = Utils::split_string(splitResults.back(), ' ');
		for (const auto& input : splitResults)
		{
			if (!input.empty())
			{
				temp += input;
			}
		}
		return Utils::strToULL(temp);
	}
	throw std::runtime_error("parse input goes wrong");
}

std::size_t holdButton(const std::size_t holdTime, const std::size_t raceTime)
{
	const auto distance = holdTime * (raceTime - holdTime);
	return distance;
}

void processData(const std::vector<int>& times, const std::vector<int>& distances)
{
	const auto timesSize = times.size();
	std::size_t result = 1;
	for (std::size_t i = 0; i < timesSize; ++i)
	{
		std::size_t counter = 0;
		const std::size_t raceTime = times[i];
		for (std::size_t j = 0; j < raceTime; ++j)
		{
			const auto distance = holdButton(j, raceTime);
			if(const std::size_t recordDistance = distances[i];
				distance > recordDistance)
			{
				counter++;
			}
		}
		result *= counter;
	}
	std::cout << "number of ways to beat a record is " << result << '\n';
}

void processDataPart2(const unsigned long long raceTime, const unsigned long long recordDistance)
{
	std::size_t counter = 0;
	for (std::size_t j = 0; j < raceTime; ++j)
	{
		if(const auto distance = holdButton(j, raceTime);
			distance > recordDistance)
		{
				counter++;
		}
	}
	
	std::cout << "number of ways to beat a record is " << counter << '\n';
}

int main()
{

	const auto data = Utils::read_file("day06_input.txt");

	if (const auto splitLines = Utils::split_string(data);
		splitLines.size() == 2)
	{
		// part 1
		const auto times = parseInput(splitLines.front());
		const auto distances = parseInput(splitLines.back());
		processData(times, distances);

		// part 2
		const auto time = parseInputPart2(splitLines.front());
		const auto distance = parseInputPart2(splitLines.back());
		processDataPart2(time, distance);
	}
	else
	{
		throw std::runtime_error("split lines goes wrong");
	}
}
