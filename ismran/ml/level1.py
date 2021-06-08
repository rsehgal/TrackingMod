import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
#df = pd.read_csv(sys.argv[1],names=['layer9_z','layer8_x','layer7_z','layer8_z'])
df = pd.read_csv(sys.argv[1],names=['start','inspected_xz','end','inspected_zx'])
x_train=df[['start','inspected_xz','end']]
y_train=df['inspected_zx']

dftest = pd.read_csv(sys.argv[2],names=['start','inspected_xz','end','inspected_zx'])
x_test=df[['start','inspected_xz','end']]
y_test=df['inspected_zx']

#print(x.head(10))

#x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.75)
model = LinearRegression()
model.fit(x_train,y_train)

import pickle
layerNo=sys.argv[3]
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
np.savetxt("output_level1_Layer_"+layerNo+".csv", supListArray,delimiter=',')

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

