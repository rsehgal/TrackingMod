# Python program to demonstrate 
# KNN classification algorithm 
# on IRIS dataser 

from sklearn.datasets import load_iris 
from sklearn.neighbors import KNeighborsClassifier 
import numpy as np 
from sklearn.model_selection import train_test_split 
from readTree import *
from numpy import genfromtxt
from sklearn import preprocessing


filename="data.csv"
dataFileName="result.root"
signal=getSignalData(dataFileName)
print(len(signal))
background=getBackgroundData(dataFileName)
print(len(background))
dataArr=np.array(np.concatenate((signal,background),axis=0))

supList=[]
for e in dataArr:
    subList=[]
    for i in range(5):
        subList.append(e[i])
    subList.append(e[9])
    supList.append(subList)

dataArrSub=np.array(supList)
#normalizedDataArraySub = preprocessing.scale(dataArrSub) #
#normalizedDataArraySub = preprocessing.normalize(dataArrSub)

np.savetxt(filename,dataArrSub,delimiter=",")
#np.savetxt(filename,normalizedDataArraySub,delimiter=",")
targetList=[]

for e in signal:
    targetList.append(e[8])

for e in background:
    targetList.append(e[8])


dataArray = genfromtxt(filename, delimiter=',')
targetArray=np.array(targetList)


print(type(dataArray))
print(len(dataArray))
print(type(targetArray))
print(len(targetArray))
print(targetArray[2000])
print(targetArray[5000])

#Learning Process
kn = KNeighborsClassifier(n_neighbors=1) 
kn.fit(dataArray, targetArray) 
print("")
print("Learning done......")
print("")

#Testing started
testFilename="data2.csv"
testDataFileName="TMVA_FePb.root"
testSignal=getSignalData(testDataFileName)
print(len(testSignal))
testBackground=getBackgroundData(testDataFileName)
print(len(testBackground))
testDataArr=np.array(np.concatenate((testSignal,testBackground),axis=0))

#Creating target for signal for scoring
testSignalTargetList=[]
for e in testSignal:
    testSignalTargetList.append(e[8])
testSignalTargetArray=np.array(testSignalTargetList)

testDataArr=testSignal

testSupList=[]
for e in testDataArr:
    testSubList=[]
    for i in range(5):
        testSubList.append(e[i])
    testSubList.append(e[9])
    testSupList.append(testSubList)

testDataArrSub=np.array(testSupList)
#normalizedTestDataArraySub = preprocessing.scale(testDataArrSub) #
#normalizedTestDataArraySub = preprocessing.normalize(testDataArrSub)

np.savetxt(testFilename,testDataArrSub,delimiter=",")
#np.savetxt(testFilename,normalizedTestDataArraySub,delimiter=",")
testDataArray = genfromtxt(testFilename, delimiter=',')
print(type(testDataArray))
print(len(testDataArray))

filterPocaList=[]
for e in testDataArr:
    subList=[]
    for i in range(5):
        subList.append(e[i])
    subList.append(e[9])
    prediction = kn.predict(np.array([subList]))
    #if(prediction[0]!=0):
    filterList=[]
    for j in range(5,8):
        filterList.append(e[j])
    filterList.append(prediction[0])
    filterPocaList.append(filterList)

testFilterPocaArr=np.array(filterPocaList)
np.savetxt("testFilterPoca.txt",testFilterPocaArr,delimiter=" ")
print("Test score on Whole Data: {:.2f}".format(kn.score(testDataArray, testSignalTargetArray)))
	

'''
X_train, X_test, y_train, y_test = train_test_split(dataArray, targetArray, random_state=0)
kn = KNeighborsClassifier(n_neighbors=1) 
kn.fit(X_train, y_train) 

#filterList=[]
#filterPocaList=[]
#for x_new in X_test:

filterPocaList=[]

counter=0
predictionSignalCounter=0
trueSignalCounter=0
for e in dataArr:
    
    subList=[]
    for i in range(5):
        subList.append(e[i])
    prediction = kn.predict(np.array([subList]))
    #if(targetList[counter]==4):
    if(targetList[counter]==1):
	trueSignalCounter=trueSignalCounter+1
    if(prediction[0]==1): # and prediction[0]==targetList[counter]):
	predictionSignalCounter=predictionSignalCounter+1
        filterList=[]
        for j in range(5,8):
            filterList.append(e[j])
        filterList.append(e[0])
        filterPocaList.append(filterList)
    counter=counter+1

print("PredictionSignalCounter : %s"  % predictionSignalCounter)
print("TrueSignalCounter : %s"  % trueSignalCounter)
print("Counter : %s"  % counter)
#filterArr=np.array(filterList)
#np.savetxt("filter.txt",filterArr,delimiter=",")

filterPocaArr=np.array(filterPocaList)
np.savetxt("filterPoca.txt",filterPocaArr,delimiter=" ")

print("PredictionRatio : {:.2f}".format(predictionSignalCounter/float(trueSignalCounter)) )
print("Test score on Test Data: {:.2f}".format(kn.score(X_test, y_test)))
print("Test score on Train Data: {:.2f}".format(kn.score(X_train, y_train)))
print("Test score on Whole Data: {:.2f}".format(kn.score(dataArray, targetArray)))
'''

