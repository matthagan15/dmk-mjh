import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.path import Path
import matplotlib.pyplot as plt
import string
import math
import numpy as np

filename = 'World.txt'
file_tree = open(filename,'r')
obstacles

first = True
for line in file_tree:	# each line represents a series of x,y points in a path from two nodes
    if count:
        x_span,y_span = [float(w) for w in line.split(',')]
        count = False
    print(line)
	groups = [w[:-1] for w in line.split(')')]
    print(groups)
