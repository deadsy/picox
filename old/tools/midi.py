#!/usr/bin/env python3
"""
Generate MIDI constants
"""

import math

max_s32 = (1 << 31) - 1
min_s32 = -(1 << 31)

def float_to_q24(x):
  xi = int(x * float(1 << 24))
  if xi > max_s32:
    return max_s32
  if xi < min_s32:
    return min_s32
  return xi

FullCycle = 1 << 32
AudioSampleFrequency = 48000
FrequencyScale = float(FullCycle) / float(AudioSampleFrequency)

k0 = math.log(FrequencyScale, 2.0) + math.log(440.0, 2.0) - (69.0/12.0)
k1 = 1.0/12.0

def main():

  print("#define K0 %d" % float_to_q24(k0))
  print("#define K1 %d" % float_to_q24(k1))


main()


