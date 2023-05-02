#include "client.hpp"

// void client::normal_response(struct pollfd &pfds)
// {
// 	if (response_header.empty())
// 	{
// 		std::string res = "hello from server";
// 		response_header = "HTTP/1.1 200 OK\r\n"
// 						"Content-Type: text/plain\r\n"
// 						"Content-length: " + std::to_string(res.size()) + "\r\n"
// 						"\r\n";
// 		response_header += std::string(res.begin(), res.end());
// 	}
// 	if (!response_header.empty())
// 	{
// 		// //std::cout << "send chunks" << std::endl;
// 		int i = send(this->client_socket, response_header.c_str(), response_header.size(), 0);
// 		if (i < 0)
// 		{
// 			//std::cout << "error "  << this->client_socket << std::endl;
// 			//std::cout << "ready == " << this->ready << " socket client == " << this->client_socket << std::endl;
// 			// //std::cout << this->headerOfRequest << std::endl;
// 			printf("errno = %d: %s\n", errno, strerror(errno));
// 			// response_header.clear();
// 			return ;
// 		}
// 		if (i == (int)response_header.size())
// 		{
// 			//std::cout << "sent complete " << this->client_socket << std::endl;
// 			//std::cout << "ready -- " << this->ready << std::endl;
// 			//std::cout << this->headerOfRequest << std::endl;
// 			// //std::cout << this->buffer << std::endl;
// 			this->clear();
// 			pfds.revents &= ~POLLOUT;
// 			return ;
// 		}
// 		response_header.erase(0, i);
// 		// //std::cout << "i == " << i  << " socket == "  << this->client_socket << std::endl;
// 	}
// 	else
// 	{ 
// 		//std::cout << "sent complete " << this->client_socket << std::endl;
// 		//std::cout << "ready -- " << this->ready << std::endl;
// 		//std::cout << this->headerOfRequest << std::endl;
// 		this->clear();
// 		pfds.revents &= ~POLLOUT;
// 		return ;
// 	}
// }

int client::normal_response(struct pollfd &pfds)
{
	// char c;

	// //std::cout << "normal response" << std::endl;
	if (!input.is_open())
	{
		// //std::cout << "lol2 " << std::endl;
		// input.open("../tests/pdf.pdf");
		input.open("../tests/upload.html");
		if (!input.is_open())
		{
			//std::cout << "couldn't open file" << std::endl;
			return (1);
		}
		// while (input.get(c))
		// {
		// 	//std::cout << "loop" << std::endl;
		// 	content_buffer.push_back(c);
		// }
		input.seekg(0, std::ios::end);
		size_t size = input.tellg();
		input.seekg(0, std::ios::beg);

		// Reserve space in the buffer
		std::vector<char> content(size);

		// Read the file in chunks
		input.read(&content[0], size);
		response_header = "HTTP/1.1 200 OK\r\n"
						"Content-Type: text/html\r\n"
						"Content-length: " + std::to_string(content.size()) + "\r\n"
						"\r\n";
		response_header += std::string(content.begin(), content.end());
	}
	if (!response_header.empty())
	{
		// //std::cout << "send chunks" << std::endl;
		int i = send(this->client_socket, response_header.c_str(), response_header.size(), 0);
		if (i < 0)
		{
			//std::cout << "error "  << this->client_socket << std::endl;
			//std::cout << "ready == " << this->ready << " socket client == " << this->client_socket << std::endl;
			//std::cout << this->headerOfRequest << std::endl;
			printf("errno = %d: %s\n", errno, strerror(errno));
			// response_header.clear();
			return (0);
		}
		response_header.erase(0, i);
		// //std::cout << "i == " << i  << " socket == "  << this->client_socket << std::endl;
	}
	else
	{ 
		//std::cout << "sent complete " << this->client_socket << std::endl;
		//std::cout << "ready -- " << this->ready << std::endl;
		//std::cout << this->headerOfRequest << std::endl;
		headerOfRequest.clear();
		// content_buffer.clear();
		buffer.clear();
		ready = 0;
		flag = 0;
		pfds.revents &= ~POLLOUT;
		input.close();
		return (0);
	}
	return (0);
}

void client::check(void)
{
	int res;

	res = headerOfRequest.find("/favicon.ico");
	if (res != -1)
		this->ready = 0;
	else
		this->ready = 1;
}

void client::clear()
{
	headerOfRequest.clear();
	bodyParss.clear();
	boundary.clear();
	bodyofRequest.clear();
	buffer.clear();
	ready = 0;
	total_bytes_received = 0;
	respond.clear();
	flag = 0;
	tmp = 0;
	flag_ = 0;
	input.close();
}

