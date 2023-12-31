#include "Utils.h"
#include <sstream>
#include <fstream>
#include <iostream>

namespace Utils
{
	std::string trim(std::string string_input)
	{
		constexpr auto whitespaces = " \t\n\r\f\v";
		string_input.erase(string_input.find_last_not_of(whitespaces) + 1);
		string_input.erase(0, string_input.find_first_not_of(whitespaces));
		return string_input;
	}

	std::string read_file(const fs::path& path)
	{
		std::string result{};
		if (std::ifstream f{ path, std::ios::in | std::ios::binary })
		{
			const auto size = file_size(path);
			result = std::string(size, '\0');
			f.read(result.data(), static_cast<long long>(size));
		}
		else
		{
			throw std::runtime_error("Unable to read the file.");
		}
		return result;
	}

	std::vector<std::string> split_string(const std::string& input_string, const char delimiter)
	{
		std::vector<std::string> results;
		std::string temp_string;
		std::stringstream ss(input_string);
		while (std::getline(ss, temp_string, delimiter))
		{
			temp_string = trim(temp_string);
			results.emplace_back(temp_string);
		}
		return results;
	}

	std::pair<std::string, std::string> create_pair_from_line(const std::string& line, const char delimiter)
	{
		std::string first;
		if (std::stringstream ss(line); std::getline(ss, first, delimiter))
		{
			first = trim(first);
			std::string second;
			std::getline(ss, second);
			second = trim(second);
			return std::make_pair(first, second);
		}
		return {};
	}

	bool isNumber(const char c)
	{
		if (c >= '0' && c <= '9')
		{
			return true;
		}
		return false;
	}

	int strToInt(const std::string& str)
	{
		try
		{
			return std::stoi(str);
		}
		catch (std::invalid_argument const& ex)
		{
			std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
		}
		catch (std::out_of_range const& ex)
		{
			std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
		}
		return -1;
	}

	int strToInt(const char c)
	{
		try
		{
			const std::string str{ c };
			return std::stoi(str);
		}
		catch (std::invalid_argument const& ex)
		{
			std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
		}
		catch (std::out_of_range const& ex)
		{
			std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
		}
		return -1;
	}

	unsigned long long strToULL(const std::string& str)
	{
		try
		{
			return std::stoull(str);
		}
		catch (std::invalid_argument const& ex)
		{
			std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
		}
		catch (std::out_of_range const& ex)
		{
			std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
		}
		return -1;
	}

	unsigned long long strToULL(const char c)
	{
		try
		{
			const std::string str{ c };
			return std::stoull(str);
		}
		catch (std::invalid_argument const& ex)
		{
			std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
		}
		catch (std::out_of_range const& ex)
		{
			std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
		}
		return -1;
	}
}
