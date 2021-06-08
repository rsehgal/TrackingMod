import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
#df = pd.read_csv(sys.argv[1],names=['layer9_z','layer8_x','layer7_z','layer8_z'])
df7 = pd.read_csv("output_level1_Layer_7.csv",names=['z','xp','x'])
df8 = pd.read_csv("output_level1_Layer_8.csv",names=['x','zp','z'])
df9 = pd.read_csv("output_level1_Layer_9.csv",names=['z','xp','x'])

df7 = df7.to_numpy()
df8 = df8.to_numpy()
df9 = df9.to_numpy()

print("Shape of df7 : "+str(df7.shape))
print("Shape of df8 : "+str(df8.shape))
print("Shape of df9 : "+str(df9.shape))

xsupList=[]
ysupList=[]
for i in range(df7.shape[0]):
	xsubList=[df9[i][1],df7[i][0],df8[i][0]]
	ysupList.append(df7[i][2])
	xsupList.append(xsubList)

xsupListArr=np.array(xsupList)
ysupListArr=np.array(ysupList)

print("Shape of X : "+str(xsupListArr.shape))
print("Shape of Y : "+str(ysupListArr.shape))
#x=df[['start','inspected_xz','end']]
#y=df7['x']


x=xsupListArr
y=ysupListArr
x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.75)
model = LinearRegression()
model.fit(x_train,y_train)
r_sq=model.score(x_test,y_test)
print('coefficient of determination:', r_sq)
y_predict = model.predict(x_test)
print("Shape Of y_predict : "+str(y_predict.shape))
diff=[]
for i in range(len(y_test)):
    #print((y_test[i],y_predict[i]))
    diff.append(y_test[i]-y_predict[i])
    #print(y_test[i])
    #print(y_predict[i])
plt.hist(diff)
print("SD : "+str(np.array(diff).std()))
plt.show()


'''
print(x.head(10))

x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.75)
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
    subList.append(y_test[i])
    supList.append(subList)

supListArray = np.array(supList)
np.savetxt("output_level1_Layer_"+layerNo+".txt", supListArray)

print("Shape Of y_predict : "+str(y_predict.shape))
diff=[]
for i in range(len(y_test)):
    #print((y_test[i],y_predict[i]))
    diff.append(y_test[i]-y_predict[i])
    #print(y_test[i])
    #print(y_predict[i])
plt.hist(diff)
print("SD : "+str(np.array(diff).std()))
plt.show()
'''
