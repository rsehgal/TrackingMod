# Python program to demonstrate 
# KNN classification algorithm 
# on IRIS dataser 

from sklearn.datasets import load_iris 
from sklearn.neighbors import KNeighborsClassifier 
import numpy as np 
from sklearn.model_selection import train_test_split 
from readTree import *
from numpy import genfromtxt

'''
iris_dataset=load_iris() 
#iris_dataset=getData() 

X_train, X_test, y_train, y_test = train_test_split(iris_dataset["data"], iris_dataset["target"], random_state=0) 

kn = KNeighborsClassifier(n_neighbors=1) 
kn.fit(X_train, y_train) 

x_new = np.array([[5, 2.9, 1, 0.2]]) 
prediction = kn.predict(x_new) 

print("Predicted target value: {}\n".format(prediction)) 
print("Predicted feature name: {}\n".format
	(iris_dataset["target_names"][prediction])) 
print("Test score: {:.2f}".format(kn.score(X_test, y_test))) 
'''

filename="data.csv"
dataFileName="result.root"
signal=getSignalData(dataFileName)
background=getBackgroundData(dataFileName)
dataArr=np.array(np.concatenate((signal,background),axis=0))

supList=[]
for e in dataArr:
    subList=[]
    for i in range(5):
        subList.append(e[i])
    supList.append(subList)

dataArrSub=np.array(supList)

np.savetxt(filename,dataArrSub,delimiter=",")
targetList=[]

for e in signal:
    targetList.append(1)

for e in background:
    targetList.append(0)


dataArray = genfromtxt(filename, delimiter=',')
targetArray=np.array(targetList)

'''
dataList=[]
targetList=[]
for e in signal:
    #print(e)
    dataList.append(e)
    targetList.append(1)

for e in background:
    #print(e)
    dataList.append(e)
    targetList.append(0)

dataArray=np.array(dataList)
#dataArray.reshape(1,-1)
targetArray=np.array(targetList)
'''

print(type(dataArray))
print(len(dataArray))
print(type(targetArray))
print(len(targetArray))
X_train, X_test, y_train, y_test = train_test_split(dataArray, targetArray, random_state=0)
kn = KNeighborsClassifier(n_neighbors=1) 
kn.fit(X_train, y_train) 

#filterList=[]
#filterPocaList=[]
#for x_new in X_test:
'''
for x_new in dataArray:
    #print(x_new)
    prediction = kn.predict(np.array([x_new]))
    #print(prediction[0])
    if(prediction[0]):
        filterList.append(x_new)
        #filterPocaList.append([x_new[4],x_new[5],x_new[6],1.0])
'''
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

