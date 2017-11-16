#pragma once
#include "afxsock.h"
#include "HttpHeader.h"
#include "HttpHelper.h"
#include <string>
#include <fstream>
#define PORT 80
class CHttpClient
{
private:
	CSocket s_client;
	std::string _Host = "";
	std::string _Page = "";
	std::string _Protocol = "";
	std::string _Error = "";
	bool _isHtml = false;
	bool Connect();
	bool Request(std::string packet);
	std::string Response();
	void initSocket();
public:
	std::string Get(std::string url);
	CHttpClient();
	CHttpClient(std::string protocol);
	CHttpClient(std::string url, std::string protocol);
	~CHttpClient();
	std::string getError();
	bool getIsHtml();

};