client::client()
{
    total_bytes_received  = 0;
    i  = flag = flag_  = j = 0;
    tmp = 0;
    buffer = bodyofRequest = boundary = "";
    tmp = 0;
	ready = 0;
	flag_res = 0;
}

client::client(const client &obj)
{
	*this = obj;
}

client& client::operator=(const client& obj)
{
	if (this != &obj)
	{
		this->client_socket = obj.client_socket;
		this->buffer = obj.buffer;
		this->boundary = obj.boundary;
		this->response_header = obj.response_header;
		this->body = obj.body;
		this->headerOfRequest = obj.headerOfRequest;
		this->bodyofRequest = obj.bodyofRequest;
		// this->content_buffer = obj.content_buffer;
		this->bytes_read = obj.bytes_read;
		this->flag = obj.flag;
		this->bodyParss = obj.bodyParss;
		this->headerParss = obj.headerParss;
		this->tmp = obj.tmp;
		this->flag_ = obj.flag_;
		this->flag_res = obj.flag_res;
		this->respond = obj.respond;
		this->total_bytes_received = obj.total_bytes_received;
		this->i = obj.i;
		this->j = obj.j;
		this->len = obj.len;
		this->ContentLength = obj.ContentLength;
	}
	return (*this);
}

client::~client()
{
}

int client::checkHeaderOfreq()
{
    int pos = 0;
	int _tmp = 0;
    
    while (buffer[pos] && flag == 0)// for entring one time
    { 
        if((buffer[pos] == '\r' || buffer[pos] == '\n') && buffer[pos + 1] == '\n')
        {
            pos += 2;
            if((buffer[pos] == '\r' || buffer[pos] == '\n') && buffer[pos + 1] == '\n' )
            {
                headerOfRequest = buffer.substr(0,pos - 1);// not include \r\n
				std::string copyheader = headerOfRequest;
				this->flag_res = headerParss.checkHeaderOfreq_(*this, copyheader);
				if (this->flag_res < 0)
				{
					flag = ERROR;
					return (1);
				}
                // if(headerParss.checkHeaderOfreq_(headerOfRequest,tmp) == -2)
                //     return -2;
				if (this->tmp == POST)
				{
					i = copyheader.find("Transfer-Encoding: chunked");   // find way to check if boundry
					if(i != -1)
					{ 
						//std::cout << "lol" << std::endl;
						i = pos  + 2;
						pos = copyheader.find("Content-Length");  
						//std::cout << copyheader<< std::endl;
						if(pos != -1)
							ContentLength = ft_atoi(copyheader.substr(pos + 16,copyheader.size()).c_str());
						
						flag = CHUNKED;
						return 1;
					}
					pos = copyheader.find("Content-Length");  
					if(pos != -1)
					{
						j = copyheader.find("boundary");
						if(j != -1)
						{
							flag = FORM;
							ContentLength = ft_atoi(copyheader.substr(pos + 16,copyheader.size()).c_str());
							// if(ContentLength == 0)
							//     return -2;
							
							i = headerOfRequest.size() + 3;// after herder
							bytes_read -= i;
							_tmp = j + 9;
							char *temp = (char*)copyheader.data() + _tmp;// because string() dont handle '\r'
							_tmp = 0;
							while (temp[_tmp] != '\r' && temp[_tmp] != '\n' && temp[_tmp + 1] != '\n')
								_tmp++;
							char *strtmp = ft_substr(temp,0,_tmp);
							boundary.append("--").append(strtmp);// free boundry and temp?
							free(strtmp);
							// //std::cout << "=> " <<  boundary << std::endl;
							return 1;
						}
						ContentLength = ft_atoi(copyheader.substr(pos + 16,copyheader.size()).c_str());
						// if(ContentLength == 0)
						//     return -2;
						flag = NONCHUNKED;
						i = headerOfRequest.size();
						return  1;
					}
				}
				else if (this->tmp == GET)
				{
					flag = GET;
					return (1);
				}
				else if (this->tmp == DELETE)
				{
					flag = DELETE;
					return (1);
				}
                // else
                // {
                //     // without body
                //     flag = GET;
                //     return 1;
                // }
            }
            --pos;
        }
        pos++;
    }
    // in entring second times
    if(flag == GET || flag == CHUNKED || flag == NONCHUNKED || flag == FORM || flag == ERROR || flag == DELETE)
        return 1;
    else
        return -2;
}

