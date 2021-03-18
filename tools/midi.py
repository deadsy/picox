#!/usr/bin/env python3
"""
Generate lookup tables.
"""

import math
import sys

FullCycle = 1 << 32
AudioSampleFrequency = 48000
FrequencyScale = float(FullCycle) / float(AudioSampleFrequency)

def midi_to_frequency(note):
	return 440.0 * math.pow(2.0, (note - 69.0) * (1.0 / 12.0))

def m2f_func(i, n):
  f = midi_to_frequency(i)
  return int(f * FrequencyScale)

def gen_lut(name, bits, func):
  """output a table with y values"""
  print('#define %s_BITS (%dU)' % (name, bits))
  print('#define %s_SIZE (1U << %s_BITS)' % (name, name))
  print('static const uint32_t %s_data[%s_SIZE + 1] = {' % (name, name))
  n = 1 << bits;
  for i in range(n + 1):
    if i == 0:
      sys.stdout.write('\t')
    if i != 0 and i % 8 == 0:
      sys.stdout.write('\r\n\t')
    sys.stdout.write('0x%08x, ' % func(i))
  sys.stdout.write('\r\n};\r\n')

def main():
  n = 7
  gen_lut("M2F_LUT", n, lambda i: m2f_func(i, n))


main()


