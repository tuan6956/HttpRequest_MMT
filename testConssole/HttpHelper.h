#pragma once
#include <string>
#include <vector>
#include <regex>
class CHttpHelper
{
public:
	static std::string getNameFile(std::string url);
	static void CreateFolder(std::string name);
	static std::vector<std::string> getAllLink(std::string html, std::string urlParent);
	CHttpHelper();
	~CHttpHelper();
};

