import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
from helpers import *
from helpersClassifier import *

df = pd.read_csv(sys.argv[1],names=['Q','DelT','zQ','actz'])
x=df[['Q','DelT','zQ']]
y=df['actz']

dftest = pd.read_csv(sys.argv[2],names=['Q','DelT','zQ','actz'])
xtest=dftest[['Q','DelT','zQ']]
ytest=dftest['actz']

#create_polynomial_regression_model(x,y,int(sys.argv[2]))
#NN(x,y,int(sys.argv[2]))
#DecisionTreeRegression(x,y,int(sys.argv[2]))
#RandomForest(x,y,n_est=int(sys.argv[2]))
NearestNeighbours(x,y,xtest,ytest,3,modelName="model_PS_55")
