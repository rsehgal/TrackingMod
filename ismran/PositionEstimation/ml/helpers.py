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
from sklearn.metrics import plot_confusion_matrix

modelfilePrefix = "model_layer_"

def PositionToIndex(pos):
	if pos==-45:
		return 0
	if pos==45:
		return 10
	return int((pos+40)/10+1)

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

def create_polynomial_regression_model_2(x,y,xtest,ytest,degree):
	"Creates a polynomial regression model for the given degree"
	#x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.75)
  
	poly_features = PolynomialFeatures(degree=degree)

	# transforms the existing features to higher degree features.
	#x_train_poly = poly_features.fit_transform(x_train)
	x_train_poly = poly_features.fit_transform(x)

	# fit the transformed features to Linear Regression
	poly_model = LinearRegression()
	poly_model.fit(x_train_poly, y)
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
	#y_test_predict = poly_model.predict(poly_features.fit_transform(x_test))
	y_test_predict = poly_model.predict(poly_features.fit_transform(xtest))

	# evaluating the model on training dataset
	rmse_train = np.sqrt(mean_squared_error(y, y_train_predicted))
	r2_train = r2_score(y, y_train_predicted)

	# evaluating the model on test dataset
	rmse_test = np.sqrt(mean_squared_error(ytest, y_test_predict))
	r2_test = r2_score(ytest, y_test_predict)

	print("The model performance for the training set")
	print("-------------------------------------------")
	print("RMSE of training set is {}".format(rmse_train))
	print("R2 score of training set is {}".format(r2_train))

	print("\n")

	print("The model performance for the test set")
	print("-------------------------------------------")
	print("RMSE of test set is {}".format(rmse_test))
	print("R2 score of test set is {}".format(r2_test))
	PlotDiffHist(xtest,ytest,y_test_predict)

def save_regression_model(x,y,barName,location,degree=4):
	modelName="model_Regression_"+barName+"_"+str(location)+".sav"
	if location >= 0:
		modelName="model_Regression_"+barName+"_+"+str(location)+".sav"
	"Creates a polynomial regression model for the given degree"
	poly_features = PolynomialFeatures(degree=degree)

	# transforms the existing features to higher degree features.
	x_poly = poly_features.fit_transform(x)

	# fit the transformed features to Linear Regression
	poly_model = LinearRegression()
	poly_model.fit(x_poly, y)
	print("========= Model built ===========")
	pickle.dump(poly_model,open(modelName,'wb'))
	print("Model saved as : "+modelName)
'''
def LoadRegressionModel(barName,pos):
	#modelName="model_Regression_"+barName+"_"+str(pos)+".sav"
	modelName="model_Regression_"+barName+"_"+str(pos)+"_degree_4.sav"
	print("Loaded model : "+modelName)
	loaded_model = pickle.load(open(modelName, 'rb'))
	return loaded_model

'''
'''
def LoadClassificationModel(barName):
        #modelName="model_Classification_"+barName+".sav"
        modelName="model_Classification_neighbors_5"+barName+".sav"
        print("Loaded model : "+modelName)
        loaded_model = pickle.load(open(modelName, 'rb'))
        return loaded_model
'''
def LoadRegressionModel(modelPath):
	#modelName="model_Regression_"+barName+"_"+str(pos)+".sav"
	#modelName="model_Regression_"+barName+"_"+str(pos)+"_degree_4.sav"
	print("Loaded Regression model : "+modelPath)
	loaded_model = pickle.load(open(modelPath, 'rb'))
	return loaded_model

def LoadClassificationModel(modelPath):
        #modelName="model_Classification_"+barName+".sav"
        #modelName="model_Classification_neighbors_5"+barName+".sav"
        print("Loaded Classification model : "+modelPath)
        loaded_model = pickle.load(open(modelPath, 'rb'))
        return loaded_model


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
	bins=np.linspace(-80,80,160)
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

def ProcessIt(model,x_train,y_train,x_test,y_test):
	#x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.75)
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

def KNN(x,y,xtest,ytest,numOfNeigh):
	model = KNeighborsRegressor(n_neighbors=numOfNeigh,weights='uniform')
	ProcessIt(model,x,y,xtest,ytest)

def DecisionTreeRegression(x,y,xtest,ytest,depth=8):
	dtree = DecisionTreeRegressor(max_depth=depth, min_samples_leaf=0.13, random_state=3)
	ProcessIt(dtree,x,y,xtest,ytest)

def RandomForest(x,y,xtest,ytest,n_est=10,ran_state=100):
	model_rf = RandomForestRegressor(n_estimators=n_est, oob_score=True, random_state=ran_state)
	ProcessIt(model_rf,x,y,xtest,ytest)

def SVR(x,y,xtest,ytest):
	#svr_rbf = SVR(kernel='rbf', C=100, gamma=0.1, epsilon=.1)
	from sklearn.svm import SVR
	svr=SVR(kernel='rbf',C=100,gamma=0.1,epsilon=0.1)
	ProcessIt(svr,x,y,xtest,ytest)

def LinearSVR(x,y):
	from sklearn.svm import LinearSVR
	from sklearn.pipeline import make_pipeline
	from sklearn.preprocessing import StandardScaler
	regr = make_pipeline(StandardScaler(),LinearSVR(random_state=0, tol=1e-5))
	ProcessIt(regr,x,y)

def PlotConfusionMatrix(model,x_test,y_test):
	score = model.score(x_test, y_test)
	print(score)
	plot_confusion_matrix(model, x_test, y_test,normalize='true')
