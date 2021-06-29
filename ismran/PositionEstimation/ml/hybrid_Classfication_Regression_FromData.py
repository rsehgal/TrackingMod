import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
from helpers import *
from barnames import *
import colorama
from colorama import Fore

posList=[-45,-40,-30,-20,-10,0,10,20,30,40,45]
#posList=[-45,-30,-10,10,30,45]
print("=========================================================")
print("========== Testing Position To Index Conversion =========")
print("=========================================================")
print("Index of : "+str(posList[0])+" : "+str(PositionToIndex(posList[0])))
indexList=[]
for i in range(1,len(posList)-1):
	print("Index of : "+str(posList[i])+" : "+str(PositionToIndex(posList[i])))
	indexList.append(PositionToIndex(posList[i]))
print("Index of : "+str(posList[len(posList)-1])+" : "+str(PositionToIndex(posList[len(posList)-1])))
print("=========================================================")
print("========= Loading models at different positions =========")
print("=========================================================")

modelDict={}
for layerNo in range(3):
	for barName in layer[layerNo]:
		regModelList=[]
		for posValue in posList:
			regModelPath="../datasets/"+barName+"/model_Regression_"+barName+"_"+str(posValue)+".sav"
			if posValue >= 0:
				regModelPath="../datasets/"+barName+"/model_Regression_"+barName+"_+"+str(posValue)+".sav"
			regModelList.append(LoadRegressionModel(regModelPath))

		classificationModelPath="../datasets/"+barName+"/model_Classification_neighbors_5_"+barName+".sav"
		modelDict[barName]=[LoadClassificationModel(classificationModelPath),regModelList]	

dftest = pd.read_csv(sys.argv[1],names=['barIndexStart','QStart','DelTStart','zQStart',
					'barIndexIns','QIns','DelTIns','zQIns',
					'barIndexEnd','QEnd','DelTEnd','zQEnd'])
barStart=dftest['barIndexStart']
startPtClass=dftest[['QStart','DelTStart','zQStart']].to_numpy()
startPtReg=dftest[['QStart','DelTStart']].to_numpy()

barIns=dftest['barIndexIns']
insPtClass=dftest[['QIns','DelTIns','zQIns']].to_numpy()
insPtReg=dftest[['QIns','DelTIns']].to_numpy()

barEnd=dftest['barIndexEnd']
endPtClass=dftest[['QEnd','DelTEnd','zQEnd']].to_numpy()
endPtReg=dftest[['QEnd','DelTEnd']].to_numpy()

print("=====================================")
#print(dftest.head(5))
#print(barStart)
print(Fore.RED+"Need to Process : "+str(len(startPtClass))+" Events")
for i in range(5):
	print(barNamesList[barStart[i]],startPtClass[i])
print("=====================================")
#print(startPt.head(5))
#print(barIns)
for i in range(5):
	print(barNamesList[barIns[i]],insPtClass[i])
print("=====================================")
#print(insPt.head(5))
#print(barEnd)
for i in range(5):
	print(barNamesList[barEnd[i]],endPtClass[i])
print("=====================================")
#print(endPt.head(5))

zPredictStartList=[]
#zPredictInsList=[]
#zPredictEndList=[]

