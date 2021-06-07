import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
#df = pd.read_csv(sys.argv[1],names=['layer9_z','layer8_x','layer7_z','layer8_z'])
df = pd.read_csv(sys.argv[1],names=['start','inspected_xz','end','inspected_zx'])
#print(df.head(10))
#print(df.info())

#x=df[['layer9_z','layer8_x','layer7_z']]
#y=df['layer8_z']
x=df[['start','inspected_xz','end']]
y=df['inspected_zx']

print(x.head(10))

x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.25)
model = LinearRegression()
model.fit(x_train,y_train)

import pickle
layerNo=sys.argv[2]
pickle.dump(model,open("model_layer_"+layerNo+".sav",'wb'))

r_sq=model.score(x_test,y_test)
print('coefficient of determination:', r_sq)
x_test = x_test.to_numpy()
y_test = y_test.to_numpy()
y_predict = model.predict(x_test)
print("==================================================")
print(y_predict)
print("Shape Of y_test : "+str(y_test.shape))

supList=[]
for i in range(len(x_test)):
    #print(i)
    subList=[]
    #print(x_test[i])
    subList.append(x_test[i][1])
    subList.append(y_predict[i])
    supList.append(subList)

supListArray = np.array(supList)
np.savetxt("sim_output_Layer_"+layerNo+".txt", supListArray)

print("Shape Of y_predict : "+str(y_predict.shape))
diff=[]
for i in range(len(y_test)):
    #print((y_test[i],y_predict[i]))
    diff.append(y_test[i]-y_predict[i])
    #print(y_test[i])
    #print(y_predict[i])
plt.hist(diff)
plt.show()


#x = np.array([5, 15, 25, 35, 45, 55]).reshape((-1, 1))
#y = np.array([5, 20, 14, 32, 22, 38])

#model = LinearRegression().fit(x, y)
#r_sq = model.score(x, y)
#print('coefficient of determination:', r_sq)
'''
model = LinearRegression()
#model.fit(x,y)
model.fit(x_train,y_train)
#dftest = pd.read_csv(sys.argv[2],names=['layer9_z','layer8_x','layer7_z'])
dftest = pd.read_csv(sys.argv[2],names=['start','inspected_xz','end'])

#dftest = pd.read_csv('_17March_1_training.txt',names=['layer9_z','layer8_x','layer7_z'])

#dftest = pd.read_csv('_17March_1_training.txt',names=['layer9_z','layer8_x','layer7_z','layer8_z'])
#dftest = pd.read_csv('_simulatedHome_training.txt',names=['layer9_z','layer8_x','layer7_z','layer8_z'])
#print(df.head(10))
#print(df.info())


#x_test=dftest[['layer9_z','layer8_x','layer7_z']]
x_test=dftest[['start','inspected_xz','end']]
#y_test=dftest['layer8_z']


#print(x_test)
print("Shape of Xtest : "+str(x_test.shape))
y_predict = model.predict(x_test)
#r_sq=model.score(x_test,y_test)
#print('coefficient of determination:', r_sq)

print("==================================================")

#for i in range(20):
#    print((y_test[i],y_predict[i]))

supList=[]
x_test = x_test.to_numpy()
for i in range(len(x_test)):
    #print(i)
    subList=[]
    #print(x_test[i])
    subList.append(y_predict[i])
    subList.append(x_test[i][1])
    supList.append(subList)

supListArray = np.array(supList)
np.savetxt("output_"+"Layer_"+sys.argv[3]+".txt", supListArray)
'''
