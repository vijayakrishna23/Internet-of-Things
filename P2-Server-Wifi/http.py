# -*- coding: utf-8 -*-
"""
P1-UDP Server
Created by: Tejesh Kalicharan, Nimmagadda
tnimmag@iu.edu id:2000432930
Date: 11th Jan, 2019
"""

#!/usr/bin/python
import SimpleHTTPServer
import SocketServer
import sys

PORT_NUMBER = int(sys.argv[1])

try:
        #Create a web server and define the handler to manage the
        #incoming request
        handler = SimpleHTTPServer.SimpleHTTPRequestHandler
        server = SocketServer.TCPServer(('', PORT_NUMBER), handler)
        print ('Started httpserver on port ' , PORT_NUMBER)

        #Wait forever for incoming http requests
        server.serve_forever()

except KeyboardInterrupt:
        #Exit the server on pressing Ctrl-C
        print ('Shutting down the web server')
        server.socket.close()
