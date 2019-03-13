#!/bin/env python3
from random import seed,sample,randint
seed(10)
for x in range(1, 4):
	f=open("test"+str(x)+".txt", "w")
	nums=sample(range(-99999,99999),randint(10,50))
	[f.write(str(x)+"\n") for x in nums]
	f.close()
