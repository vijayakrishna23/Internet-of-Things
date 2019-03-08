#!/usr/bin/python
import SimpleHTTPServer
import SocketServer
import sys
#from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
#from os import curdir, sep

PORT_NUMBER = int(sys.argv[1])

try:
        #Create a web server and define the handler to manage the
        #incoming request
        handler = SimpleHTTPServer.SimpleHTTPRequestHandler
        server = SocketServer.TCPServer(('', PORT_NUMBER), handler)
        print ('Started httpserver on port ' , PORT_NUMBER)

        #Wait forever for incoming htto requests
        server.serve_forever()

except KeyboardInterrupt:
        print ('shutting down the web server')
        server.socket.close()
