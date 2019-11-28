# -*- coding: utf-8 -*-
"""
Created on Fri Jan 11 16:56:22 2019

@author: vjkri
"""
import tensorflow as tf
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
a2 = np.array(a2, dtype = np.float)
for i in range(0, len(a2)):
	a2[i] = 3.3*a2[i]
	a2[i] = a2[i]/4095
print(a2)
librosa.output.write_wav("voice1.wav", a2 , 16000)
N, temp, threshold, argon_address, argon_port, filtered, check_ms = 10, 0, 2.69, '149.161.252.39', 8888, [], []
ys, sr = librosa.load("voice1.wav", mono=True, sr=None)
ys = ys[::3]
mfcc = librosa.feature.mfcc(ys, sr=16000)
pad_width = 11 - mfcc.shape[1]
mfcc = np.pad(mfcc, pad_width=((0, 0), (0, pad_width)), mode='constant')
mfcc = np.reshape(mfcc, (1, 20, 11, 1))
json_file = open('cnn.json', 'r')
loaded_model_json = json_file.read()
json_file.close()
loaded_model = tf.keras.models.model_from_json(loaded_model_json)
loaded_model.load_weights("cnn.h5")
prediction = (loaded_model.predict(mfcc))
if prediction[0][0] > 0.5:
    print('ON')
    s.sendto(b'Yes', (argon_address, argon_port))
elif prediction[0][0] < 0.5:
    print('OFF')
    s.sendto(b'No', (argon_address, argon_port))

