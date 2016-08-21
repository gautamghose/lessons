#!/usr/bin/python

import subprocess
import sys
import os.path
import random

class Stack:
     def __init__(self):
         self.items = []

     def isEmpty(self):
         return self.items == []

     def push(self, item):
         self.items.append(item)

     def pop(self):
         return self.items.pop()

     def peek(self):
         return self.items[len(self.items)-1]

     def size(self):
         return len(self.items)


if len(sys.argv) < 3:
    print "Usage:",sys.argv[0], "filename-with-directed-edges" "max-vertex (vertices are from 1 through max-vertex)"
    sys.exit(1)

FILENAME = sys.argv[1]

nr_vertices = int(sys.argv[2]) #875714 #9

#vertices range from 1 to 875714: Assignment specifies it.
#will be a list of vectors. The index in the list indicates the vertext number (starts from 1). Each element in vector represents the other end of the edge.
graph = [[] for i in range(nr_vertices+1)] 
graph_rev = [[] for i in range(nr_vertices+1)] 
m = 0
f = open(FILENAME)
for line in iter(f):
    edge = line.strip().split()
    v1 = int(edge[0])
    v2 = int(edge[1])
    assert(graph[v1].count(v2) == 0)
    graph[v1].append(v2)
    assert(graph_rev[v2].count(v1) == 0)
    graph_rev[v2].append(v1)
    m += 1
f.close()

#print graph
#print graph_rev

nr_edges = m
assert(nr_vertices == len(graph) - 1) # The vertex number '0' is not used
assert(nr_vertices == len(graph_rev) - 1) # The vertex number '0' is not used
#assert(nr_vertices == 875714)

t = 0 #global variable to be used to calculate the finishing times
explored = [False for i in range(nr_vertices+1)] #global variable to keep whether a vertex is explored or not
s = 0 #global variable to be used to keep track of the leader
leader = [0 for i in range(nr_vertices+1)] #compute leader for each vertex
finishing_times = [0 for i in range(nr_vertices+1)] #compute finishing times. 

l = sys.getrecursionlimit()
sys.setrecursionlimit(100*l)
print l

def dfs_iter(gr, start):
    global explored
    global t
    global s
    global leader
    global finishing_times

    elements = Stack()

    elements.push(start)

    while not elements.isEmpty():
        v = elements.peek()
        explored[v] = True
        leader[v] = s

        fully_explored = True
        for v2 in gr[v]:
            if not explored[v2]:
                elements.push(v2)
                fully_explored = False
                break

        if fully_explored:
            assert(elements.pop() == v)
            t += 1
            finishing_times[t] = v
    return

def dfs(gr, start):
    global explored
    global t
    global s
    global leader
    global finishing_times

    explored[start] = True
    leader[start] = s
    for v2 in gr[start]:
        if not explored[v2]:
            dfs(gr, v2)
    t += 1
    finishing_times[start] = t
    return

curr = nr_vertices
while curr >= 1:
    if not explored[curr]:
        s = curr
        dfs_iter(graph_rev, curr)
    curr -= 1

print "Done G-reverse"
print "finishing-times", finishing_times[:5]

#Main loop now
#compute the order of main loop
mainloop_order = []
while len(finishing_times) > 1:
    mainloop_order.append(finishing_times.pop())
    
print "main-loop", mainloop_order[:5]

t = 0
leader = [0 for i in range(nr_vertices+1)] #compute leader for each vertex
explored = [False for i in range(nr_vertices+1)] #global variable to keep whether a vertex is explored or not
s = 0 #global variable to be used to keep track of the leader
leader = [0 for i in range(nr_vertices+1)] #compute leader for each vertex
finishing_times = [0 for i in range(nr_vertices+1)] #compute finishing times. 

for curr in mainloop_order:
    if not explored[curr]:
        s = curr
        dfs_iter(graph, curr)

#Now compute the number of unique leaders
sccs = {} #dictionary of (leader_vertex: number-of-nodes-with-that-leader)
for v in leader:
    if v == 0:
        continue
    if not sccs.has_key(v):
        sccs[v] = 0
    sccs[v] += 1

print "Number of scc's", len(sccs)
#print sccs

#Put the tuples in the dictionary into a list of tuples so that it can be sorted
results = []
for scc_leader, scc_size in sccs.items():
    results.append( (scc_leader, scc_size) )

results_rev = sorted(results, key = lambda x: x[1])
print "results sorted"
count = 0
while count < 5 and len(results_rev)>0:
    scc = results_rev.pop()
    print scc
    count += 1

sys.exit(0)


