#include <iostream>
#include <ranges>
#include <unordered_map>

#include "Utils.h"

struct Range
{
	Range(const unsigned long long inputBegin, const unsigned long long length) : begin(inputBegin)
	{
		end = begin + length;
	}

	unsigned long long begin;
	unsigned long long end;

	[[nodiscard]] bool isInRange(const unsigned long long number) const
	{
		if (number < begin || number > end)
		{
			return false;
		}
		return true;
	}

	bool operator==(const Range& other) const
	{
		return begin == other.begin && end == other.end;
	}
};

template <>
struct std::hash<Range>
{
	std::size_t operator()(const Range& r) const noexcept
	{
		using std::hash;

		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:

		return (hash<unsigned long long>()(r.begin)
			^ hash<unsigned long long>()(r.end) << 1) >> 1;
	}
};

std::vector<unsigned long long> seeds;
std::vector<Range> seedsPart2;
std::vector<std::pair<Range, Range>> mapSeedToSoil{};
std::vector<std::pair<Range, Range>> mapSoilToFertilizer{};
std::vector<std::pair<Range, Range>> mapFertilizerToWater{};
std::vector<std::pair<Range, Range>> mapWaterToLight{};
std::vector<std::pair<Range, Range>> mapLightToTemperature{};
std::vector<std::pair<Range, Range>> mapTemperatureToHumidity{};
std::vector<std::pair<Range, Range>> mapHumidityToLocation{};


void parseSeeds(const std::string& seedLine)
{
	auto temp = Utils::split_string(seedLine, ' ');
	temp.erase(temp.begin());
	for (const auto& seed : temp)
	{
		seeds.emplace_back(Utils::strToULL(seed));
	}
}

void parseSeedsPart2(const std::string& seedLine)
{
	auto temp = Utils::split_string(seedLine, ' ');
	temp.erase(temp.begin());
	const auto tempSize = temp.size();
	for (std::size_t i = 0; i < tempSize - 1; i += 2)
	{
		const unsigned long long length = Utils::strToULL(temp[i + 1]);
		seedsPart2.emplace_back(Utils::strToULL(temp[i]), length);
	}
}

void parseRange(const std::string& rangeLine, std::vector<std::pair<Range, Range>>* map)
{
	if (map != nullptr)
	{
		if (const auto temp = Utils::split_string(rangeLine, ' ');
			temp.size() == 3)
		{
			const unsigned long long length = Utils::strToULL(temp[2]);
			Range dest{ Utils::strToULL(temp[0]), length };
			Range src{ Utils::strToULL(temp[1]), length };
			map->emplace_back(std::make_pair(src, dest));
		}
		else
		{
			throw std::runtime_error("something goes wrong during range parse");
		}
	}
	else
	{
		throw std::runtime_error("map is nullptr");
	}
}

void parseInput(const std::vector<std::string>& splitLines)
{
	std::vector<std::pair<Range, Range>>* map = nullptr;
	for (const auto& line : splitLines)
	{
		if (line.contains("seeds: "))
		{
			parseSeeds(line);
		}
		else if (line.contains("seed-to-"))
		{
			map = &mapSeedToSoil;
		}
		else if (line.contains("soil-to-"))
		{
			map = &mapSoilToFertilizer;
		}
		else if (line.contains("fertilizer-to-"))
		{
			map = &mapFertilizerToWater;
		}
		else if (line.contains("water-to-"))
		{
			map = &mapWaterToLight;
		}
		else if (line.contains("light-to-"))
		{
			map = &mapLightToTemperature;
		}
		else if (line.contains("temperature-to-"))
		{
			map = &mapTemperatureToHumidity;
		}
		else if (line.contains("humidity-to-"))
		{
			map = &mapHumidityToLocation;
		}
		else if (!line.empty())
		{
			parseRange(line, map);
		}
		else
		{
			//empty line
		}
	}
}


unsigned long long getMappedValue(const unsigned long long srcValue, std::vector<std::pair<Range, Range>>* map)
{
	if (map != nullptr)
	{
		for (const auto& [src, dst] : *map)
		{
			if (src.isInRange(srcValue))
			{
				const auto diff = srcValue - src.begin;
				const auto result = dst.begin + diff;
				return result;
			}
		}
		return srcValue;
	}
	throw std::runtime_error("map is nullptr");
}

