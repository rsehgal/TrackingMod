import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
from helpers import *

df = pd.read_csv(sys.argv[1],names=['Q','DelT','zQ'])
x=df[['Q','DelT']]
y=df['zQ']

df = pd.read_csv(sys.argv[2],names=['Q','DelT','zQ'])
xtest=df[['Q','DelT']]
ytest=df['zQ']

create_polynomial_regression_model_2(x,y,xtest,ytest,int(sys.argv[3]))
#KNN(x,y,xtest,ytest,int(sys.argv[3]))
#DecisionTreeRegression(x,y,xtest,ytest,int(sys.argv[3]))
#RandomForest(x,y,xtest,ytest,n_est=int(sys.argv[3]))
#LinearSVR(x,y)
