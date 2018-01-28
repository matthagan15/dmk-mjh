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


# plt.figure()
# img = plt.imshow(pdists[0], cmap='hot', interpolation='nearest')
# img.norm.vmin = 0
# img.norm.vmax = 1
# for pd in pdists:
#     img.set_data(pd)
#     plt.draw()
#     plt.pause(0.1)

filename = 'robot.txt'
file_robot = open(filename,'r')

detections = []
data = []
wall_grids = []
robot_grids = []
wall_grid = []
robot_grid = []
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
        detections.append([position[0]+x,position[1]+y])
        continue
    if mode == 'wall_dist':
        wall_grid.append([float(w) for w in line.split(',')])
        continue
    if mode == 'robot_dist':
        robot_grid.append([float(w) for w in line.split(',')])
        continue
    if mode == 'end':
        data.append([position,detections])
        detections = []
        wall_grids.append(wall_grid)
        wall_grid = []
        robot_grids.append(robot_grid)
        robot_grid = []
        continue

def radToDeg(x):
    return x*180.0/math.pi


obstacles = []
for verts in list_obs:
    polygon = patches.Polygon(verts, True)
    obstacles.append(polygon)

fig = plt.figure(figsize=(6,6))
ax1 = fig.add_subplot(1, 1, 1)
# ax2 = fig.add_subplot(1, 2, 2)
# ax3 = fig.add_subplot(2, 2, 4)
p = PatchCollection(obstacles, alpha=0.1,color='black')
ax1.add_collection(p)
# plt.xlim(0,10)
# plt.ylim(0,10)
ax1.yaxis.set_visible(False)
ax1.xaxis.set_visible(False)
ax1.set_aspect('equal', 'datalim')

# robot, = ax1.plot([],[],'bo')
pings, = ax1.plot([],[],'r.')
wallDist = ax1.imshow(wall_grids[0], cmap='binary',aspect='equal',extent=(0,10,0,10),alpha=0.7)
wallDist.norm.vmin = 0
wallDist.norm.vmax = 1
robotDist = ax1.imshow(robot_grids[0], cmap='Reds',aspect='equal',extent=(0,10,0,10),alpha=0.5)
robotDist.norm.vmin = 0
robotDist.norm.vmax = 1


for i in range(len(data)):
    #unpack
    x,y,th = data[i][0]
    det = data[i][1]

    # robot.set_data(x,y)
    if len(det) != 0:
        xdet,ydet = zip(*det)
        pings.set_data(xdet,ydet)
    else:
        pings.set_data([],[])
    if i != 0:
        scanner.remove()
    scanner = patches.Wedge((x, y),scan_pow, 0, 360,alpha=0.3,color='red')
    ax1.add_patch(scanner)
    wallDist.set_data(wall_grids[i])
    robotDist.set_data(robot_grids[i])
    plt.draw()
    plt.pause(0.3)
plt.show()
