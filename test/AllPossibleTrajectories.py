import turtle
import reeds_shepp as rs
import utils
import draw
import math
import random as rd
import sys
import os



def main():
    firstAngle = 0
    lastAngle = 0
    pts = []

    with open("route.txt", "r") as reader:
        # Read and print the entire file line by line
        line = reader.readline()
        firstAngle = float(line)
        line = reader.readline()
        lastAngle = float(line)
        i = 0
        prev = 0
        line = reader.readline()
        while line != '':  # The EOF char is an empty string
            num = float(line)
            if i % 2 == 1:
                pts.append((prev/ rs.PathElement.RATIO, num/ rs.PathElement.RATIO))
            line = reader.readline()
            i = i + 1
            prev = num

    
    # generate PATH so the vectors are pointing at each other
    PATH = []
    PATH.append((pts[0][0], pts[0][1], firstAngle))
    for i in range(1, len(pts) - 1):
        dx = pts[i+1][0] - pts[i][0]
        dy = pts[i+1][1] - pts[i][1]
        theta = math.atan2(dy, dx)
        PATH.append((pts[i][0], pts[i][1], utils.rad2deg(theta)))
    PATH.append((pts[-1][0], pts[-1][1], lastAngle))

    
    # init turtle
    tesla = turtle.Turtle()
    tesla.speed(0) # 0: fast; 1: slow, 8.4: cool
    tesla.shape('arrow')
    tesla.resizemode('user')
    tesla.shapesize(1, 1)

    screen = turtle.Screen()
    screen.screensize(2500, 4000)


    # draw vectors representing points in PATH
    for pt in PATH:
        draw.goto(tesla, pt)
        draw.vec(tesla)

    # draw all routes found
    tesla.speed(0)
    #edges = rs.get_all_polygons()

    f = open("trajectory.txt","w")
    
    s = "#Paths " + str(len(PATH) - 1) + "\n"
    f.write(s)#number of segments at all
    #allBannedTrajectories = []
    for i in range(len(PATH) - 1):
        paths = rs.get_all_paths(PATH[i], PATH[i+1])
        #forbiddenTrajectories = []
        s = "_PathID " + str(i) + " from " + str(PATH[i][0]) + " " + str(PATH[i][1]) + " to " + str(PATH[i+1][0]) + " " + str(PATH[i+1][1]) + "\n"
        f.write(s)
        s = "__#Possible_Path_Segment " + str(len(paths)) + "\n"
        f.write(s)
        j = 0
        for path in paths:
            s = "___SegmentID_" + str(j) + "_PathID_" + str(i) 
            
            draw.set_random_pencolor(tesla)
            draw.goto(tesla, PATH[i])
            #first = PATH[i]
            #s = "____ " + str(PATH[i][0]) + " " + str(PATH[i][1]) + " " + str(PATH[i][2]) + "\n"
            
            localTrajectory = draw.draw_path(tesla, path)
            s = s + " " + str(len(localTrajectory)) + "\n"
            f.write(s)
            j += 1

            for k in range(len(localTrajectory)):
                s = "_____Section " + str(round(localTrajectory[k][0], 3)) + " "
                s = s + str(round(localTrajectory[k][1], 3)) + " "
                param = round(localTrajectory[k][2], 2)
                s = s + str(param) + " "
                typeOfTraj = localTrajectory[k][3]
                s = s + str(typeOfTraj) + " "
                steering = localTrajectory[k][4]
                s = s + str(steering) + "\n"
                f.write(s)
    
    f.close()
    # draw shortest route
    tesla.pencolor(1, 0, 0)
    tesla.pensize(3)
    tesla.speed(10)
    draw.goto(tesla, PATH[0])
    path_length = 0
    trajectory = []
    g = open("Trace.txt","w")
    s = str(round(rs.PathElement.RATIO*pts[0][0], 3)) + " " + str(round(rs.PathElement.RATIO*pts[0][1], 3)) + " B\n"
    g.write(s)

    for i in range(len(PATH) - 1):
        path = rs.get_optimal_path(PATH[i], PATH[i+1])
        path_length += rs.path_length(path)
        trajectory = draw.draw_path(tesla, path)
        for j in range(len(trajectory)):
            s = str(round(rs.PathElement.RATIO*trajectory[j][0], 3)) + " " + str(round(rs.PathElement.RATIO*trajectory[j][1], 3)) + \
                " " + str(round(trajectory[j][2], 2)) + " " + trajectory[j][3] + " " + trajectory[j][4] + "\n"
            g.write(s)


    print("Shortest path length: {} px.".format(int(draw.scale(path_length))))
    g.close()

    turtle.done()


if __name__ == '__main__':
        main()
