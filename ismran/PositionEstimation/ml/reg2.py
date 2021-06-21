import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
import matplotlib.pyplot as plt

df = pd.read_csv(sys.argv[1],names=['qnear','qfar','qmean','delt','actz'])
#print(df.head(10))
#print(df.info())

#x=df[['qnear','qfar','qmean','delt']]
x=df[['delt']]
y=df['actz']

x2=[[2.68],[2.7],[2.86],[2.95],[3.08],[3.25],[3.4],[3.51],[3.6],[3.7],[3.8]]
x2=np.array(x2)
y2=[-45.,-40.,-30.,-20.,-10.,0.,10.,20.,30.,40.,45.]
y2=np.array(y2)
#print(x.head(10))

#x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.85)


#model = LinearRegression().fit(x, y)
#r_sq = model.score(x, y)
#print('coefficient of determination:', r_sq)

model = LinearRegression()
model2 = LinearRegression()

from sklearn.neighbors import KNeighborsRegressor
#model = KNeighborsRegressor(n_neighbors=10,weights='uniform')

from sklearn.tree import DecisionTreeRegressor
#model=DecisionTreeRegressor(random_state=0)

from sklearn.neural_network import MLPRegressor
#model=MLPRegressor(random_state=1, max_iter=10)

model.fit(x,y)
model2.fit(x2,y2)
print("Training Done.......")
dftest = pd.read_csv(sys.argv[2],names=['qnear','qfar','qmean','delt','actz'])
#print(dftest.head(10))
#print(dftest.info())

#x_test=dftest[['qnear','qfar','qmean','delt']]
x_test=dftest[['delt']]
y_test=dftest['actz']


#print(x_test)
print("Shape of Xtest : "+str(x_test.shape))
y_predict = model.predict(x_test)
ypred=y_predict.reshape(-1,1)
y_predict2 = model2.predict(ypred)
print(y_predict2)
r_sq=model.score(x_test,y_test)
print('coefficient of determination:', r_sq)
print("==================================================")

bins=np.linspace(-50,50,200)
print("Mean : "+str(np.array(y_predict).mean()))
print("SD : "+str(np.array(y_predict).std()))
plt.hist(y_predict,bins=bins)
plt.hist(y_predict2,bins=bins)

plt.show()

#for i in range(20):
#    print((y_test[i],y_predict[i]))
'''
supList=[]
x_test = x_test.to_numpy()
for i in range(len(x_test)):
    #print(i)
    subList=[]
    #print(x_test[i])
    subList.append(x_test[i][1])
    subList.append(y_predict[i])
    supList.append(subList)

supListArray = np.array(supList)
np.savetxt("output_"+"Layer_"+sys.argv[3]+".txt", supListArray)
'''