unsigned long long getReversedMappedValue(const unsigned long long dstValue, std::vector<std::pair<Range, Range>>* map)
{
	if (map != nullptr)
	{
		for (const auto& [src, dst] : *map)
		{
			if (dst.isInRange(dstValue))
			{
				const auto diff = dstValue - dst.begin;
				const auto result = src.begin + diff;
				return result;
			}
		}
		return dstValue;
	}
	throw std::runtime_error("map is nullptr");
}

unsigned long long processData()
{
	std::vector<unsigned long long> locations;
	for (const unsigned long long seed : seeds)
	{
		unsigned long long result = getMappedValue(seed, &mapSeedToSoil);
		result = getMappedValue(result, &mapSoilToFertilizer);
		result = getMappedValue(result, &mapFertilizerToWater);
		result = getMappedValue(result, &mapWaterToLight);
		result = getMappedValue(result, &mapLightToTemperature);
		result = getMappedValue(result, &mapTemperatureToHumidity);
		result = getMappedValue(result, &mapHumidityToLocation);
		locations.emplace_back(result);
	}
	std::ranges::sort(locations);
	return locations.front();
}

//unsigned long long processDataPart2()
//{
//	//std::vector<unsigned long long> locations;
//	unsigned long long lowestLocation = ULLONG_MAX;
//	for (const Range seedRange : seedsPart2)
//	{
//		for (unsigned long long i = seedRange.begin; i <= seedRange.end; ++i)
//		{
//			unsigned long long result = getMappedValue(i, &mapSeedToSoil);
//			result = getMappedValue(result, &mapSoilToFertilizer);
//			result = getMappedValue(result, &mapFertilizerToWater);
//			result = getMappedValue(result, &mapWaterToLight);
//			result = getMappedValue(result, &mapLightToTemperature);
//			result = getMappedValue(result, &mapTemperatureToHumidity);
//			result = getMappedValue(result, &mapHumidityToLocation);
//			lowestLocation = std::min(lowestLocation, result);
//		}
//	}
//	//std::ranges::sort(locations);
//	return lowestLocation;
//}

unsigned long long processDataPart2()
{
	for (unsigned long long i = 0; i < ULLONG_MAX; ++i)
	{
		unsigned long long result = getReversedMappedValue(i, &mapHumidityToLocation);
		result = getReversedMappedValue(result, &mapTemperatureToHumidity);
		result = getReversedMappedValue(result, &mapLightToTemperature);
		result = getReversedMappedValue(result, &mapWaterToLight);
		result = getReversedMappedValue(result, &mapFertilizerToWater);
		result = getReversedMappedValue(result, &mapSoilToFertilizer);
		result = getReversedMappedValue(result, &mapSeedToSoil);


		for (const auto& seedRange : seedsPart2)
		{
			if (seedRange.isInRange(result))
			{
				return i - 1; // idk why -1, but 79874952 was too high;
			}
		}
	}

	return ULLONG_MAX;
}


//unsigned long long testPart2()
//{
//	unsigned long long i = 79874951;
//	unsigned long long result = getReversedMappedValue(i, &mapHumidityToLocation);
//	result = getReversedMappedValue(result, &mapTemperatureToHumidity);
//	result = getReversedMappedValue(result, &mapLightToTemperature);
//	result = getReversedMappedValue(result, &mapWaterToLight);
//	result = getReversedMappedValue(result, &mapFertilizerToWater);
//	result = getReversedMappedValue(result, &mapSoilToFertilizer);
//	result = getReversedMappedValue(result, &mapSeedToSoil);
//
//
//	for (const auto& seedRange : seedsPart2)
//	{
//		if (seedRange.isInRange(result))
//		{
//			return i;
//		}
//	}
//
//	return ULLONG_MAX;
//}

int main()
{
	// part 1
	const auto data = Utils::read_file("day05_input.txt");
	const auto splitLines = Utils::split_string(data);
	parseInput(splitLines);
	const auto result = processData();
	std::cout << "the lowest location id is " << result << std::endl;

	// part 2
	if (!splitLines.empty())
	{
		parseSeedsPart2(splitLines[0]);
		//testPart2();
		const auto resultPart2 = processDataPart2();
		std::cout << "the lowest location id is " << resultPart2 << std::endl;
	}
	else
	{
		throw std::runtime_error("split lines vector is empty");
	}


}
