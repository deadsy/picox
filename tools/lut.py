#!/usr/bin/env python3
"""
Generate lookup tables.
"""

import math
import sys

def gen_ydy_table(name, bits, func):
  """output a table with y and dy values"""
  n = 1 << bits
  print('/* generated by ./tools/lut.py */')
  print('#define %s_BITS (%dU)' % (name, bits))
  print('#define %s_SIZE (1U << %s_BITS)' % (name, name))
  print('static const int32_t %s_data[%s_SIZE << 1] = {' % (name, name))
  for i in range(n):
    if i == 0:
      sys.stdout.write('\t')
    if i != 0 and i % 4 == 0:
      sys.stdout.write('\r\n\t')
    (y, dy) = func(i)
    sys.stdout.write('%s, %s, ' % (y, dy))
  sys.stdout.write('\r\n};\r\n')

def q_to_str(x):
  return '%d' % x

max_df = 0
min_df = 0

def dy_frac(n, dy):
  global max_df, min_df
  frac = (1 << n) - 1
  max_df = max(max_df, dy * frac)
  min_df = min(min_df, dy * frac)

def scale_bits():
  n = math.log(float(max_df - min_df), 2.0)
  n = math.ceil(n) - 32
  print("scale bits = %d" % int(n))

max_s32 = (1 << 31) - 1
min_s32 = -(1 << 31)

def float_to_q31(x):
  xi = int(x * float(1 << 31))
  if xi > max_s32:
    return max_s32
  if xi < min_s32:
    return min_s32
  return xi

def float_to_q17(x):
  xi = int(x * float(1 << 17))
  if xi > max_s32:
    return max_s32
  if xi < min_s32:
    return min_s32
  return xi

def cos_func(i, n):
  k = float(1 << n)
  y0 = math.cos(float(i) * 2.0 * math.pi / k)
  y1 = math.cos(float(i+1) * 2.0 * math.pi / k)
  i0 = float_to_q31(y0)
  i1 = float_to_q31(y1)
  dy_frac(25, i1 - i0)
  y = q_to_str(i0)
  dy = q_to_str(i1 - i0)
  return (y, dy)

def midi_to_frequency(note):
	return 440.0 * math.pow(2.0, (note - 69.0) * (1.0 / 12.0))

def m2f_func(i, n):
  y0 = midi_to_frequency(float(i))
  y1 = midi_to_frequency(float(i + 1))
  i0 = float_to_q17(y0)
  i1 = float_to_q17(y1)
  dy_frac(24, i1 - i0)
  y = q_to_str(i0)
  dy = q_to_str(i1 - i0)
  return (y, dy)

def main():

  global min_df, max_df

  min_df = 0
  max_df = 0
  n = 7
  gen_ydy_table('COS_LUT', n, lambda i: cos_func(i, n))
  scale_bits()

  min_df = 0
  max_df = 0
  n = 7
  gen_ydy_table('M2F_LUT', n, lambda i: m2f_func(i, n))
  scale_bits()

main()