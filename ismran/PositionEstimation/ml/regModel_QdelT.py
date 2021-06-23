import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
import matplotlib.pyplot as plt

df = pd.read_csv(sys.argv[1],names=['Q','DelT','zQ'])
#print(df.head(10))
#print(df.info())

#x=df[['qnear','qfar','qmean','delt']]
x=df[['Q','DelT']]
y=df['zQ']


print(x.head(10))

#x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.85)


#model = LinearRegression().fit(x, y)
#r_sq = model.score(x, y)
#print('coefficient of determination:', r_sq)

#model = LinearRegression()

from sklearn.neighbors import KNeighborsRegressor
model = KNeighborsRegressor(n_neighbors=300,weights='uniform')

from sklearn.tree import DecisionTreeRegressor
#model=DecisionTreeRegressor(random_state=0)

from sklearn.neural_network import MLPRegressor
#model=MLPRegressor(random_state=1, max_iter=10)

model.fit(x,y)
print("Training Done.......")
dftest = pd.read_csv(sys.argv[2],names=['Q','DelT','zQ'])
#print(dftest.head(10))
#print(dftest.info())

#x_test=dftest[['qnear','qfar','qmean','delt']]
x_test=dftest[['Q','DelT']]
y_test=dftest['zQ']


#print(x_test)
print("Shape of Xtest : "+str(x_test.shape))
y_predict = model.predict(x_test)
r_sq=model.score(x_test,y_test)
print('coefficient of determination:', r_sq)
print("==================================================")

bins=np.linspace(-50,50,50)
print("Mean : "+str(np.array(y_predict).mean()))
print("SD : "+str(np.array(y_predict).std()))
plt.hist(y_predict,bins=bins)
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
