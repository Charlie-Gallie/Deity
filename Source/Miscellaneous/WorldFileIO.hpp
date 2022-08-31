#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <regex>

namespace dty {
	static bool FileExists(const std::string& _path) {
		FILE* file;
		fopen_s(&file, _path.c_str(), "r");

		if (file) {
			fclose(file);
			return true;
		}
		return false;
	}

	using PropertyArray = std::deque<std::deque<int32_t>>;
	class WorldFileIO {
	public:
		WorldFileIO() = default;
		WorldFileIO(const std::string& _path) :
			path(_path)
		{
			Open(path);
		}

		void LoadTiles(std::string _token) {
			worldInfo = GetPropertyArray(_token, 3);
		}

		PropertyArray GetPropertyArray(std::string _token, uint32_t _elements) {
			if (!_elements)
				return {};

			// Final deque containing all the elements
			PropertyArray elements;

			// Define regex fragments
			std::string
				elementRegex = "\\{-?[\\s+0-9\\s]+",
				extraElementRegex = ",-?[\\s+0-9\\s+]+",
				propertyRegex = "(" + _token + elementRegex,
				terminationRegex = "\\})";

			// Adjust regex for variable amount of elements
			for (uint32_t index = 1; index < _elements; ++index)
				propertyRegex += extraElementRegex;

			// Append end of regex
			propertyRegex += terminationRegex;

			// Execute the regex
			std::regex searchRegex(propertyRegex);
			std::smatch searchMatch;

			for (const std::string& line : fileContents) {
				if (std::regex_search(line, searchMatch, searchRegex)) {
					std::deque<int32_t> properties;

					std::string value = searchMatch.str();

					// Match all elements -
					// Confirmed to be the right amount from the regex search above
					std::regex elementsRegex("(-?[0-9]+)");
					std::smatch elementsMatch;

					std::string::const_iterator elementStart(value.cbegin());
					while (std::regex_search(elementStart, value.cend(), elementsMatch, elementsRegex)) {
						properties.push_back(std::stoi(elementsMatch[0]));
						elementStart = elementsMatch.suffix().first;
					}

					elements.push_back(properties);
				}
			}

			return elements;
		}

		bool Open(std::string _path) {
			if (fileHandle.is_open())
				fileHandle.close();

			path = _path;

			if (!FileExists(path))
				return false;

			fileHandle.open(path);

			if (!fileHandle.good())
				return false;

			std::string line;
			while (getline(fileHandle, line))
				fileContents.push_back(line);

			return true;
		}
	private:
		std::deque<std::string> fileContents;
		PropertyArray worldInfo;
		std::ifstream fileHandle;
		std::string path = "";
	};
}