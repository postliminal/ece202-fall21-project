#!/opt/miniconda3/bin python3

import numpy as np
from matplotlib import pyplot as plt

data37 = np.loadtxt('rssi_data_ch37.txt', delimiter=',')  # shape (1000, 2)
data38 = np.loadtxt('rssi_data_ch38.txt', delimiter=',')  # shape (1000, 2)
data39 = np.loadtxt('rssi_data_ch39.txt', delimiter=',')  # shape (1000, 2)

x1 = data37[:, 0]
y1 = data37[:, 1]
x2 = data38[:, 0]
y2 = data38[:, 1]
x3 = data39[:, 0]
y3 = data39[:, 1]

# plt.rcParams["figure.figsize"] = (18, 3)

fig, axs = plt.subplots(3, 1, sharex=True)
fig.suptitle('This is a somewhat long figure title', fontsize=16)

axs[0].plot(x1, y1, 'r', label='Channel 37')
axs[0].set_yscale('linear')
axs[0].set_ylabel('RSSI (dBm)')
axs[0].set_ylim([-100, 0])
axs[0].legend(loc='upper right')
axs[0].grid(visible=True)

axs[1].plot(x2, y2, 'k', label='Channel 38')
axs[1].set_yscale('linear')
axs[1].set_ylabel('RSSI (dBm)')
axs[1].set_ylim([-100, 0])
axs[1].legend(loc='upper right')
axs[1].grid(visible=True)

axs[2].plot(x3, y3, 'b', label='Channel 39')
axs[2].set_yscale('linear')
axs[2].set_ylabel('RSSI (dBm)')
axs[2].set_ylim([-100, 0])
axs[2].legend(loc='upper right')
axs[2].grid(visible=True)

plt.show()
