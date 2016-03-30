import random
random.seed()
N = 22
MIN = 20
MAX = 100
data = []
print(N)
for i in range(0, N):
	for j in range(0, N):
		if (i == j):
			data.append(0)
		elif (i < j):
			data.append(random.randint(MIN, MAX))
		else:
			data.append(data[j * N + i])
for i in range(0, N):
	for j in range(0, N):
		print data[i * N + j],
		print ' ',
	print '\n',