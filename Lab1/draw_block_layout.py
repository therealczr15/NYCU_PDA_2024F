import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import math
import time

import matplotlib.colors as mcolors

def fix_color(color):
    return mcolors.to_hex(color)

def draw_block(ax, x, y, width, height, id):
    if id > 0:
        color = fix_color("#FFCCCC") 
    else:
        color = fix_color("#BBBBBB")

    ax.add_patch(
        patches.Rectangle( 
            (x, y),
            width,
            height,
            fill=True,
            edgecolor=fix_color("#000000"),
            facecolor=color,
            alpha=1.0
        )
    )


png_size = (16,12)
txt_name = sys.argv[1] 
png_name = sys.argv[2].strip() 
fread = open(txt_name,'r')
f = fread.read().split("\n")


total_block_number = int(f[0])
window_width = int( f[1].split(" ")[0] )
window_height = int( f[1].split(" ")[1] )

fig = plt.figure(figsize=png_size)

ax = fig.add_subplot(111)
ax.set_xbound(0,window_width)
ax.set_ybound(0,window_height)


i = 2

for block in range(total_block_number ):
    ss =  f[i].split(" ")
    draw_block(ax,int(ss[1]),int(ss[2]),int(ss[3]),int(ss[4]),int(ss[0]))
    i+=1

#plt.savefig(str(sys.argv[1])[:-4]+".png")

plt.savefig(png_name)
