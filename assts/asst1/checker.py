#!/bin/env python3

import sys

x = 1
for y in range(7, 13):
	if y == 10:
		x = 1
	fa = open('answ'+str(x)+'.txt', 'r')
	fb = open('answ'+str(x+1)+'.txt', 'r')
	fc = open('sol'+str(y)+'.txt', 'w')

	sa = set([a.strip('\n') for a in fa.readlines()])
	sb = set([b.strip('\n') for b in fb.readlines()])

	if (y < 10):
		[fc.write(str(x)+'\n') for x in sa.union(sb)]
	else:
		[fc.write(str(x)+'\n') for x in sa.intersection(sb)]

	fa.close()
	fb.close()
	fc.close()

	x += 2