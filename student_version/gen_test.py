from random import randint, random

test_cases = 1
maze_size = 30

for i in range(test_cases):

	L = randint(1, maze_size)
	R = randint(1, maze_size)
	C = randint(1, maze_size)
	print('{} {} {}'.format(L, R, C))

	Sl = randint(1, L)
	Sr = randint(1, R)
	Sc = randint(1, C)

	El = randint(1, L)
	Er = randint(1, R)
	Ec = randint(1, C)

	while Sl == El and Sr == Er and Sc == Ec:
		El = randint(1, L)
		Er = randint(1, R)
		Ec = randint(1, C)

	print('{} {} {}'.format(Sl, Sr, Sc))
	print('{} {} {}'.format(El, Er, Ec))

	for l in range(1, L+1):
		for r in range(1, R+1):
			row = ''
			for c in range(1, C+1):
				if   l == Sl and r == Sr and c == Sc: row += 'S'
				elif l == El and r == Er and c == Ec: row += 'E'
				else: row += '.' if random() > 0.4 else '#'
			print(row)
		# print()

print('0 0 0')