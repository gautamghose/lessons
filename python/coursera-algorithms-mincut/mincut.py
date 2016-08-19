#!/usr/bin/python

import subprocess
import sys
import os.path
import random


if len(sys.argv) < 2:
    print "Usage:",sys.argv[0], "filename-with-adjacency-list-graph-representation"
    sys.exit(1)

FILENAME = sys.argv[1]

graph = [] #will be a list of tuples (<int>, [])
i = 0
edges_int = []
f = open(FILENAME)
for line in iter(f):
    edges = line.strip().split()
    edges_int = []
    for e in edges:
        edges_int.append(int(e))
    graph.append( () )
    graph[i] = (edges_int[0], edges_int[1:])
    i += 1
f.close()
#random.seed(20)

def random_():
    return random.randint(0,1000)

def pick_random_edge():
    global graph
    i = random_()%len(graph)
    v1 = graph[i][0]
    j = random_()%len(graph[i][1])
    v2 = graph[i][1][j]
    return (v1, v2), (i,j)

while len(graph) > 2:
  #print "Number of vertices", len(graph)
  t1,t2 = pick_random_edge()

  v1 = t1[0]
  v2 = t1[1]
  if (v1 > v2):
    tmp = v1
    v1 = v2
    v2 = tmp
  #These two vertices are getting collapsed

  idx_of_v1 = -1
  idx_of_v2 = -1
  #print "Removing", (v1,v2)
  i = 0
  for g in graph:
    if g[0] == v1:
        idx_of_v1 = i
    elif g[0] == v2:
        idx_of_v2 = i
    i += 1
  assert(idx_of_v1 != -1)
  assert(idx_of_v2 != -1)
  #graph[idx_of_v1][1].remove(v2)
  #graph[idx_of_v1][1] = [x for x in graph[idx_of_v1][1] if x != v2] 
  count = graph[idx_of_v1][1].count(v2)
  assert(count >= 1)
  for i in range(count):
    graph[idx_of_v1][1].remove(v2)

  #graph[idx_of_v2][1].remove(v1)
  count = graph[idx_of_v2][1].count(v1)
  assert(count >= 1)
  for i in range(count):
    graph[idx_of_v2][1].remove(v1)

  for i in graph[idx_of_v2][1]:
    graph[idx_of_v1][1].append(i)

  for g in graph:
    if g[0] != v1 and g[0] != v2:
        for idx in range(len(g[1])):
            if g[1][idx] == v2:
                g[1][idx] = v1
  del(graph[idx_of_v2])

assert(len(graph) == 2)
assert(len(graph[0][1]) == len(graph[1][1]))
print "Min-cut", len(graph[0][1])
'''
for g in graph:
    print g, len(g[1])
'''
sys.exit(0)


