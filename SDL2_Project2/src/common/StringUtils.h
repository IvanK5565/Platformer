#pragma once
#include "Enums.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
using std::string, std::vector, std::queue;

namespace myGame {
	static string trim(const std::string& source) {
		std::string s(source);
		s.erase(0, s.find_first_not_of(" \n\r\t"));
		s.erase(s.find_last_not_of(" \n\r\t") + 1);
		return s;
	}
	static string trim(const std::string& source, const char c) {
		std::string s(source);
		s.erase(0, s.find_first_not_of(c));
		s.erase(s.find_last_not_of(c) + 1);
		return s;
	}
	static size_t split(const string& txt, vector<string>& strs, char ch)
	{
		size_t pos = txt.find(ch);
		size_t initialPos = 0;
		strs.clear();

		// Decompose statement
		while (pos != string::npos) {
			strs.push_back(txt.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = txt.find(ch, initialPos);
		}

		// Add the last one
		strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

		return strs.size();
	}
	static vector<string> split(const string& txt, char ch)
	{
		vector<string> strs;
		split(txt, strs, ch);
		return strs;
	}
}