long long	client::ft_atoi(const char *str)
{
	long long	res;
	long long	negative;

	negative = 1;
	res = 0;
	while (*str && (*str == ' ' || *str == '\n' || *str == '\t' ||
			*str == '\v' || *str == '\f' || *str == '\r'))
		++str;
	if (*str == '-')
		negative = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (*str && *str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - 48);
		++str;
	}
	return (res * negative);
}
  
char * client::ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	str = (char*)malloc(sizeof(*s) * (len - start + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i >= start && j < len - start)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
	return (str);
}  

int client::pushToBuffer()
{
    char data[BUFFER];
    bzero(data, BUFFER);
    this->bytes_read = recv(client_socket, &data, BUFFER, 0);

    if(bytes_read == -1)
        return -1;
    if(bytes_read == 0)
        return 0;
    int j = 0; 
    
    while (j < bytes_read)
    {
        buffer.push_back(data[j]);
        j++;
    }
    return this->bytes_read;
}

int client::deleteMethod(struct pollfd &pfds)
{
	//std::cout << "hello from delete"  << std::endl;
	//std::cout << "URI -- " << URI << std::endl;
	std::string str;
	if (URI.size() <= 7)
		respond.flagResponse = FORBIDEN;
	else
	{
		for (int i = 0; i < 7; i++)
			str.push_back(URI[i]);
		if (str.compare("upload/") == 0)
		{
			int i = remove((char *)URI.data());
			if (i  == 0)
			{
				//std::cout << "removed successfully" << std::endl;
				this->respond.flagResponse = DELETED;
			}
			else
			{
				//std::cout << "error on remove" << std::endl;
				this->respond.flagResponse = NOTFOUND;
			}
		}
		else
			respond.flagResponse = FORBIDEN;
	}
	this->respond.ready = 1;
	if (this->respond.ready == 1)
	{
		if (this->respond.flagResponse == DELETED)
		{
			this->respond.status_code = 204;
			this->respond.phrase = "No Content";
			this->respond.type = 1;
			// this->respond.content = 1;
			// this->respond.body = "Resource Already Exist";
		}
		if (this->respond.flagResponse == NOTFOUND)
		{
			this->respond.status_code = 404;
			this->respond.phrase = "Not Found";
			this->respond.type = 1;
			this->respond.content = 1;
			this->respond.body = "Resource Doesn't Exist";
		}
		if (this->respond.flagResponse == FORBIDEN)
		{
			this->respond.status_code = 403;
			this->respond.phrase = "Forbidden";
			this->respond.type = 1;
			this->respond.content = 1;
			this->respond.body = "You Don't Have Permession To Do That";
		}
		this->respond.generate_response();
		int i = this->respond.send_response(*this ,pfds);
		if (i == 0)
		{
			this->clear();
			pfds.revents &= ~POLLOUT;
		}
		else if (i == CLOSE)
			return (CLOSE);
	}
	return (0);
}

int client::postMethod(struct pollfd &pfds)
{
	if(this->flag == NONCHUNKED) // if has content length
	{
		//std::cout << "post handle1" << std::endl;
		this->bodyParss.handle_post(*this);
		this->respond.ready = 1;
	}
	else if(this->flag == CHUNKED)// // handle chunked data when resend request
	{
		//std::cout << "chunked handle1" << std::endl;
		this->bodyParss.handling_chunked_data(*this);
		this->respond.ready = 1;
	}
	else if(this->flag == FORM)
	{
		//std::cout << "form handle1" << std::endl;
		this->bodyParss.handling_form_data(*this);
		this->respond.ready = 1;
	}
	if (this->respond.ready == 1)
	{
		if (this->respond.flagResponse == CREATED)
		{
			this->respond.status_code = 201;
			this->respond.phrase = "created";
			this->respond.type = 1;
			this->respond.content = 1;
			// this->respond.headers.push_back("Location: " + )
			this->respond.body = "successfully uploaded";
		}
		if (this->respond.flagResponse == EMPTY)
		{
			this->respond.status_code = 204;
			this->respond.phrase = "No Content";
			this->respond.type = 1;
			// this->respond.content = 1;
			// this->respond.body = "No Body Found";
		}
		if (this->respond.flagResponse == EXIST)
		{
			this->respond.status_code = 409;
			this->respond.phrase = "Conflict";
			this->respond.type = 1;
			this->respond.content = 1;
			this->respond.body = "Resource Already Exist";
		}
		this->respond.generate_response();
		int i = this->respond.send_response(*this ,pfds);
		if (i == 0)
		{
			this->clear();
			pfds.revents &= ~POLLOUT;
		}
		else if (i == CLOSE)
			return (CLOSE);
	}
	return (0);
}