#!/usr/bin/python
import sys

graph = [] #A vector of vectors. Graph represented as adjacency list
in_degrees = [] #A vector of integers. in_degrees[i] carries the number of edges coming in to vertex i
completed_lists = [] #Avector of vectors. Will contain the final output lists
partial_lists = [] #A vector of vectors. Will store the partial lists which need to be post processed to generate full ones.
vertex_map = {} #A dictionary of <string, int>. Node names mapped to node id's which are generated on the fly
vertex_list = [] #Vector of strings, each string representing a unique node name.

def build_graph():
    """Reads the input file and builds the graph"""
    global graph
    global in_degrees
    global out_degrees
    global vertex_map
    global vertex_list

    with open(sys.argv[1]) as f:
        for line in f:
            lst = filter(lambda ch: ch not in "[]\"", line).split(',')
            lst = [x.strip() for x in lst if len(x.strip()) > 0]
            idx = -1
            for v_str in lst:
                idx += 1
                if v_str not in vertex_map:
                    v_id = len(vertex_list)
                    vertex_map[v_str] = v_id
                    vertex_list.append(v_str)
                    graph.append([])
                    in_degrees.append(0)
                if idx == 0:
                    continue
                #Put an edge from (idx-1) to (idx)
                src = vertex_map[lst[idx-1]]
                dest = vertex_map[lst[idx]]
                assert v_str == lst[idx]
                graph[src].append(dest)
                in_degrees[dest] += 1

    assert len(graph) == len(in_degrees)
    assert len(graph) == len(vertex_list)
    assert len(vertex_map) == len(vertex_list)
    return
                

def build_list(curr_list, start):
    """Recursively builds the curr_list. Adds to the global completed_list or partial_list as appropriate"""
    global graph
    global in_degrees
    global completed_lists
    global partial_lists

    assert in_degrees[start] == 0
    curr_list.append(start)
    if len(graph[start]) == 0:
        completed_lists.append(curr_list)
        return
    found_forward_path = False
    for dest in graph[start]:
        in_degrees[dest] -= 1
        print "Reduced in_degrees for", dest, "to", in_degrees[dest], "from", start
        if in_degrees[dest] < 0:
            print "dest", dest, "start", start, "curr_list", curr_list
            assert in_degrees[dest] >= 0
        if in_degrees[dest] == 0:
            found_forward_path = True
            build_list(list(curr_list), dest)
    if not found_forward_path:
        partial_lists.append(curr_list)
    return

def build_list_starter():
    idx = 0
    start_with = []
    while idx < len(in_degrees):
        if in_degrees[idx] == 0:
            start_with.append(idx)
        idx += 1
    for start_v in start_with:
        build_list([], start_v)
    pass

def postprocess_partial_lists():
    global graph
    global completed_lists
    global partial_lists

    newly_completed_lists = []
    for lst in partial_lists:
        last_v = lst[-1]
        #Find last_v's next in all completed_lists and complete this partial list
        for d in graph[last_v]:
            for c_list in completed_lists:
                idx = 0
                for v in c_list:
                    if v == d:
                        break
                    idx += 1
                if idx < len(c_list):
                    #append c_lst[idx:] to lst, and then append to completed_lists
                    new_completed_lst = list(lst)
                    new_completed_lst += c_list[idx:]
                    newly_completed_lists.append(new_completed_lst)

    for lst in newly_completed_lists:
        completed_lists.append(lst)

    pass

def print_graph():
    global graph
    global in_degrees
    global vertex_list

    print "vertex_list", vertex_list
    idx = 0
    for g in graph:
        print "id", idx, g
        idx += 1
    print "in_degrees", in_degrees

def main():
    if len(sys.argv) < 2:
        print "Usage:", sys.argv[0], "<Filename-with-witness-accounts>"
        return 1

    build_graph()
    print_graph()
    build_list_starter()
    postprocess_partial_lists()
    print "completed"
    for lst in completed_lists:
        for v in lst:
            print vertex_list[v],
        print
    print "partial"
    for lst in partial_lists:
        for v in lst:
            print vertex_list[v],
        print
    return 0


if __name__ == "__main__":
  rc = main()
  sys.exit(rc)

