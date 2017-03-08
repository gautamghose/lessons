#!/usr/bin/python

import brotli
import sys

if len(sys.argv) < 2:
    print "Usage:", sys.argv[0], "<input-filename>", "[<output-filename>]"
    sys.exit(1)

infile = open(sys.argv[1], 'rb')
if len(sys.argv) < 3:
    outfname = "" + sys.argv[1] + ".broz"
else:
    outfname = sys.argv[2]
outfile = open(outfname, 'wb')

chunksize = 1024*1024
inlen = 0
outlen = 0
compressor = brotli.Compressor(quality=1, lgwin=19)

bs = infile.read(chunksize)
inlen += len(bs)
while bs != "":
    wbs = compressor.compress(bs)
    outfile.write(wbs)
    outlen += len(wbs)
    bs = infile.read(chunksize)
    inlen += len(bs)

infile.close()
wbs = compressor.finish()
outfile.write(wbs)
outlen += len(wbs)
outfile.close()
print "Summary:", inlen, "bytes got reduced to", outlen, "bytes for", "{0:.3f}x".format(float(inlen)/float(outlen)), "reduction."

