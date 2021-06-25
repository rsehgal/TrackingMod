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

#df = pd.read_csv(sys.argv[1],names=['Q','DelT','zQ'])
#x=df[['Q']]
#y=df['zQ']

#create_polynomial_regression_model(x,y,int(sys.argv[2]))
#NN(x,y,int(sys.argv[2]))
#DecisionTreeRegression(x,y,int(sys.argv[2]))
RandomForest(x,y,n_est=int(sys.argv[2]))
