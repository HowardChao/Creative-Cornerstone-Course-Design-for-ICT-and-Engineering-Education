L, R, C = 1, 1, 1

while True:
	L, R, C = [ int(x) for x in input().split() ]
	if L == 0 and R == 0 and C == 0: break

	Sl, Sr, Sc = [ int(x) for x in input().split() ]
	El, Er, Ec = [ int(x) for x in input().split() ]

	maze = []
	for l in range(L):
		level = []
		for r in range(R):
			row = list(input())
			level.append(row)
		maze.append(level)

	explored = [[[False] * C] * R] * L

	steps = 0
	found = False
	queue = [(Sl-1,Sr-1,Sc-1)]
	q_len = 0
	while True:
		if len(queue) == q_len: break

		for nd in range(q_len, len(queue)):
			l, r, c = queue[nd][0], queue[nd][1], queue[nd][2]
			if l == El-1 and r == Er-1 and c == Ec-1:
				found = True
				break

			# up
			if l + 1 < L and not explored[l+1][r][c]:
				explored[l+1][r][c] = True
				queue.append((l+1, r, c))

			# down
			if l - 1 >= 0 and not explored[l-1][r][c]:
				explored[l-1][r][c] = True
				queue.append((l-1, r, c))

			# front
			if r + 1 < R and not explored[l][r+1][c]:
				explored[l][r+1][c] = True
				queue.append((l, r+1, c))

			# back
			if r - 1 >= 0 and not explored[l][r-1][c]:
				explored[l][r-1][c] = True
				queue.append((l, r-1, c))

			# right
			if c + 1 < C and not explored[l][r][c+1]:
				explored[l][r][c+1] = True
				queue.append((l, r, c+1))

			# left
			if c - 1 >= 0 and not explored[l][r][c-1]:
				explored[l][r][c-1] = True
				queue.append((l, r, c-1))

		q_len = len(queue)
		if found == True: break
		steps += 1

	if found: print('Escaped in {} minute(s).'.format(steps))
	else: print('Trapped!')

