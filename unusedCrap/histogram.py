import math as m
import matplotlib.pyplot as plt
import numpy as np
import os

def graphit(arg):
    fp = open(arg, "r")
    line = fp.readlines()
    count = len(line) #returns the number of elements in line
    X = [] #horizontal distance
    Y = [] #depth
    for n in range(0, count-3):  #count through the lines taken from file
        stringholder = line[n].split() #split space delimited words in line up into string elements in stringholder, stringholder is a list
        X.append(float(stringholder[0]))
        Y.append(float(stringholder[1]))
    fp.close()
    X = np.array(X)
    Y = np.array(Y)
    Y = Y*(-1)
    #plt.plot(X,Y)
    #plt.imshow(X,Y)
    # Big bins
    plt.hist2d(X, Y, bins=(50, 50), cmap=plt.cm.jet)
    #plt.show()

prepath = '../outputfiles/'
#turn files into a list
entries = os.listdir(prepath)
#loop graphit, passing it new, blank, X and Y lists each time
for i in entries:
    graphit(prepath + i)
#do histogram stuff
plt.show()

