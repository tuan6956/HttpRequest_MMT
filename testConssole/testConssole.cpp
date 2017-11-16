
// Socket_WebClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


/* Khai bao thu vien */
#include "afxsock.h"
#include "HttpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/* Declare function */
int create_tcp_socket();
char *get_ip(char *host);
char *build_get_query(char *host, char *page);
void usage();

#define HOST "data03.chiasenhac.com"
#define BUFF 5000
#define PAGE "/"
#define PORT 80
#define USERAGENT "HTMLGET 1.1"
// The one and only application object

CWinApp theApp;

using namespace std;
string build(string host, string page, string protocol);
//Ref: http://stackoverflow.com/questions/19715144/how-to-convert-char-to-lpcwstr
wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}
void testRQ()
{
	string url = "http://students.iitk.ac.in/programmingclub/course/lectures/";
	CHttpClient* client = new CHttpClient("http1.1");
	string response = client->Get("http://students.iitk.ac.in/programmingclub/course/lectures/");
	if (client->getIsHtml())
	{
		string folderName = CHttpHelper::getNameFile(url);
		CHttpHelper::CreateFolder(folderName);
		std::vector<std::string> arr = CHttpHelper::getAllLink(response, "");
	}
}
/* Ref code:http://coding.debuntu.org/c-linux-socket-programming-tcp-simple-http-client */
int main()
{
	//testRQ();
	fstream fst;
	fst.open("doan.html", ios::in);
	string html = "";
	while (!fst.eof())
	{
		string temp;
		getline(fst, temp);
		html += temp;
	}
	CHttpHelper::getAllLink(html, "/programmingclub/course/lectures");
	
	/*cout << pch << endl;
	cout << str;*/
	/*string ab = "http://abc.com/123123.html";
	int index = ab.find("//");

	ab = ab.erase(0, index+2);
	index = ab.find("/",0);
	bool c = index == ab.length() - 1;
	std::string page = ab.substr(index, ab.length() - index);
	ab = ab.substr(0, index);
	string rs = build("portal.hcmus.edu.vn", "/", "HTTP 1.1");*/
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			/* Ref code:http://coding.debuntu.org/c-linux-socket-programming-tcp-simple-http-client */
			CSocket s_client;
			int tmpres;
			char *ip;
			const char *get2;
			char buf[BUFSIZ + 1];
			char *host;
			string page;
			//argc = 2;
			/*if (argc == 1)
			{
				usage();
				exit(2);
			}*/

			//string abc(argv[2]);
			//host = argv[1];
			host = HOST;
			/*if (argc > 2)
			{
				page = argv[2];
			}
			else
			{
				page = "48f758883b75df604f9a2c0eecb3e8e5/59e77bc2/NhacCuaTui903/AmThamBenEm-SonTungMTP-4066476.mp3";
			}*/
			page = "downloads/1531/1/1530200-d4e8f77b/128/Chua%20Bao%20Gio%20-%20Trung%20Quan%20[128kbps_MP3].mp3";

			//Buoc 1: Khoi dong socket trong Window
			AfxSocketInit(NULL);
			//Buoc 2: Tao socket
			s_client.Create();
			//Buoc 3: Ket noi toi Server
			//ip = get_ip(host);
			//fprintf(stderr, "IP is %s\n", ip);
			if (s_client.Connect(_T(HOST), PORT) < 0)
			{
				perror("Could not connect");
				exit(1);
			}
			string get = build(HOST, page, "HTTP/1.1");
			//fprintf(stderr, "%s - %s", host, page);
			//get = build_get_query(host, page);
			//fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);

			//Send the query to the server
			/*int sent = 0;
			while (sent < strlen(get))
			{
				tmpres = s_client.Send(get + sent, strlen(get) - sent, 0);
				if (tmpres == -1) {
					perror("can't send query");
					exit(1);
				}
				sent += tmpres;
			}*/
			int sent = 0;
			get2 = get.c_str();
			while (sent < strlen(get2))
			{
				tmpres = s_client.Send(get2 + sent, strlen(get2) - sent, 0);
				if (tmpres == -1) {
					perror("Can't send query");
					exit(1);
				}
				sent += tmpres;
			}
			//now it is time to receive the page
			memset(buf, 0, sizeof(buf));
			fstream fs;
			fs.open("abc2.mp3", ios::out | ios::binary);
			int htmlstart = 0;
			char * htmlcontent;
			long long lenMSG = -1;
			string str = "";
			
			while ((tmpres = s_client.Receive(buf, BUFSIZ, 0)) > 0)
			{
				//if(htmlstart == 0)
				//{
				//	/* Under certain conditions this will not work.
				//	* If the \r\n\r\n part is splitted into two messages
				//	* it will fail to detect the beginning of HTML content
				//	*/
				//	htmlcontent = strstr(buf, "\r\n\r\n");
				//	if(htmlcontent != NULL)
				//	{
				//		htmlstart = 1;
				//		htmlcontent += 4;
				//	}
				//}
				//else
				//{
				//		htmlcontent = buf;
				//}
				if (buf) {
					//cout << strlen(buf);
					/*buf[strlen(buf)] = '\0';
					cout << sizeof(buf);*/
					if (lenMSG == -1)
					{
						string az(buf);
						regex e("Content-Length: ([0-9]+)");
						smatch mt;
						bool abc = regex_search(az, mt, e);
						cout << mt[1];
						string len = mt[1];
						lenMSG = stoll(len);
						//lenMSG = stoll(mt[1]);
						int index = az.find("\r\n\r\n");
						lenMSG -= az.length() - index - 4;
						htmlcontent = strstr(buf, "\r\n\r\n");
						int size = 0;
						for (int i = 0; i < 511; i++)
						{
							if (buf[i] == 13 && buf[i + 1] == 10 && buf[i + 2] == 13 && buf[i + 3] == 10)
							{
								size = i + 4;
								break;
							}
						}
						if (htmlcontent != NULL)
						{
							htmlcontent += 4;
						}
						string testtt(htmlcontent);
						for (int i = 0; i < 512; i++)
						{
							cout << testtt[i];
						}
						fs.write(htmlcontent, 512 - size);
						//fs.close();
						htmlcontent = NULL;
						delete htmlcontent;
					}
					else
					{

						fs.write(buf, tmpres);
						lenMSG -= tmpres;
					}
					/*char * pch;
					pch = strstr(buf, "\r\n0\r\n\r\n");
					if (pch != NULL && strcmp(pch, "\r\n0\r\n\r\n") == 0)
					{
						cout << "CO2";

						int acx = 0;
						break;

					}
					if(lenMSG <= 19)
						cout << lenMSG << endl;
					if (lenMSG <= 0)
					{
						cout << "CO1";
						break;
					}*/
				}
				memset(buf, 0, tmpres);
			}
			fs.close();
			/*fs.open("abc2.mp3", ios::in | ios::binary);

			while (!fs.eof())
			{
				fs.read(buf, BUFSIZ);
				for (int i = 0; i < BUFSIZ; i++)
				{
					cout << buf[i];
				}
			}*/
			if (tmpres < 0)
			{
				perror("Error receiving data");
			}
			s_client.Close();
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}
	system("pause");

	return nRetCode;
}


