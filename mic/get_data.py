#!"C:\Users\royja\Miniconda3\python.exe"

import serial
import os
import re
print(os.getcwd())


# 10 s clips
# save to file and plot too
arr = []
with serial.Serial('COM4', 9600, timeout=1) as ser:
    for i in range(16000):
        line = ser.readline()
        # line = ser.read(2)
        cleanline = line.rstrip(b'\r\n')
        arr.append(cleanline)
        # print("line: ", cleanline)

print(arr[0:100])


apath = os.path.join(os.getcwd(), "mic", "audio.csv")

with open(apath, "a+") as f:
    if os.stat(apath).st_size == 0:
        print("created file")
        for i in range(len(arr)):
            dum = re.sub(r'\'.+?\'', '', str(arr[i]))
            # print(dum)
            f.write(str(arr[i])+"\n")
