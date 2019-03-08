

Modified by: Tejesh Kalicharan, Nimmagadda

Date: 17th Jan, 2019

Team members: Tejesh kalicharan, Nimmagadda (tnimmag@iu.edu id:2000432930)

              Vijaya Krishna, GopalaKrishnan Partha (vgopala@iu.edu id:2000431429)

Executable Files: server.py, client.py, http.py

Ports used: 22222, 33333, 44444, 12340, 12344

Execution Steps:

1.First execute make and make web on terminal with Makefile and record the audio from microphone as soon as Argon starts breathing (i.e. slow blinking cyan). 
2.Next execute server.py on 'iot.lukefahr.org' server Syntax to execute: python server.py 44444 (use this port for this assignment else make changes in p2.cpp). Wait a moment to recieve and it will print the values of .wav file.
3.Check for "Yes440Hz" or "No440Hz" on Argon serial terminal and check for D7 led turn on on Argon for 15secs is it is 440Hz wave.
4.Lastly execute http.py on server. Syntax to execute: python http.py Open a browser and connect to 'iot.lukefahr.org:' for display of current directory files on server