void usage()
{
	fprintf(stderr, "USAGE: htmlget host [page]\n\
		\thost: the website hostname. ex: coding.debuntu.org\n\
		\tpage: the page to retrieve. ex: index.html, default: /\n");
}

/*
Get ip from domain name
*/
char *get_ip(char *host)
{
	struct hostent *hent;
	int iplen = 15; //XXX.XXX.XXX.XXX
	char *ip = (char *)malloc(iplen + 1);
	memset(ip, 0, iplen + 1);
	if ((hent = gethostbyname(host)) == NULL)
	{
		perror("Can't get IP");
		exit(1);
	}
	if (inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
	{
		perror("Can't resolve host");
		exit(1);
	}
	return ip;
}


char *build_get_query(char *host, char *page)
{

	char *query;
	char *getpage = page;
	char *tpl = "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	if (getpage[0] == '/') {
		getpage = getpage + 1;
		fprintf(stderr, "Removing leading \"/\", converting %s to %s\n", page, getpage);
	}
	fprintf(stderr, "\nvo");

	// -5 is to consider the %s %s %s in tpl and the ending \0
	query = new char[(strlen(host) + strlen(getpage) + strlen(USERAGENT) + strlen(tpl) - 5)];
	fprintf(stderr, "\ndu lieu %s - %s - %s", query, getpage, tpl);
	sprintf_s(query, strlen(query) + 1, tpl, getpage, host, USERAGENT);

	return query;
}
string build(string host, string page, string protocol) {

	string METHOD = "GET ";
	if (page[0] == '/') {
		page.erase(0, 1);
	}
	string sHost = "Host: " + host;
	string connection = "Connection: close";
	string agent = "User-Agent: HTMLGET 1.0";
	string newline = "\r\n";
	return METHOD + "/" + page + " " + protocol + newline + sHost + newline + agent + newline + connection + newline + newline;
}
