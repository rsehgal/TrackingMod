import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from sklearn.neural_network import MLPRegressor
from sklearn.datasets import make_regression

df = pd.read_csv(sys.argv[1]) #,names=['start','inspected_xz','end','inspected_zx'])
df=df.dropna()
#print(df)
#print(df.head(10))
#print(df.info())
#x=df[['start','inspected_xz','end']]
#y=df['inspected_zx']
N=40
x=df.iloc[: , :N]
y=df.iloc[: , N:N+2]
print(x.head(10))
print(y.head(10))
x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.15)
model = MLPRegressor(hidden_layer_sizes=(100,130,120,34),random_state=45, max_iter=1000,tol=0.001)
from mymlp import *
Inspect(model,x,y,numOfEpochs=100)
model.fit(x_train,y_train)
r_sq=model.score(x_test,y_test)
print('coefficient of determination:', r_sq)
  
