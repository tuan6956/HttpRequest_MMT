#pragma once
class CHttpHeader
{
public:
	static std::string GenerateHeader(std::string HTTPMETHOD, std::string Protocol, std::string Host, std::string Page);
	static std::pair<std::string, std::string> CreateAbsoluteAddress(std::string address);
	CHttpHeader();
	~CHttpHeader();
};

