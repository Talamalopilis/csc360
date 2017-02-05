import urllib2
import thread
import time
import random

NUM_CLIENTS = 10

HOST = "localhost"
PORT = "15078"

ADD = "http://{0}:{1}/?op=add&val=".format(HOST, PORT)
REMOVE = "http://{0}:{1}/?op=remove".format(HOST, PORT)
SUM = "http://{0}:{1}/?op=sum&val=".format(HOST, PORT)
OP_MAP = {1: ADD, 2: REMOVE, 3: SUM}

CHANCE_ADD = 40
CHANCE_REMOVE = 40
CHANCE_SUM = 20

POWER_BALL = [1 for i in xrange(CHANCE_ADD)] + \
[2 for i in xrange(CHANCE_REMOVE)] + \
[3 for i in xrange(CHANCE_SUM)]
ADD_RANGE = range(0,20)
SUM_RANGE = range(0,5)

def spin_ball(op = None):
	if not op:
		op = random.choice(POWER_BALL)
	if op == 1:
		value = str(random.choice(ADD_RANGE))
	elif op == 3:
		value = str(random.choice(SUM_RANGE))
	else:
		value =  ""
	return (op, value)

def client(default_op = None):
	while(True):
		op, value = spin_ball(default_op)
		url = OP_MAP[op] + value
		#print url
		start = time.time()
		content = urllib2.urlopen(url).read()
		print "{}client waited: {}".format(content, time.time() - start)
		time.sleep(random.randint(1, 3))

thread.start_new_thread(client, (1,)) 
thread.start_new_thread(client, (2,)) 
#must have at least one add and 
# one remove thread to avoid deadlock
for i in xrange(NUM_CLIENTS - 2):
	thread.start_new_thread(client, ())
	time.sleep(2)

while 1:
	pass
