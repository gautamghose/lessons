#!/usr/bin/python

import subprocess
import sys
import os.path
import random

if len(sys.argv) < 2:
    print "Usage:",sys.argv[0], "filename-with-weighted-edges" 
    sys.exit(1)

FILENAME = sys.argv[1]

X = {} # set of vertices whose minimum distances are already computed
edges = [] # list of edges, where each edge is a tuple (v1, v2, w) where v1,v2 are source and target vertices and w is the non-negative weight

maxv = 0
f = open(FILENAME)
for line in iter(f):
    vertex_line = line.strip().split() # split by space/whitespace
    v1 = int(vertex_line[0])
    if v1 > maxv:
        maxv = v1
    i = 0
    for pair in vertex_line:
        if i == 0:
            i += 1
            continue
        e = pair.split(',')
        assert(len(e) == 2)
        v2 = int(e[0])
        w = int(e[1])
        edges.append( (v1, v2, w) )
        i += 1
f.close()


sorted_edges = sorted(edges, key = lambda x: x[2])

print "Maxm node", maxv, "Number of edges:", len(sorted_edges)

def find_min_edge():
    global sorted_edges
    global X

    min_score = 5000000
    min_idx = -1
    idx = 0
    for e in sorted_edges:
        v1 = e[0]
        v2 = e[1]
        w = e[2]
        if v1 in X and v2 not in X:
            score = X[v1] + w
            if score < min_score:
                min_score = score
                min_idx = idx
        idx += 1

    assert(min_idx != -1)
    e = sorted_edges.pop(min_idx)
    return e[0], e[1], e[2]
            
    
def dijkstra_main_loop(start):
    global X
    global edges
    global maxv #max vertex, also implies the number of vertices

    nr_vertices = maxv
    X[start] = 0

    while len(X) < nr_vertices:
        v1, v2, w = find_min_edge()
        assert(X.has_key(v1) == True)
        assert(X.has_key(v2) == False)
        X[v2] = X[v1] + w
        print "Computed shortes distance for vertex", v2, X[v2]
        
dijkstra_main_loop(1)

out_str = "Output Needed: "
targets = [7,37,59,82,99,115,133,165,188,197]
for t in targets:
    if X.has_key(t):
        out_str += str(X[t]) + ","
    else:
        out_str += "NA,"
print out_str
sys.exit(0)


