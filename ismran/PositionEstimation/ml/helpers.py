import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
import pickle
from matplotlib import pyplot as plt
from sklearn.metrics import mean_squared_error, r2_score
from scipy.stats import norm
#-------------------------------
from sklearn.linear_model import LinearRegression
from sklearn.linear_model import Ridge
from sklearn.linear_model import Lasso
from sklearn.linear_model import ElasticNet
from sklearn.neighbors import KNeighborsRegressor
from sklearn.tree import DecisionTreeRegressor
from sklearn.svm import SVR
from sklearn.ensemble import RandomForestRegressor

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
	print("====================================================================")
	print("================ PRINTING MODEL HYPER PARAMETERS ===================")
	print(poly_model.coef_)
	print(poly_model.intercept_)
	print("====================================================================")
	print("============ Num Of Parameters :"+str(len(poly_model.coef_))+" ============")
	print("====================================================================")

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
	#diff=[]
	#supList=[]
	#supListOut=[]
	bins=np.linspace(-50,50,100)
	#(mu,sigma) = norm.fit(y_predict)
	#plt.hist(y_predict,bins=bins,facecolor='green')
	

	#import matplotlib.mlab as mlab
	#y = mlab.normpdf(bins,mu,sigma)
	#plot.plot(bins,y,'r--',linewidth=2)

	import scipy
	mu, sigma = scipy.stats.norm.fit(y_predict)
	print((mu,sigma))
	best_fit_line = scipy.stats.norm.pdf(bins, mu, sigma)
	#plt.plot(bins, best_fit_line)
	plt.hist(y_predict,bins=bins,facecolor='magenta')
	plt.show()

	supList=[]
	print("Shape Of Y_test : "+str(y_test.shape))
	for i in range(len(y_test)):
		subList=[x_test[i][0],x_test[i][1],y_predict[i]]
		supList.append(subList)

	supListArr=np.array(supList)
	#print(supListArr)
	np.savetxt("output.csv",supListArr,delimiter=',')

def ProcessIt(model,x,y):
	x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.75)
	model.fit(x_train, y_train)
	# predicting on test data-set
	y_predict = model.predict(x_test)

	# evaluating the model on test dataset
	rmse_test = np.sqrt(mean_squared_error(y_test, y_predict))
	r2_test = r2_score(y_test, y_predict)

	print("\n")

	print("The model performance for the test set")
	print("-------------------------------------------")
	print("RMSE of test set is {}".format(rmse_test))
	print("R2 score of test set is {}".format(r2_test))
	PlotDiffHist(x_test,y_test,y_predict)

def KNN(x,y,numOfNeigh):
	model = KNeighborsRegressor(n_neighbors=numOfNeigh,weights='uniform')
	ProcessIt(model,x,y)

def DecisionTreeRegression(x,y,depth=8):
	dtree = DecisionTreeRegressor(max_depth=depth, min_samples_leaf=0.13, random_state=3)
	ProcessIt(dtree,x,y)

def RandomForest(x,y,n_est=10,ran_state=100):
	model_rf = RandomForestRegressor(n_estimators=n_est, oob_score=True, random_state=ran_state)
	ProcessIt(model_rf,x,y)
