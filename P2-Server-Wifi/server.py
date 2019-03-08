import librosa

import socket

import struct

import numpy as np

import sys



port = int(sys.argv[1])

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

s.bind(('0.0.0.0', port))

arr = []

raw, addr = s.recvfrom(64000)

a2 = struct.unpack('<'+'f'*int(len(raw)/4), raw)

a2 = np.array(a2, dtype = np.float)

print(a2)

librosa.output.write_wav("elephant2.wav", a2 , 16000)
