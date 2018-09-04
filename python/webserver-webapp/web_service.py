#!/usr/bin/python

from wsgiref.util import setup_testing_defaults
from wsgiref.simple_server import make_server, WSGIServer
from SocketServer import ThreadingMixIn

from time import sleep
import threading
import json
import urllib
import unicodedata

def get_random_name():
    """Gets a random name from the known website.
    Args: None
    Returns:
        a (string, string) tuple with first-name and last-name. The strings are in expanded unicode.
    """
    url = "http://uinames.com/api/"
    uh = urllib.urlopen(url)
    data = uh.read() #This will be a JSON string
    print "DATA:", data
    info = json.loads(data)
    return (info['name'], info['surname'])


def replace_pattern(input_str, pattern, replace_by):
    """Replace the 'pattern' in input_str with 'replace_by'  """
    print "Replacing", pattern, "by", replace_by, "in", input_str
    return input_str + ":" + replace_by

def get_random_joke(firstname, lastname):
    """Gets a random joke and personalizes it with the name supplied.
    Args:
        firstname: a string
        lastname: a string
    Returns:
        a string with personalization
    """
    url = "http://api.icndb.com/jokes/random?firstName={0}&lastName={1}&limitTo=\[nerdy\]".format("first--name", "last--name")
    try:
        uh = urllib.urlopen(url)
    except:
        print "ERROR in urlopen:", firstname, lastname
        raise
    data = uh.read()
    info = json.loads(data)
    joke_str = info[u'value'][u'joke']

    joke_str = replace_pattern(joke_str, "first--name", firstname)
    joke_str = replace_pattern(joke_str, "last--name", lastname)
    #joke_str = unicodedata.normalize('NFKD', info[u'value'][u'joke']).encode('ascii','ignore') 
    #return info[u'value'][u'joke']
    return joke_str

class UnderlyingServerError(RuntimeError):
    pass

def get_formatted_random_joke():
    """Main routine that does the heavy lifting of generating the random jokes.
    Args:
        None
    Returns:
        a string, with the random joke personalized by random name
    """
    try:
        firstname, lastname = get_random_name()
    except:
        raise UnderlyingServerError('Could not get random name from server')
    return firstname

    try:
        joke_str = get_random_joke(firstname, lastname)
    except:
        print "BAD NAME:", firstname, lastname, "END"
        raise UnderlyingServerError('Could not get random joke from server for: {0} {1}'.format(firstname,lastname))

    #joke_str = unicodedata.normalize('NFKD', joke_str).encode('ascii','ignore') 
    return joke_str

# A relatively simple WSGI application. It's going to print out the
# environment dictionary after being updated by setup_testing_defaults
def simple_app(environ, start_response):
    status = '200 OK'
    headers = [('Content-type', 'text/plain; charset=utf-16')]

    start_response(status, headers)

    try:
        joke_str = get_formatted_random_joke()
    except UnderlyingServerError as err:
        joke_str = "Service Error: {0}".format(err)

    print "TYPE of joke_str:", type(joke_str)
    ret = joke_str.encode('utf16')
    #ret = "{0}\n".format(unicodedata.normalize('NFKD', joke_str).encode('ascii', 'ignore'))
    return ret


class ThreadedWSGIServer(ThreadingMixIn, WSGIServer): 
    pass


httpd = make_server('', 5000, simple_app, ThreadedWSGIServer)
print ("Service on port 5000")
httpd.serve_forever()


'''
with make_server('', 8000, simple_app) as httpd:
    print("Serving on port 8000...")
    httpd.serve_forever()
'''

