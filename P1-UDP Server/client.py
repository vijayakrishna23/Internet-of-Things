# -*- coding: utf-8 -*-
"""
P1-UDP server
Created by: Vijaya Krishna, Gopalakrishnan Partha
vgopala@iu.edu id:2000431429
Date: 11th Jan, 2019
"""

import librosa
import numpy
import struct
import socket
import sys

if(len(sys.argv) == 3):
    port = int(sys.argv[1])
    filename = sys.argv[2]
else:
    print("Syntax: client.py <port> <filename>")    

server_address = ('iot.lukefahr.org', port)
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
ys, sr = librosa.core.load(filename, mono=True, sr = 16000)

a = numpy.array(ys, dtype = numpy.float32)
b = struct.pack('<'+'f'*len(a), *a)

print("len = " + str(len(b)))
k = 0
#Dividing into chunks for transfer
for i in range(0, len(b), 512):
    c = b[i:i+512]
    s.sendto(c, server_address)
    k += 1
s.sendto(b'Done', server_address)    
print("Transfer Done.")
s.close()
    
