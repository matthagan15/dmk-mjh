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


fig, ax = plt.subplots()
obstacles = []
for verts in list_obs:
    polygon = patches.Polygon(verts, True)
    obstacles.append(polygon)
p = PatchCollection(obstacles, alpha=1.0,color='black')
ax.add_collection(p)

filename = 'robot.txt'
file_robot = open(filename,'r')


detections = []
for line in file_robot:
    line = line[:-1]
    if line[0].isalpha():
        mode = line
        continue
    if mode == 'scanner':
        scan_pow = float(line)
    x,y = [float(w) for w in line.split(',')]
    if mode == 'position':
        position = [x,y]
        continue
    if mode == 'detections':
        detections.append([x,y])
        continue

plt.plot(position[0],position[1],'b^')
circle = patches.Circle((position[0], position[1]), 2)
q = PatchCollection([circle], alpha=0.3,color='red')
ax.add_collection(q)

if len(detections) != 0:
    x_det,y_det = zip(*detections)
    plt.plot(x_det,y_det,'r*')

plt.xlim(0,10)
plt.ylim(0,10)
ax.yaxis.set_visible(False)
ax.xaxis.set_visible(False)
ax.set_aspect('equal', 'datalim')
plt.show()
