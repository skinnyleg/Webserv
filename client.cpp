#include "client.hpp"

int client::extractheader()
{
	size_t res = 0;
	size_t pos = 0;

	res = buffer.find("/favicon.ico");
	// std::cout << "sock " << this->client_socket << std::endl;
	// std::cout << buffer << std::endl;
	// std::cout << "end print" << std::endl;
	if (res != std::string::npos)
	{
		std::cout << "lol\n";
		this->ignore = 1;
		return (1);
	}
	// std::cout << "extract sock " << this->client_socket << std::endl;
	if (this->header_request.empty())
	{
		while (pos < buffer.size())
		{
			res = buffer.find("\r\n", pos);
			if (res != std::string::npos)
			{
				if (buffer[res] == '\r' && buffer[res + 1] == '\n')
				{
					res += 2;
					if (buffer[res] == '\r' && buffer[res + 1] == '\n')
					{
						header_request = buffer.substr(0, res + 1);
						// std::cout << "extracted header "<< this->client_socket << std::endl;
						// std::cout << header_request << std::endl;
						// std::cout << "end header" << std::endl;
						buffer.erase(0, res + 2);
						break ;
					}
					else
						pos = res;
				}
			}
			else
				pos += 1;
		}
	}
	return (0);
}

void client::openfile()
{
	size_t res = 0;
	res = header_request.find("Content-Type");
	if (res != std::string::npos)
	{
		if (!file.is_open())
		{
			res = header_request.find("pdf");
			if (res != std::string::npos)
				file.open("file.pdf");
			res = header_request.find("mp4");
			if (res != std::string::npos)
				file.open("file.mp4");
			res = header_request.find("png");
			if (res != std::string::npos)
				file.open("file.png");
			res = header_request.find("text");
			if (res != std::string::npos)
				file.open("file");
		}
		if (!file.is_open())
			file.open("data");
	}
}

int client::response()
{
	char c;

	if (file.is_open())
	{
		file << buffer;
		file.flush();
	}
	else
	{
		if (!input.is_open())
		{
			input.open("../test/vid.mp4");
			if (!input.is_open())
			{
				std::cout << "couldn't open file" << std::endl;
				return (1);
			}
			while (input.get(c))
				content_buffer.push_back(c);
			response_header = "HTTP/1.1 200 OK\r\n"
							"Content-Type: video/mp4\r\n"
							"Content-length: " + std::to_string(content_buffer.size()) + "\r\n"
							"\r\n";
			// int i = send(this->client_socket, response_header.c_str(), response_header.size(), 0);
			// std::cout << "socket client " << this->client_socket << std::endl;
			// std::cout << "header req" << std::endl;
			// std::cout << header_request << std::endl;
			// std::cout << "end client " << this->client_socket << std::endl;
			// if (i < 0)
			// {
			// 	std::cout << "send error" << std::endl;
			// 	return (1);
			// }
			// if (i > 0)
			// 	std::cout << "sent success " << this->client_socket << std::endl;
			// response_header.clear();
			std::cout << "sock response " << this->client_socket << std::endl;
			response_header += std::string(content_buffer.begin(), content_buffer.end());
		}
		size_t size = 100000;
		if (!response_header.empty())
		{
			if (response_header.size() < size)
				size = response_header.size();
			std::string send_req = response_header.substr(0, size);
			send(this->client_socket, send_req.c_str(), send_req.size(), 0);
			response_header.erase(0, size);
		}
		else
		{
			std::cout << "sent complete " << this->client_socket << std::endl;
			return (0);
		}
	}
	return (0);
}

client::client()
{
	this->ignore = 0;
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
		this->header_request = obj.header_request;
		this->bytes_read = obj.bytes_read;
		this->ignore = obj.ignore;
	}
	return (*this);
}

client::~client()
{
}
