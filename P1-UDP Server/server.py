# -*- coding: utf-8 -*-
"""
P1-UDP Server
Created by: Tejesh Kalicharan, Nimmagadda
tnimmag@iu.edu id:2000432930
Date: 11th Jan, 2019
"""
import librosa
import socket 
import struct 
import numpy as np
import sys

port = int(sys.argv[1])

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
s.bind(('0.0.0.0', port))

arr = []
#Recieve chunks of Packets
while(True):
    raw, addr = s.recvfrom(2048)
    raw1 = raw.decode("utf-8", errors = "ignore")
  
    if raw1 == "Done":
        print("File Received.")
        break
    #Append Each chunk recieved
    arr.append(raw)
    
#Collage all chunks into a single chunk
a1 = arr[0]
for i in range(1, len(arr)):
    a1 = a1 + arr[i]

a2 = struct.unpack('<'+'f'*int(len(a1)/4), a1)
    
a2 = np.array(a2, dtype = float32)
#From array to wav file
librosa.output.write_wav("elephant2.wav", a2 , 16000)
