#!/usr/bin/env python3

################################################
#
#  This file will generate a WAV file 
#  consisting of a cosign wave of the given
#  frequency, sampling rate, and duration
#
################################################

import argparse as ap
import librosa
import numpy as np

parser = ap.ArgumentParser( description='C++ Wave Generator')
parser.add_argument( '-sr', '--sampling_rate', type=float, default=16000,
                        help='Sampling Rate')
parser.add_argument( '-f', '--frequency', type=float, default=440,
                        help='Wave Frequency')
parser.add_argument( '-d', '--duration', type=float, default=1.0,
                        help='Wave Duration (seconds)')
parser.add_argument( '-w', '--wav', type=str, default='wave.wav',
                        help='Output WAV file') 

args = parser.parse_args()

sr = int(args.sampling_rate)
freq = args.frequency 
t = args.duration
wavfile = args.wav 

#generate the wave data
print ('Generating wave data')
xs = np.arange(t * sr) / sr
ys = np.cos(2 * np.pi * freq * xs)

# write the wav file out
print ('Writing WAV to: ' + str(wavfile))
try:
    librosa.output.write( wavfile, ys, sr )
except AttributeError:    
    librosa.output.write_wav( wavfile, ys, sr )


