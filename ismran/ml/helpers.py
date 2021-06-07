import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
import pickle
from matplotlib import pyplot as plt
from sklearn.metrics import mean_squared_error, r2_score

modelfilePrefix = "model_layer_"

def CreateAndSaveModel(layerNo):
	df = pd.read_csv(sys.argv[1],names=['start','inspected_xz','end','inspected_zx'])
	x=df[['start','inspected_xz','end']]
	y=df['inspected_zx']
	model = LinearRegression()
	model.fit(x,y)
	#layerNo=sys.argv[2]
	pickle.dump(model,open(modelfilePrefix+layerNo+".sav",'wb'))

def ReadModel(layerNo):
	loaded_model = pickle.load(open(modelfilePrefix+layerNo+".sav", 'rb'))
	return loaded_model

from sklearn.preprocessing import PolynomialFeatures

def create_polynomial_regression_model(x,y,degree):
	"Creates a polynomial regression model for the given degree"
	x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.75)
  
	poly_features = PolynomialFeatures(degree=degree)

	# transforms the existing features to higher degree features.
	x_train_poly = poly_features.fit_transform(x_train)

	# fit the transformed features to Linear Regression
	poly_model = LinearRegression()
	poly_model.fit(x_train_poly, y_train)

	# predicting on training data-set
	y_train_predicted = poly_model.predict(x_train_poly)

	# predicting on test data-set
	y_test_predict = poly_model.predict(poly_features.fit_transform(x_test))

	# evaluating the model on training dataset
	rmse_train = np.sqrt(mean_squared_error(y_train, y_train_predicted))
	r2_train = r2_score(y_train, y_train_predicted)

	# evaluating the model on test dataset
	rmse_test = np.sqrt(mean_squared_error(y_test, y_test_predict))
	r2_test = r2_score(y_test, y_test_predict)

	print("The model performance for the training set")
	print("-------------------------------------------")
	print("RMSE of training set is {}".format(rmse_train))
	print("R2 score of training set is {}".format(r2_train))

	print("\n")

	print("The model performance for the test set")
	print("-------------------------------------------")
	print("RMSE of test set is {}".format(rmse_test))
	print("R2 score of test set is {}".format(r2_test))
	PlotDiffHist(x_test,y_test,y_test_predict)

def PlotDiffHist(x_test,y_test,y_predict):
	x_test = x_test.to_numpy()
	print("================")
	print((y_test.shape,y_predict.shape))
	print("================")

	print((len(y_test),len(y_predict)))
	y_test = y_test.to_numpy()
	#y_predict = y_predict.to_numpy()
	diff=[]
	supList=[]
	supListOut=[]
	#bins=np.linspace(-15,16,50)
	for i in range(len(y_test)):
	    subList=[]
	    subListIn=[]
	    subList.append(y_test[i][0])
	    subList.append(y_predict[i][0])
	    subListIn.append(x_test[i][0])
	    subListIn.append(y_predict[i][0])

	    #print((y_test[i],y_predict[i]))
	    #print(y_test[i]-y_predict[i])
	    diff.append(y_test[i]-y_predict[i])
	    #print(y_test[i])
	    #print(y_predict[i])
	    supList.append(subList)
	    supListOut.append(subListIn)
	print("@@@@@@@@@@@@@@@@@@@@@@@@ Loop over, now going to plot the histogram @@@@@@@@@@@")
	#plt.hist(diff)#,bins=bins)
	supListArr = np.array(supList)
	supListOutArr = np.array(supListOut)
	print("Shape fo final array: "+str(supListArr.shape))
	np.savetxt("higher.txt", supListArr)
	np.savetxt("output.txt", supListOutArr)

	#plt.show()
