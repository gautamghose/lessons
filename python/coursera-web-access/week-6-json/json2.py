import json
import urllib

'''
Parsing a JSON string like this:-
{
  comments: [
    {
      name: "Matthias"
      count: 97
    },
    {
      name: "Geomer"
      count: 97
    }
    ...
  ]
}
'''

while True:
    address = raw_input('Enter location: ')
    if len(address) < 1 : break

    url = address
    print 'Retrieving', url
    uh = urllib.urlopen(url)
    data = uh.read() #This will be a JSON string as shown above
    print 'Retrieved',len(data),'characters'
    #print data
    info = json.loads(data)
    print "Len", len(info['comments'])
    total_comments = 0
    for commenter in info['comments']:
        total_comments += int(commenter['count'])
    print "Sum:", total_comments


input = '''
[
  { "id" : "001",
    "x" : "2",
    "name" : "Chuck"
  } ,
  { "id" : "009",
    "x" : "7",
    "name" : "Chuck"
  } 
]'''

'''
info = json.loads(input)
print 'User count:', len(info)

for item in info:
    print 'Name', item['name']
    print 'Id', item['id']
    print 'Attribute', item['x']
'''

