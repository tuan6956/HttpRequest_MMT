#include "stdafx.h"
#include "HttpHeader.h"




std::string CHttpHeader::GenerateHeader(std::string HTTPMETHOD, std::string Protocol, std::string Host, std::string Page)
{
	std::string METHOD = "GET ";
	if (Page[0] == '/') {
		Page.erase(0, 1);
	}
	std::string sHost = "Host: " + Host;
	std::string agent = "User-Agent: HTMLGET 1.0";
	std::string newline = "\r\n";
	std::string Connection = "";
	if (Protocol == "HTTP/1.1")
		Connection = "Connection: close" + newline;
	return HTTPMETHOD + " /" + Page + " " + Protocol + newline + sHost + newline + agent + newline + Connection + newline;
	return std::string();
}

std::pair<std::string, std::string> CHttpHeader::CreateAbsoluteAddress(std::string address)
{
	
	if (address == "")
		return std::pair<std::string, std::string>();
	int index = address.find("//");
	if(index < 0)
		return std::pair<std::string, std::string>();
	address = address.erase(0, index+2);
	index = address.find("/");
	if(index < 0 || index == address.length() - 1)
		return std::pair<std::string, std::string>(address,"/");
	std::string page = address.substr(index, address.length() - index);
	if (page[page.length() - 1] == '/')
		page = page.erase(page.length() - 1, 1);
	std::string host = address.substr(0, index);
	return std::pair<std::string, std::string>(host, page);
}

CHttpHeader::CHttpHeader()
{
}


CHttpHeader::~CHttpHeader()
{
}
