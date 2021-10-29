#!"C:\Users\royja\Miniconda3\python.exe"

import serial

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
