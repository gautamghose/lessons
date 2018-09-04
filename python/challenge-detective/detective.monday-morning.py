#!/usr/bin/python
import sys
import json

graph = [] #A vector of vectors. Graph represented as adjacency list
in_degrees = [] #A vector of integers. in_degrees[i] carries the number of edges coming in to vertex i
vertex_map = {} #A dictionary of <string, int>. Node names mapped to node id's which are generated on the fly
vertex_list = [] #Vector of strings, each string representing a unique node name.

completed_lists = [] #A vector of vectors. Will contain the final possible output lists
curated_completed_lists = [] #A vector of vectors. Will contain the final output lists
partial_lists = [] #A vector of vectors. Will store the partial lists which need to be post processed to generate full ones.

def build_graph():
    """Reads the input file and builds the graph.
    The input file has the witness account arrays in json format.
    Args:
        None
    Returns:
        None. Only builds the global graph.
    """
    global graph
    global in_degrees
    global vertex_map
    global vertex_list

    with open(sys.argv[1]) as f:
        data = json.loads(f.read())
        #debug print data

    for lst in data:
        lstmod = [x.strip() for x in lst if len(x.strip()) > 0]
        idx = -1
        for v_str_u in lst:
            idx += 1
            v_str = str(v_str_u)
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
    """Recursively builds the curr_list. Adds to the global completed_list or partial_list as appropriate
    Args:
        curr_list: a vector of integers representing a partial list of vertices built so far.
        start: an integer, representing the next vertex to explore.
    Returns:
        Nothing. Populates the global variables completed_lists and partial_lists. Also updates in_degrees vector.
    """
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
        #debug print "Reduced in_degrees for", dest, "to", in_degrees[dest], "from", start
        if in_degrees[dest] < 0:
            #debug print "dest", dest, "start", start, "curr_list", curr_list
            assert in_degrees[dest] >= 0
        if in_degrees[dest] == 0:
            found_forward_path = True
            build_list(list(curr_list), dest)
    #if a forward path is not found, just store this partially created list for post processing.
    if not found_forward_path:
        partial_lists.append(curr_list)
    return

def build_list_starter():
    """Top level invocation of the core build_list() recursive method"""
    idx = 0
    start_with = []
    while idx < len(in_degrees):
        if in_degrees[idx] == 0:
            start_with.append(idx)
        idx += 1
    for start_v in start_with:
        build_list([], start_v)
    return

def postprocess_partial_lists():
    """Look at the partial lists created so far and complete them by consulting the completed lists.
    Partial lists will be promoted to completed_lists as appropriate.
    """
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
    return

def postprocess_complete_lists():
    """From all the completed lists, select the longest ones and covering all nodes"""
    global graph
    global completed_lists
    global curated_completed_lists

    #sort the lists by length
    completed_lists.sort(key=lambda x: len(x), reverse=True) #sort by the length
    curated_completed_lists = []
    covered = [False for x in range(0, len(graph))]
    for lst in completed_lists:
        include_this_list = False
        for v in lst:
            if not covered[v]:
                covered[v] = True
                include_this_list = True
        #if include_this_list:
        if True:
            curated_completed_lists.append(lst)
    return

def print_graph():
    """Debug helper routine to print the global graph"""
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
    #debug print_graph()
    build_list_starter()
    postprocess_partial_lists()

    postprocess_complete_lists()

    json_output = []
    #debug print "completed"
    print "curated-completed"
    for lst in curated_completed_lists:
        json_lst = []
        for v in lst:
            json_lst.append(vertex_list[v])
            #debug print vertex_list[v],
            print vertex_list[v],
        #debug print
        print
        json_output.append(json_lst)

    print "partial"
    for lst in partial_lists:
        for v in lst:
            print vertex_list[v],
        print

    print "answer"
    print json.dumps(json_output)
    return 0


if __name__ == "__main__":
  rc = main()
  sys.exit(rc)

