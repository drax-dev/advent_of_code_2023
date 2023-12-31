#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace Utils
{
	namespace fs = std::filesystem;

	std::string trim(std::string string_input);
	std::string read_file(const fs::path& path);
	std::vector<std::string> split_string(const std::string& input_string, char delimiter = '\n');
	std::pair<std::string, std::string> create_pair_from_line(const std::string& line, char delimiter = ' ');
	bool isNumber(char c);
	int strToInt(const std::string& str);
	int strToInt(char c);
	unsigned long long strToULL(const std::string& str);
	unsigned long long strToULL(char c);
}