supList=[]
#for i in range(10000):#len(startPtClass)):
for i in range(len(startPtClass)):
	xtestStart = np.array([startPtClass[i]])	
	xtestIns = np.array([insPtClass[i]])	
	xtestEnd = np.array([endPtClass[i]])	

	barNameStart = barNamesList[barStart[i]]
	barNameIns = barNamesList[barIns[i]]
	barNameEnd = barNamesList[barEnd[i]]

	classificationModelStart = modelDict[barNameStart][0]
	classificationModelIns = modelDict[barNameIns][0]
	classificationModelEnd = modelDict[barNameEnd][0]
	
	regModelStart = modelDict[barNameStart][1]
	regModelIns = modelDict[barNameIns][1]
	regModelEnd = modelDict[barNameEnd][1]

	ypredStartClassification = classificationModelStart.predict(xtestStart)
	ypredInsClassification = classificationModelIns.predict(xtestIns)
	ypredEndClassification = classificationModelEnd.predict(xtestEnd)

	#print(ypredClassification)
	regModelStartIndex=PositionToIndex(ypredStartClassification)
	regModelInsIndex=PositionToIndex(ypredInsClassification)
	regModelEndIndex=PositionToIndex(ypredEndClassification)
	#print(regModelIndex)
	#print("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&")
	#print(startPtReg)	
	xtestRegressionValStart=np.array([startPtReg[i]])
	xtestRegressionValIns=np.array([insPtReg[i]])
	#print(xtestRegressionValIns)
	xtestRegressionValEnd=np.array([endPtReg[i]])
	#print(xtestRegressionValEnd)

	poly_features = PolynomialFeatures(degree=4)

	poly_modelStart=regModelStart[regModelStartIndex]
	poly_modelIns=regModelIns[regModelInsIndex]
	poly_modelEnd=regModelEnd[regModelEndIndex]

	#print(i,xtestRegressionValStart)
	y_test_predict_Start = poly_modelStart.predict(poly_features.fit_transform(xtestRegressionValStart))
	zPredictStartList.append(y_test_predict_Start)
	y_test_predict_Ins = poly_modelIns.predict(poly_features.fit_transform(xtestRegressionValIns))
	y_test_predict_End = poly_modelEnd.predict(poly_features.fit_transform(xtestRegressionValEnd))
	#print(y_test_predict)
	if not len(zPredictStartList)%50000 :
		print("Processed : "+str(len(zPredictStartList))+" events")
		#plt.hist(predictedZ,bins=bins)
		#plt.savefig("hist.png");
	#predictedZ.append(y_test_predict[0])
	#subList=[xtestRegressionVal[0][0],xtestRegressionVal[0][1],y_test_predict[0]]
	subList=[y_test_predict_Start[0],y_test_predict_Ins[0],y_test_predict_End[0]]
	supList.append(subList)

print("Shape of output : "+str(np.array(supList).shape))	
np.savetxt("ML_Layer2.txt",np.array(supList),delimiter=",")

#plt.plot([1,2,3,4],[1,2,3,4])
#plt.show()



'''
barName=sys.argv[1]
modelPath="../datasets/"+barName+"/model_Classification_neighbors_5_"+barName+".sav"
classificationModel = LoadClassificationModel(modelPath)


modelList=[]
for posValue in posList:
	regModelPath="../datasets/"+barName+"/model_Regression_"+barName+"_"+str(posValue)+".sav"
	if posValue >= 0:
		regModelPath="../datasets/"+barName+"/model_Regression_"+barName+"_+"+str(posValue)+".sav"
	modelList.append(LoadRegressionModel(regModelPath))
'''

'''
dftest = pd.read_csv(sys.argv[2],names=['Q','DelT','zQ','actz'])
xtestClassifcation=dftest[['Q','DelT','zQ']]
ytest=dftest['actz']
xtestRegression=dftest[['Q','DelT']]

xtestClassifcation = xtestClassifcation.to_numpy()
xtestRegression = xtestRegression.to_numpy()
predictedZ = []

PlotConfusionMatrix(classificationModel,xtestClassifcation,ytest)
plt.show()

bins=np.linspace(-80,80,160)
#for xtest in xtestClassifcation:
print("============= Starting Predictions ====================")
supList=[]
for i in range(len(xtestClassifcation)):
	#xtest = np.array([xtest])	
	xtest = np.array([xtestClassifcation[i]])	
	#print(xtest)
	ypredClassification = classificationModel.predict(xtest)
	#print(ypredClassification)
	regModelIndex=PositionToIndex(ypredClassification)
	#print(regModelIndex)
	
	xtestRegressionVal=xtestRegression[i]#dftest[['Q','DelT']]
	xtestRegressionVal=np.array([xtestRegressionVal])
	#print(xtestRegressionVal)
	poly_model=modelList[regModelIndex]
	poly_features = PolynomialFeatures(degree=4)
	y_test_predict = poly_model.predict(poly_features.fit_transform(xtestRegressionVal))
	#print(y_test_predict)
	if not len(predictedZ)%10000 :
		print("Processed : "+str(len(predictedZ))+" events")
		#plt.hist(predictedZ,bins=bins)
		#plt.savefig("hist.png");
	predictedZ.append(y_test_predict[0])
	subList=[xtestRegressionVal[0][0],xtestRegressionVal[0][1],y_test_predict[0]]
	supList.append(subList)
	
np.savetxt("output.csv",np.array(supList),delimiter=" ")
print("All the predictions done...")
plt.hist(predictedZ,bins=bins)
plt.show()
'''



'''
ypredClassification = classificationModel.predict(xtestClassifcation)
regModelIndex=PositionToIndex(ypredClassification)

xtestRegression=dftest[['Q','DelT']]
#print(xtest)

poly_model=modelList[regModelIndex]
poly_features = PolynomialFeatures(degree=4)
y_test_predict = poly_model.predict(poly_features.fit_transform(xtestRegression))
'''
