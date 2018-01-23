import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.collections import PatchCollection
from matplotlib.path import Path
import matplotlib.pyplot as plt
import string
import math
import numpy as np

filename = 'World.txt'
file_world = open(filename,'r')
list_obs = []

first = True
for line in file_world:	# each line represents a series of x,y points in a path from two nodes
    line = line[:-2]    # remove end line symbol
    if first:
        x_span,y_span = [float(w) for w in line.split(',')]
        first = False
        continue
    strings = [w[1:].split(',') for w in line.split(')')]
    obs = []
    for s in strings:
        numbers = [float(a) for a in s]
        obs.append(numbers)
    list_obs.append(obs)

filename = 'robot.txt'
file_robot = open(filename,'r')


detections = []
data = []
for line in file_robot:
    line = line[:-1]
    if line[0].isalpha():
        mode = line
        if mode != "end":
            continue
    if mode == 'scanner':
        scan_pow,scan_width = [float(w) for w in line.split(',')]
        continue
    if mode == 'position':
        position = [float(w) for w in line.split(',')]
        continue
    if mode == 'detections':
        x,y = [float(w) for w in line.split(',')]
        detections.append([x,y])
        continue
    if mode == 'end':
        data.append([position,detections])
        detections = []
        continue

def radToDeg(x):
    return x*180.0/math.pi

fig, ax = plt.subplots()
obstacles = []
for verts in list_obs:
    polygon = patches.Polygon(verts, True)
    obstacles.append(polygon)
p = PatchCollection(obstacles, alpha=1.0,color='black')
ax.add_collection(p)
plt.xlim(0,10)
plt.ylim(0,10)
ax.yaxis.set_visible(False)
ax.xaxis.set_visible(False)

robot, = plt.plot([],[],'b^')
pings, = plt.plot([],[],'r.')

for i in range(len(data)):
    #unpack
    x,y,th = data[i][0]
    det = data[i][1]

    robot.set_data(x,y)
    if len(det) != 0:
        xdet,ydet = zip(*det)
        pings.set_data(xdet,ydet)
    else:
        pings.set_data([],[])
    th1 = radToDeg(th-scan_width)
    th2 = radToDeg(th+scan_width)
    if i != 0:
        scanner.remove()
    scanner = patches.Wedge((x, y),scan_pow, th1, th2,alpha=0.3,color='red')
    ax.add_patch(scanner)
    plt.draw()
    plt.pause(3)
plt.show()
