import numpy as np
from root_numpy import root2array, tree2array
from ROOT import TFile
def getData(filename):
    arr=root2array(filename,"Signal")
    return arr

def getSignalData(filename):
    arr=root2array(filename,"Signal")
    return arr

def getBackgroundData(filename):
    arr=root2array(filename,"Background")
    return arr


def getSignal(filename):
    infile=TFile(filename)
    signal=infile.Signal
    '''
    count=0
    for e in signal:
        count=count+1
        print(count)
        print(e.deltaThetaX)
    '''
    return signal

def getBackground(filename):
    infile=TFile(filename)
    background=infile.Background
    return background


def happyBirthday():
    print("Happy Birthday to you!")

def main():
    #signal=getSignal()
    #for e in signal:
    #    print(e.deltaThetaX)
    #happyBirthday()
    #print(getData())

    signal=getData()
    for e in signal:
        print(e)

#main()

#if __name__ == "__main__":
#    getSignal()
    #signal=getSignal()
    #for e in signal:
    #    print(e.deltaThetaX)

