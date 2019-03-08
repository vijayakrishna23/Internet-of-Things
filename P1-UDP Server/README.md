Modified by: Tejesh Kalicharan, Nimmagadda

Date: 17th Jan, 2019

Team members: Tejesh kalicharan, Nimmagadda (tnimmag@iu.edu id:2000432930)

               Vijaya Krishna, GopalaKrishnan Partha (vgopala@iu.edu id:2000431429)
              
Executable Files: server.py, client.py, http.py

Ports used: 22222, 33333, 44444, 12340, 12344

Execution Steps:

1.First execute server.py on 'iot.lukefahr.org' server
Syntax to execute: python server.py <port>
Once the file is recieved 'File Recieved' Message is displayed on the terminal.

2.Next execute client.py from another node
Syntax to execute: python client.py <port> <filename>
Note: For 'filename' either the file should be in same directory as client.py else it's complete path has to be given.
Once the file is sent, 'Transfer Done' is displayed on the terminal.

3.Lastly execute http.py on server.
Syntax to execute: python http.py <port>
Open a browser and connect to 'iot.lukefahr.org:<port>' for display of current directory files on server.

