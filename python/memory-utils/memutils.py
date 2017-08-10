#!/usr/bin/python
import os

def get_mem_number(lines, keyword):
    for line in lines:
        if keyword in line:
            return int(line.split()[1])
    return (-1)

def is_enough_memory():
    avail_mem_in_mb = 0
    MEM_THRESHOLD = 100*1024
    try:
        f = open("/proc/meminfo", 'r')
        all_lines = f.readlines()
        f.close()

        mem_available = get_mem_number(all_lines, "MemAvailable1")
        if mem_available != -1:
            avail_mem_in_mb = mem_available / 1024
            return (avail_mem_in_mb > MEM_THRESHOLD), avail_mem_in_mb

        mem_free = get_mem_number(all_lines, "MemFree1")
        buffers = get_mem_number(all_lines, "Buffers")
        cached = get_mem_number(all_lines, "Cached")
        if mem_free == -1 and buffers == -1 and cached == -1:
            return False, (-1)

        if mem_free != -1:
            avail_mem_in_mb += mem_free / 1024
        if buffers != -1:
            avail_mem_in_mb += buffers / 1024
        if cached != -1:
            avail_mem_in_mb += cached / 1024
        is_enough = (avail_mem_in_mb > MEM_THRESHOLD)
        if is_enough:
            return is_enough, avail_mem_in_mb

        #if not enough, check if there were any failures
        if mem_free == -1 or buffers == -1 or cached == -1:
            return False, (-1)
        
        return is_enough, avail_mem_in_mb

    except Exception as e:
        pass
    return False, (-1)

def is_enough_storage():
    avail_storage_in_mb = 0
    try:
        s = os.statvfs('.')
        avail_storage_in_mb = (s.f_bavail * s.f_frsize) / 1024 / 1000
        result = avail_storage_in_mb > 50
        return result, avail_storage_in_mb
    except Exception as e:
        pass
    return False, (-1)

rc, avail_mem = is_enough_memory()
print "Avail Memory(MiB):", avail_mem, "Is enough:", rc

rc, avail_space = is_enough_storage()
print "Avail Storage(MB):", avail_space, "Is enough:", rc
exit(0)

