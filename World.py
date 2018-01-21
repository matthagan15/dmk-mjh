import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.collections import PatchCollection
from matplotlib.path import Path
import matplotlib.pyplot as plt
import string
import math
import numpy as np

filename = 'World.txt'
file_tree = open(filename,'r')
list_obs = []

first = True
for line in file_tree:	# each line represents a series of x,y points in a path from two nodes
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

plt.xlim(0,10)
plt.ylim(0,10)
ax.yaxis.set_visible(False)
ax.xaxis.set_visible(False)
plt.show()
