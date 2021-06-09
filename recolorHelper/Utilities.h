#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <filesystem>

#ifdef _WIN32
#include <conio.h>
#endif

namespace fs = std::filesystem;

namespace RecolorTool
{	
	std::string stripWhiteSpace(std::string str);
	std::string stripQuotes(std::string str);
	std::string normalizePath(std::string str); 
	std::string stringToLowercase(std::string str);
	int case_insensitive_match(std::string s1, std::string s2);
	void closeProgramAfterSleep(int sleepTime);

	#ifdef _WIN32
	void closeProgramWIN32();
	#endif
}