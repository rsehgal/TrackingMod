import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
import matplotlib.pyplot as plt
#df = pd.read_csv(sys.argv[1],names=['layer9_z','layer8_x','layer7_z','layer8_z'])
df = pd.read_csv(sys.argv[1],names=['start','end','ins'])
#print(df.head(10))
#print(df.info())

#x=df[['layer9_z','layer8_x','layer7_z']]
#y=df['layer8_z']
x=df[['start','end']]
y=df['ins']

print(x.head(10))

#x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.85)

#x = np.array([5, 15, 25, 35, 45, 55]).reshape((-1, 1))
#y = np.array([5, 20, 14, 32, 22, 38])

#model = LinearRegression().fit(x, y)
#r_sq = model.score(x, y)
#print('coefficient of determination:', r_sq)

model = LinearRegression()
model.fit(x,y)
#model.fit(x_train,y_train)

#dftest = pd.read_csv(sys.argv[2],names=['layer9_z','layer8_x','layer7_z'])
dftest = pd.read_csv(sys.argv[2],names=['start','end','ins','delT','barIndex'])
#dftest = pd.read_csv('_17March_1_training.txt',names=['layer9_z','layer8_x','layer7_z'])

#dftest = pd.read_csv('_17March_1_training.txt',names=['layer9_z','layer8_x','layer7_z','layer8_z'])
#dftest = pd.read_csv('_simulatedHome_training.txt',names=['layer9_z','layer8_x','layer7_z','layer8_z'])
#print(df.head(10))
#print(df.info())


#x_test=dftest[['layer9_z','layer8_x','layer7_z']]
x_test=dftest[['start','end']]
y_test=dftest['ins']
delt=dftest['delT']
barIndex=dftest['barIndex']
bins=np.linspace(-300,300,600)

#print(x_test)
print("Shape of Xtest : "+str(x_test.shape))
y_predict = model.predict(x_test)
#r_sq=model.score(x_test,y_test)
#print('coefficient of determination:', r_sq)

print("==================================================")

#for i in range(20):
#    print((y_test[i],y_predict[i]))

supList=[]
resi=[]
x_test = x_test.to_numpy()
for i in range(len(x_test)):
    #print(i)
    #subList=[]
    #print(x_test[i])
    #subList.append(x_test[i][1])
    if barIndex[i] == 41:
        #subList.append(delt[i])
        #subList.append(y_predict[i])
        subList=[delt[i],y_predict[i]]
        supList.append(subList)

    resi.append(y_predict[i]-y_test[i])
supListArray = np.array(supList)
np.savetxt("output_"+"Layer_"+sys.argv[3]+".txt", supListArray)

plt.hist(resi,bins=bins)
plt.show()
