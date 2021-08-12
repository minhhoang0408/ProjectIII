import turtle
import reeds_shepp as rs
import utils
import draw
import math
import random as rd
import sys
import os

firstAngle = 0
lastAngle = 0
pts = []

with open("route.txt", "r") as reader:
    # Read and print the entire file line by line
    line = reader.readline()
    firstAngle = round(float(line), 4)
    line = reader.readline()
    lastAngle = round(float(line), 4)
    i = 0
    prev = 0
    line = reader.readline()
    while line != '':  # The EOF char is an empty string
        num = round(float(line), 4)
        if i % 2 == 1:
            pts.append((prev/ rs.PathElement.RATIO, num/ rs.PathElement.RATIO))
        line = reader.readline()
        i = i + 1
        prev = num
    print ("I : ", i)
    

# generate PATH so the vectors are pointing at each other
PATH = []
PATH.append((pts[0][0], pts[0][1], firstAngle))
for i in range(1, len(pts) - 1):
    dx = pts[i+1][0] - pts[i][0]
    dy = pts[i+1][1] - pts[i][1]
    theta = math.atan2(dy, dx)
    PATH.append((pts[i][0], pts[i][1], utils.rad2deg(theta)))
PATH.append((pts[-1][0], pts[-1][1], lastAngle))

# print ("++++ READ FILE ++++") 
# for pt in pts: 
#   print("X: {}, Y: {}".format(pt[0], pt[1]))

# print ("+++++ PATH LIST ++++++")
# for path in PATH:
#   print("X: {}, Y: {}, THETA: {}".format(path[0], path[1], path[2]))

full_path = []
total_length = 0

for i in range(len(PATH) - 1):
    path = rs.get_optimal_path(PATH[i], PATH[i+1])
    full_path += path
    total_length += rs.path_length(path)

print("Shortest path length: {}".format(round(total_length, 2)))

for e in full_path:
    print(e) 

