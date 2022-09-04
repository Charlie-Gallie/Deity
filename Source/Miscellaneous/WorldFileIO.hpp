#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <regex>

#include "Miscellaneous/Vector2.hpp"

/*
	The information provided/not provided by the user of this
	class is inconsistent and should be changed but refactoring
	will probably take a bit too long.
*/

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

		~WorldFileIO() {
			Close();
		}

		void LoadTiles(std::string _token) {
			worldInfo.clear();

			PropertyArray worldSizePropertyArray = GetPropertyArray("s", 2);
			worldSize = {
				(uint32_t)worldSizePropertyArray[0][0],
				(uint32_t)worldSizePropertyArray[0][1]
			};

			worldInfo = GetPropertyArray(_token, tilePropertyElements);
		}

		std::deque<int32_t> GetTileElements(Vector2u _tilePosition) {
			return worldInfo[_tilePosition.y * worldSize.x + _tilePosition.x];
		}

		std::deque<int32_t> GetTileElements(size_t _index) {
			return worldInfo[_index];
		}

		void SetTileElements(Vector2u _tilePosition, std::deque<int32_t> _elements) {
			worldInfo[_tilePosition.y * worldSize.x + _tilePosition.x] = _elements;
		}

		void SetTileElements(size_t _index, std::deque<int32_t> _elements) {
			worldInfo[_index] = _elements;
		}

		void Save() {
			std::ofstream writeFileHandle(path);

			writeFileHandle << "s{" << worldSize.x << "," << worldSize.y << "}\n";
			writeFileHandle << "p{700,-1200}\n\n";

			for (auto& a : worldInfo) {
				writeFileHandle << "t{" << a[0] << "," << a[1] << "," << a[2] << "}\n";
			}

			writeFileHandle.close();
		}

		void Close() {
			fileHandle.close();
		}

		PropertyArray GetPropertyArray(std::string _token, uint32_t _elements) {
			if (!_elements)
				return {};

			// Todo: Allocate some space if I can figure out how much will be written

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
		std::ifstream fileHandle;
		std::string path = "";

		PropertyArray worldInfo;
		Vector2u worldSize = 0;

		const std::string tileToken = "t";
		const uint32_t tilePropertyElements = 3; // The amount of elements in a single world file property, e.g., t{n, n, n}
	};
}