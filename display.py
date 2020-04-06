import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as md
import datetime as dt
import time

prefix = "u2fjv"
locations = ["d", "g", "m", "u", "6", "7", "9"]
places = ["shop", "playground", "pharmacy", "park"]

D = np.loadtxt("results/predictions.txt")
timestamps = D[:,0]
dates=[dt.datetime.fromtimestamp(ts) for ts in timestamps]
predictions = D[:,1:D.shape[1]]

fig, ax = plt.subplots()

xfmt = md.DateFormatter('%Y-%m-%d %H:%M:%S')
ax.xaxis.set_major_formatter(xfmt)

col = 0
for l in locations:
	for p in places:
		if ((len(sys.argv) < 2) or (sys.argv[1] == l)) and ((len(sys.argv) < 3) or (sys.argv[2] == p)):
			ax.plot(dates, predictions[:,col], label=prefix+l+" "+p)
		col = col + 1

plt.subplots_adjust(bottom=0.25, right=0.8)
ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
ax.grid(axis="both")
xt = np.arange(np.min(timestamps),np.max(timestamps),86400)
ax.xaxis.set_ticks([dt.datetime.fromtimestamp(ts) for ts in xt])
plt.xticks(rotation=90)

plt.show()
