# -*- coding: utf-8 -*-
"""
Created on Fri Jan 11 16:56:22 2019
@author: vjkri
"""
import webrtcvad
import librosa
import socket 
import struct 
import numpy as np
import sys

port = int(sys.argv[1])

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
s.bind(('0.0.0.0', port))

arr = []

#while(True):
raw, addr = s.recvfrom(32000)

a2 = struct.unpack('<'+'h'*int(len(raw)/2), raw)
#print(a2)   
a2 = np.array(a2, dtype = np.float)
for i in range(0, len(a2)):
	a2[i] = 3.3*a2[i]
	a2[i] = a2[i]/4095
#a2 = np.array(a2, dtype = np.float)
print(a2)
librosa.output.write_wav("voice18.wav", a2 , 16000)

model = webrtcvad.Vad()

model.set_mode(3)

ys, sr = librosa.core.load('voice18.wav', sr = 16000)

#print('---')
#ys = ys[16000:32000]
wav_bits = struct.pack('<' + 'f' * len(ys), *ys)
#print(len(wav_bits))
#print('[[[]]]')
window_time = 0.03

sample_per_window = int(window_time * sr)
#print(sample_per_window)
#print(len(wav_bits))
#print(len(wav_bits))
#bytes_per_sample = 2
#print(wav_bits[0:5])

count = 0
for i in range(0, len(ys), sample_per_window):

    frame = wav_bits[i * 2 : (i+sample_per_window) * 2]
    is_speech = model.is_speech(frame, sample_rate = sr)

    #print(len(wav_bits[i : i+6400]))
    #frames = np.int16(wav_bits[i : i+6400]).tobytes()
    #iss_speech = model.is_speech(frames, sample_rate = 16000)
    print(is_speech)
    if is_speech == True:
        #print(True)
        count += 1


#print(count)
if count > 10:
    print('Voiced')
else:
    print('Not voice')

