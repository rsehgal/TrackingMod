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

print(x.head(10))

#x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.15)
#model = LinearRegression()

#model = MLPRegressor(hidden_layer_sizes=(10,5),random_state=45, max_iter=1000,tol=0.001)
#from mymlp import *
model = MLPRegressor(hidden_layer_sizes=(150,100,20),random_state=45, max_iter=1000,tol=0.001)
#Inspect(model,x,y,numOfEpochs=50)

from sklearn.neighbors import KNeighborsRegressor
#model = KNeighborsRegressor(n_neighbors=100,weights='uniform')
#model.fit(x_train,y_train)
model.fit(x,y)

#import pickle
#layerNo=sys.argv[2]
#pickle.dump(model,open("model_layer_"+layerNo+".sav",'wb'))

dftest = pd.read_csv(sys.argv[2],names=['layerIndex','barIndex','Q','delT','x','z'])
x_test=df[['layerIndex','barIndex','Q','delT']]
y_test=df[['x','z']]


r_sq=model.score(x_test,y_test)
print('coefficient of determination:', r_sq)

y_pred=model.predict(x_test)
#y_pred=y_pred.to_numpy()
y_test=y_test.to_numpy()

print("Shape of Y_Test : "+str(y_test.shape))
print("Shape of Y_Pred : "+str(y_pred.shape))

bins=np.linspace(-60,60,120)

diffX=[]
diffZ=[]
for i in range(len(x_test)):
	#print(y_test[i],y_pred[i])
	diffX.append((y_test[i][0]-y_pred[i][0])/10.)
	diffZ.append((y_test[i][1]-y_pred[i][1])/10.)
	
print("Size of diffX : "+str(len(diffX)))
print("Size of diffZ : "+str(len(diffZ)))

fig, axs = plt.subplots(1, 2)
axs[0].set_title('Hist DiffX')
axs[0].hist(diffX,color='r',bins=bins)
axs[1].set_title('Hist DiffZ')
axs[1].hist(diffZ,color='g',bins=bins)


#plt.hist(diffX)
#plt.hist(diffZ)
plt.show()
