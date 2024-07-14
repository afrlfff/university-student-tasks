#pragma once

#include <string>
#include <vector>

std::vector<std::string> Split(const std::string& str, char c)
{
	std::string tmp;
	std::vector<std::string> result;
	for (int i = 0; i < str.size(); ++i)
	{
		if (str[i] == c) {
			result.push_back(tmp);
			tmp.clear();
		} else if (i == (str.size() - 1)) {
            result.push_back(tmp + std::string(1, str[i]));
        } else {
			tmp.push_back(str[i]);
		}
	}
	return result;
}