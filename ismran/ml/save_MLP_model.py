import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
from sklearn.neural_network import MLPRegressor
from sklearn.datasets import make_regression

#df = pd.read_csv(sys.argv[1],names=['layer9_z','layer8_x','layer7_z','layer8_z'])
df = pd.read_csv(sys.argv[1],names=['layerIndex','barIndex','Q','delT','x','z'])
#print(df.head(10))
#print(df.info())

#x=df[['layer9_z','layer8_x','layer7_z']]
#y=df['layer8_z']
x=df[['layerIndex','barIndex','Q','delT']]
y=df[['x','z']]

layerNo=sys.argv[2]
print("Generting model for Layer : "+str(layerNo))
#print(x.head(10))

#x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.15)
#model = LinearRegression()

#model = MLPRegressor(hidden_layer_sizes=(150,100,20),random_state=45, max_iter=1000,tol=0.001)

from sklearn.neighbors import KNeighborsRegressor
model = KNeighborsRegressor(n_neighbors=10,weights='uniform')

from mymlp import *
#Inspect(model,x,y,numOfEpochs=50)
#model.fit(x_train,y_train)
model.fit(x,y)

import pickle
pickle.dump(model,open("NN3_Model_Layer_"+layerNo+".sav",'wb'))

#r_sq=model.score(x_test,y_test)
#print('coefficient of determination:', r_sq)

