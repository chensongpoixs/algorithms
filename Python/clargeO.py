#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 如果 a + b + c = N 且 a ^ 2 + b ^ 2 + c ^ 2 {a, b, c 为自然数}, 如何求出所有a, b, c可能的组合?

import time

############################################ 时间复杂度称为 T ############################################

##########################################################################################################
statr_time = time.time()

for a in range(0, 1001):
	for b in range(0, 1001):
		for c in range(0, 1001):
			if a + b + c == 1000 and a**2 + b**2 == c**2:
				print("a, b, c:%d, %d, %d" % (a, b, c))

end_time = time.time()

print("1 times:%d" % (end_time - statr_time))
print("finised")

#  T = 1000 * 1000 * 1000 * 2
#  T = 2000 * 2000 * 2000 * 2
#  T = N * N * N * 2 
#  T(n) = N ^ 3 * 2  #--- > 时间复杂度 
#  T(n) = N ^ 3
##########################################################################################################

statr_time = time.time()

for a in range(1, 1001):
	for b in range(0, 1001):
		c	= 1000 - a - b   # 时间复杂度
		if a++2	+	b**2	== c**2:
			print("a, b, c:%d, %d, %d" % (a, b, c))

end_time = time.time()

print("2 times:%d" % (end_time - statr_time))
print("finised")
##########################################################################################################