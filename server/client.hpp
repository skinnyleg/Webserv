#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <codecvt>
#include <strings.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <map>
#include <cstring>
#include <sys/socket.h>
#include <fstream>
#include <fcntl.h>
#include <cstdlib>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <poll.h>
#include <dirent.h>

enum {
	EMPTY,
	GET,
	POST,
	DELETE,
	CHUNKED,
	FORM,
	NONCHUNKED,
	ERROR,
	CREATED,
	CLOSE,
	EXIST,
	UPDATED,
	DELETED,
	NOTFOUND,
	FORBIDEN,
	ALIVE,
	OPFILE,
	REDIRECT,
	AUTOINDEX,
	CONFLICT,
	INTERNALERR,
	CGI
};

using std::string;
using std::vector;
#define BUFFER 500000

class client;

class parssingOfBody
{
    private:

    public:
        std::string             file;
        std::string             exetention;
        int                     fd;

        parssingOfBody(/* args */);
        void handle_post(client &obj, std::multimap<std::string, std::string>);
        void handling_chunked_data(client &obj, std::multimap<std::string, std::string>);
        void handling_form_data(client &obj, std::multimap<std::string, std::string>);
		void putDataTofile(string  data, client & obj, std::multimap<std::string, std::string>);
        void  create_file_and_put_content(std::string & bodyofRequest,std::string & headerOfRequest, int&created, std::string path, std::multimap<std::string, std::string>);
		void clear();

        ~parssingOfBody();
};

class response
{
    public:
		int status_code;
		int type;
		std::string response_req;
		std::string body;
		std::vector<std::string>headers;
		std::string version;
		std::string del;
		std::string closeheader;
		std::string aliveheader;
		int close;
		std::string contentlength;
		std::string contenttype;
		std::string redirectUrl;
		int content;
		int flagResponse;
		int ready;

        response(/* args */);
		void generate_response(std::map<int, std::string>);
		int send_response(client &obj, struct pollfd &pfds);
		void defineContentType();
		response(const response &obj);
		response& operator=(const response &obj);
		void clear();
        ~response();

};

class parssingOfHeader
{
    private:

    public:

        parssingOfHeader();

        long long	ft_atoi(const char *str);

        int checkHeaderLine(client &obj, std::map<std::string, std::string> Percent);
		int checkHeaderOfreq_(client &obj, std::string copy, std::map<std::string, std::string> Percent, std::multimap<std::string, std::string>);
		int VerifyURI(client &obj, std::map<std::string, std::string> PercentEncoding);
		int check_media(client &obj, std::multimap<std::string, std::string>);
        int checkHeaders(client &obj, std::string copy, std::multimap<std::string, std::string>);
        ~parssingOfHeader();
};


class client
{
	public:
	int client_socket;
	std::string buffer;
	std::string boundary;
	std::string response_header;
	std::string body;
	std::ifstream input;
	std::string headerOfRequest;
	std::string bodyofRequest;
	std::ofstream file;
	// std::vector<char> content_buffer;
	std::string URI;
	int bytes_read;
	int flag;
	int ready;
	std::string path;
	std::string redirpath;
	std::string uploadPath;
	std::string listPath;


	parssingOfHeader headerParss;
	parssingOfBody   bodyParss;
	response respond;
	int tmp;
	int flag_;
	int total_bytes_received;
	int i;
	int j;
	int len;
	int flag_res;
	int ContentLength;

	int extractheader();
	void openfile();
	// int response(int pfds_index, vector<struct pollfd> &pfds);
	int pushToBuffer();
	void clear();
	int checkHeaderOfreq(std::map <std::string, std::string>,std::multimap <std::string, std::string> );
	long long	ft_atoi(const char *str);
	char *ft_substr(char const *s, unsigned int start, size_t len);
	void check(void);
	int postMethod(std::multimap<std::string, std::string> mimetypes_);
	void initResponse(std::map<std::string, std::string>);
	int fillBody(std::map<std::string, std::string>);
	int generateListing(std::map<std::string, std::string>);
	void generateUrl();
	client();
	client(const client &obj);
	client& operator=(const client& obj);
	~client();
};

#endif
