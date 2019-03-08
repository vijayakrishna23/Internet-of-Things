# -*- coding: utf-8 -*-
"""
Created on Thu Feb  7 12:37:28 2019
@author: Vijaya Krishna (vgopala@iu.edu id:2000431429)

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

raw, addr = s.recvfrom(64000)

a2 = struct.unpack('<'+'f'*int(len(raw)/4), raw)

a2 = np.array(a2, dtype = np.float)
librosa.output.write_wav("raw.wav", a2 , 16000)


N, temp, threshold, argon_address, argon_port, filtered, check_ms = 10, 0, 2.69, '68.51.123.193', 8888, [], []

[filtered.append(sum(a2[i-N: i])/N) for i in range(0, len(a2)-1)]

filtered = np.array(filtered, dtype = np.float)

librosa.output.write_wav("filter.wav", filtered , 16000)

[check_ms.append(filtered[i]**2) for i in range(0, len(filtered)-1)]


s.sendto(b'Yes440Hz', (argon_address, argon_port)) if (sum(check_ms)/len(check_ms)) > threshold  else s.sendto(b'No440Hz', (argon_address, argon_port))

