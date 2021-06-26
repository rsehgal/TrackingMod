
from sklearn.metrics import confusion_matrix
from sklearn import tree
from sklearn.metrics import roc_curve, auc
from scipy import interp
import numpy as np
import matplotlib.pyplot as plt

def whoami():
	import sys
	return sys._getframe(1).f_code.co_name

def NearestNeighbours(x_train,y_train,x_test,y_test,num_neighbours=3):
	print("========== %s Classifier =========" % whoami())
	from sklearn.neighbors import KNeighborsClassifier
	clf = KNeighborsClassifier(n_neighbors=num_neighbours)
	#Y_train=Y_train.reshape(X_train.shape[0])
	clf = clf.fit(x_train,y_train )
	print("========= Model built ===========")
	y_pred=clf.predict(x_test)
	print("========= Prediction done =======")
	#print(X_pred.shape)
	#print(X_test.shape)
	score = clf.score(x_test, y_test)
	print(score)
	print(confusion_matrix(y_test,y_pred))
	#if(writeToFile):
	#	WriteToFile(X_Test,X_pred,whoami())
	#return clf

