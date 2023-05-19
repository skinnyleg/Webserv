#!/usr/bin/python3

import cgi
import sys
# import os
import urllib.parse

data=sys.stdin.read()

parsed_data = urllib.parse.parse_qs(data)

# Get the values of first_name and last_name
first_name = parsed_data.get('first_name', [''])[0]
last_name = parsed_data.get('last_name', [''])[0]


print("Content-type: text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Hello - Second CGI Program</title>")
print("</head>")
print("<body>")
print("<h2>Hello %s %s</h2>" % (first_name, last_name))
print("</body>")
print("</html>")
