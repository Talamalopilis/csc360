import urllib2
import thread
import time
import random

NUM_CLIENTS = 10

HOST = "localhost"
PORT = "15078"

ADD = "{0}:{1}/?op=add&val=".format(HOST, PORT)
REMOVE = "{0}:{1}/?op=remove".format(HOST, PORT)
SUM = "{0}:{1}/?op=sum&val=".format(HOST, PORT)
OP_MAP = {1: ADD, 2: REMOVE, 3: SUM}

CHANCE_ADD = 40
CHANCE_REMOVE = 40
CHANCE_SUM = 20

POWER_BALL = [1 for i in xrange(CHANCE_ADD)] + \
[2 for i in xrange(CHANCE_REMOVE)] + \
[3 for i in xrange(CHANCE_SUM)]
ADD_RANGE = range(0,20)
SUM_RANGE = range(0,5)

def spin_ball():
	op = random.choice(POWER_BALL)
	if op == 1:
		value = str(random.choice(ADD_RANGE))
	elif op == 3:
		value = str(random.choice(SUM_RANGE))
	else:
		value =  ""
	return (op, value)

def client():
	while(True):
		op, value = spin_ball()
		url = OP_MAP[op] + value
		print url
		start = time.time()
		content = urllib2.urlopen(url).read()
		print content
		print "client waited: {}".format(time.time() - start)
		time.sleep(random.randint(1, 3))

for i in xrange(NUM_CLIENTS):
	thread.start_new_thread(client)
