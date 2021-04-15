import ROOT
from ROOT import TFile,TTree

def Split(numOfShots,filename):
    print("Requested number of Shots : "+str(numOfShots))
    print(filename)
    fFile=TFile(filename, "READ")
    fTree= fFile.Get("ftree")
    nEntries=fTree.GetEntries()
    nEntriesInOneShot=int(nEntries/numOfShots)
    for i in range(numOfShots):
        entriesStart=i*nEntriesInOneShot
        entriesEnd=(i+1)*nEntriesInOneShot
        df = ROOT.RDataFrame("ftree", filename)
        outfileName="Split_"+str(i+1)+".root"
        df.Range(entriesStart,entriesEnd).Snapshot("ftree", outfileName)


import sys

if __name__ == "__main__":
    Split(int(sys.argv[1]),sys.argv[2])
