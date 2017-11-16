#include "stdafx.h"
#include "HttpClient.h"

void CHttpClient::initSocket()
{
	HMODULE hModule = ::GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			_Error = "Fatal Error: MFC initialization failed";
		}
		else
		{
			AfxSocketInit(NULL);
			s_client.Create();
		}
	}
}

bool CHttpClient::Connect()
{
	CString host(_Host.c_str());
	if (s_client.Connect(host, PORT) < 0)
	{
		_Error = "Could not connect";
		return false;
	}
	return true;
}

bool CHttpClient::Request(std::string packet)
{
	int sent = 0;
	int tmpres = 0;
	const char* get2 = packet.c_str();
	while (sent < strlen(get2))
	{
		tmpres = s_client.Send(get2 + sent, strlen(get2) - sent, 0);
		if (tmpres == -1) {
			_Error = "Can't send query";
			return false;
		}
		sent += tmpres;
	}
	return true;
}

std::string CHttpClient::Response()
{
	char* buf = new char[BUFSIZ];
	memset(buf, 0, sizeof(buf));
	std::fstream fs;
	int htmlstart = 0;
	char * htmlcontent;
	//long long lenMSG = -1;
	bool isContainHeader = true;
	std::string str = "";
	int tmpres = 0;
	//if (_Protocol == "HTTP/1.0")
	//	lenMSG = 0;
	tmpres = s_client.Receive(buf, BUFSIZ, 0);
	if (tmpres < 0)
	{
		_Error = "Error receiving data";
		perror("Error receiving data");
		s_client.Close();
		return "";
	}
	std::string html(buf);
	std::regex e("Content-Type:.([^;]+)|Content-Type:.([^\n]+)");
	std::smatch match;
	bool isRG = std::regex_search(html, match, e);
	bool isFile = true;
	_isHtml = false;
	if (match[1].str().find("text/html") >= 0)
	{
		isFile = false;
		_isHtml = true;
	}
	if (isFile)
		fs.open(CHttpHelper::getNameFile(_Page), std::ios::out | std::ios::binary);
	html.clear();
	do
	{
		if (buf)
		{
			if (isContainHeader)
			{
				htmlcontent = strstr(buf, "\r\n\r\n");
				if (htmlcontent != NULL)
				{
					htmlcontent += 4;
				}
				if (isFile)
					fs.write(htmlcontent, strlen(htmlcontent));
				else
				{
					std::string temphtml(htmlcontent);
					html += temphtml;
				}
				isContainHeader = false;
				htmlcontent = NULL;
				delete htmlcontent;
			}
			else
			{
				if (isFile)
					fs.write(buf, tmpres);
				else
				{
					std::string temphtml(buf);
					html += temphtml;
				}
			}
		}
		memset(buf, 0, tmpres);
	} while ((tmpres = s_client.Receive(buf, BUFSIZ, 0)) > 0);
	
	s_client.Close();
	if (isFile)
	{
		fs.close();
		return "Download";
	}
	return html;
}

std::string CHttpClient::Get(std::string url)
{
	auto address = CHttpHeader::CreateAbsoluteAddress(url);
	_Host = address.first;
	_Page = address.second;
	bool sc = CHttpClient::Connect();
	if (!sc)
		return "Error Connect";
	std::string header = CHttpHeader::GenerateHeader("GET", _Protocol, _Host, _Page);
	Request(header);

	return Response();
}


//-----------------------------Constructor-----------------------------------

CHttpClient::CHttpClient()
{
	initSocket();
}

CHttpClient::CHttpClient(std::string protocol)
{
	if (protocol == "http1.1")
		_Protocol = "HTTP/1.1";
	else
		_Protocol = "HTTP/1.0";
	initSocket();
}

CHttpClient::CHttpClient(std::string url, std::string protocol)
{
	auto address = CHttpHeader::CreateAbsoluteAddress(url);
	_Host = address.first;
	_Page = address.second;
	if (protocol == "http1.1")
		_Protocol = "HTTP/1.1";
	else
		_Protocol = "HTTP/1.0";
	initSocket();
}

CHttpClient::~CHttpClient()
{

}

//---------------------------------------------------------------------------

std::string CHttpClient::getError()
{
	return _Error;
}

bool CHttpClient::getIsHtml()
{
	return _isHtml;
}

