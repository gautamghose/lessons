#!/usr/bin/python

from wsgiref.util import setup_testing_defaults
from wsgiref.simple_server import make_server, WSGIServer
from SocketServer import ThreadingMixIn

from time import sleep
import threading
import json
import urllib
import unicodedata

counter = 0

def get_random_name():
    """Gets a random name from the known website.
    Args: None
    Returns:
        a (string, string) tuple with first-name and last-name
    """
    url = "http://uinames.com/api/"
    uh = urllib.urlopen(url)
    data = uh.read() #This will be a JSON string as shown above
    #print 'Retrieved',len(data),'characters'
    #print data
    info = json.loads(data)
    print info['surname']
    #return (info['name'], info['surname'])
    return ("gautam", "ghose")

def get_random_joke(firstname, lastname):
    """Gets a random joke and personalizes it with the name supplied.
    Args:
        firstname: a string
        lastname: a string
    Returns:
        a string with personalization
    """
    url = "http://api.icndb.com/jokes/random?firstName={0}&lastName={1}&limitTo=\[nerdy\]".format(firstname, lastname)
    uh = urllib.urlopen(url)
    data = uh.read()
    print data
    info = json.loads(data)
    print info[u'value']
    print info[u'value'][u'joke']

    #joke_str = info['value']['joke']
    #return info[u'value'][u'joke']
    joke_str = unicodedata.normalize('NFKD', info[u'value'][u'joke']).encode('ascii','ignore') 
    return joke_str

def replace_pattern(input_str, pattern, replace_by):
    """Replaces all occurrences of "pattern" inside the "input_str" by "replace_by" string
    """
    return input_str

# A relatively simple WSGI application. It's going to print out the
# environment dictionary after being updated by setup_testing_defaults
def simple_app(environ, start_response):
    global counter
    setup_testing_defaults(environ)

    status = '200 OK'
    headers = [('Content-type', 'text/plain; charset=utf-8')]

    start_response(status, headers)

    firstname, lastname = get_random_name()
    joke_str = get_random_joke("plain_first_name", "plain_last_name")
    print joke_str
    #Replace plain_first_name with actual firstname, and plain_last_name with actual lastname

    joke_str_mod = replace_pattern(joke_str, "plain_first_name", "FIRST_NAME")
    joke_str_mod = replace_pattern(joke_str_mod, "plain_last_name", "LAST_NAME")

    ret = [("%s: %s\n" % (key, value)).encode("utf-8")
           for key, value in environ.items()]
    ret = "{0} Hello, cruel world!\n".format(threading.current_thread().ident)
    ret = joke_str_mod + "\n"
    print "Will be sleeping now"

    if counter < 5:
        counter += 1
        sleep(50)
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

