import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
from helpers import *

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

barName=sys.argv[1]
modelPath="../datasets/"+barName+"/model_Classification_neighbors_5_"+barName+".sav"
classificationModel = LoadClassificationModel(modelPath)


modelList=[]
for posValue in posList:
	regModelPath="../datasets/"+barName+"/model_Regression_"+barName+"_"+str(posValue)+".sav"
	if posValue >= 0:
		regModelPath="../datasets/"+barName+"/model_Regression_"+barName+"_+"+str(posValue)+".sav"
	modelList.append(LoadRegressionModel(regModelPath))

dftest = pd.read_csv(sys.argv[2],names=['Q','DelT','zQ','actz'])
dftest = dftest.sample(frac=1).reset_index(drop=True)

#xtestClassifcation=dftest[['Q','DelT','zQ']]
xtestClassifcation=dftest[['Q','DelT']]
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
	
	if i > 100000 :
		break;
	
np.savetxt("output.csv",np.array(supList),delimiter=" ")
print("All the predictions done...")
plt.hist(predictedZ,bins=bins)
plt.show()




'''
ypredClassification = classificationModel.predict(xtestClassifcation)
regModelIndex=PositionToIndex(ypredClassification)

xtestRegression=dftest[['Q','DelT']]
#print(xtest)

poly_model=modelList[regModelIndex]
poly_features = PolynomialFeatures(degree=4)
y_test_predict = poly_model.predict(poly_features.fit_transform(xtestRegression))
'''
