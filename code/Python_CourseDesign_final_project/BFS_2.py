def BFS_2(self, nd_from, nd_to):
    """ return a sequence of nodes of the shortest path"""
    # TODO: similar to BFS but fixed start point and end point
    ndList = []
    # what I write begin ------
    # initialize
    queue = [nd_from]  # Q: contains only start node initially
    transitionTable = dict()
    explored = set()
    # main loop
    while True:
        if not queue:  # if Q is empty
            print('End point is not found!')
            break
        u = queue.pop()
        if u is nd_to:  # check if u is the end node
            break  # return the transition table
        explored.add(u)  # add u to explored set
        for v in u.getSuccessors():  # loop over all adjacency of u
            if (v not in queue) and (v not in explored):
                queue.insert(0, v)  # add v to Q
                transitionTable[v] = u
    # from transition table (transitionTable) to answer (ndList)
    now_nd = nd_to
    ndList.insert(0, now_nd)
    while now_nd is not nd_from:
        now_nd = transitionTable[now_nd]
        ndList.insert(0, now_nd)  # insert to the front end, then no need to reverse
    # what I write end ------
    return ndList