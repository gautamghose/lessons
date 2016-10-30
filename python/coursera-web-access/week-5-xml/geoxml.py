import urllib
import xml.etree.ElementTree as ET

#Parses an xml of the following style
'''
<?xml version="1.0" encoding="UTF-8"?>
<commentinfo>
  <note>This file contains the sample data for testing</note>

  <comments>
    <comment>
       <name>Romina</name>
       <count>97</count>
    </comment>
    <comment>
       <name>Laurie</name>
       <count>97</count>
    </comment>
    <comment>
       <name>Inika</name>
       <count>2</count>
    </comment>
  </comments>
</commentinfo>
'''

while True:
    address = raw_input('Enter location: ')
    if len(address) < 1 : break

    url = address
    print 'Retrieving', url
    uh = urllib.urlopen(url)
    data = uh.read()
    print 'Retrieved',len(data),'characters'
    #print data
    tree = ET.fromstring(data)


    counts = tree.findall('.//count')
    print len(counts)
    print "Sum:", sum([int(x.text) for x in counts])
