#include "stdafx.h"
#include "HttpHelper.h"


std::string CHttpHelper::getNameFile(std::string page)
{
	if (page[page.length() - 1] == '/')
		page = page.erase(page.length() - 1, 1);
	int index = page.find_last_of('/');
	return page.substr(index + 1, page.length() - index - 1);
}

void CHttpHelper::CreateFolder(std::string name)
{
	std::string temp = "mkdir " + name;
	system(temp.c_str());
}

std::vector<std::string> CHttpHelper::getAllLink(std::string html, std::string pageParent)
{
	std::vector<std::string> arrUrl;
	int index = 0;
	int index2 = 0;
	while (true)
	{
		index = html.find("href=\"", index2);
		if (index < 0)
			break;
		index += 6;
		index2 = html.find("\"", index);
		if (index2 < 0)
			break;
		std::string url = html.substr(index, index2 - index);
		int iFind = pageParent.find(url);
		if (iFind >= 0)
			continue;
		if (url[0] == '/')
			url = url.erase(0, 1);
		arrUrl.push_back(pageParent + "/" + url);
		index2 += 1;
	}
	return arrUrl;
}





CHttpHelper::CHttpHelper()
{
}


CHttpHelper::~CHttpHelper()
{
}
