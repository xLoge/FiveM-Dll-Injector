#pragma once

#include <random>
#include "../header/xorstr.h"

namespace gen
{
	std::string string(int lenght = 16)
	{
		std::string str(xorstr_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~!@#$%^&()-_=+[{}];'"));
		std::random_device rd;
		std::mt19937_64 generator(rd());
		std::shuffle(str.begin(), str.end(), generator);
		return str.substr(0, lenght);
	}
	int integer(int min = 0, int max = 100)
	{
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<int> dist(min, max);
		return dist(generator);
	}
}