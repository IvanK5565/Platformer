#pragma once
#include "Enums.h"
#include <string>
#include <vector>
#include <queue>
using std::string, std::vector, std::queue;

namespace myGame {
	static StatusCode code;
	static queue<string> errors;

	static string getError() {
		if (errors.empty()) return "";
		string buf = errors.front();
		errors.pop();
		if (errors.empty()) code = StatusCode::CORRECT;
		return buf;
	}
	static void setError(string mess) {
		errors.push(mess);
		code = StatusCode::UNCORRECT;
	}
}