import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
import matplotlib.pyplot as plt
import math
#df = pd.read_csv(sys.argv[1],names=['layer9_z','layer8_x','layer7_z','layer8_z'])
df = pd.read_csv(sys.argv[1],names=['start','inspected_xz','end','inspected_zx'])
#print(df.head(10))
#print(df.info())

#x=df[['layer9_z','layer8_x','layer7_z']]
#y=df['layer8_z']
x=df[['start','inspected_xz','end']]
y=df['inspected_zx']

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
dftest = pd.read_csv(sys.argv[2],names=['start','inspected_xz','end','inspected_zx','delT'])
#dftest = pd.read_csv('_17March_1_training.txt',names=['layer9_z','layer8_x','layer7_z'])

#dftest = pd.read_csv('_17March_1_training.txt',names=['layer9_z','layer8_x','layer7_z','layer8_z'])
#dftest = pd.read_csv('_simulatedHome_training.txt',names=['layer9_z','layer8_x','layer7_z','layer8_z'])
#print(df.head(10))
#print(df.info())


#x_test=dftest[['layer9_z','layer8_x','layer7_z']]
x_test=dftest[['start','inspected_xz','end']]
y_test=dftest['inspected_zx']
del_t=dftest['delT']


#print(x_test)
print("Shape of Xtest : "+str(x_test.shape))
y_predict = model.predict(x_test)
#r_sq=model.score(x_test,y_test)
#print('coefficient of determination:', r_sq)

print("==================================================")

#for i in range(20):
#    print((y_test[i],y_predict[i]))

supList=[]
diff=[]
x_test = x_test.to_numpy()
bins=np.linspace(-500.,500.,1000)
bins_x=np.linspace(-60.,60.,1000)
bins_y=np.linspace(-15.,15.,1000)
filt_delT=[]
filt_ypred=[]
supListDeltYpred=[]
for i in range(len(x_test)):
	#print(i)
	#subList=[]
	#print(x_test[i])
	#subList.append(x_test[i][1])
	#subList.append(y_predict[i])
	#supList.append(subList)
	if math.fabs(y_predict[i]) < 50 :
		diff.append(y_test[i]-y_predict[i])
		filt_delT.append(del_t[i])
		filt_ypred.append(y_predict[i])

		subListDeltYpred=[del_t[i],y_predict[i],y_test[i]]
		supListDeltYpred.append(subListDeltYpred)

supListArr=np.array(supListDeltYpred)
#filt_delT=np.array(filt_delT)
#filt_ypred=np.array(filt_ypred)
#print("Shape of Filt_T : "+str(filt_delT.shape))
#print("Shape of Filt Y_Pred : "+str(filt_ypred.shape))
print("SD of diff : "+str(np.array(diff).std()))
print("Mean of diff : "+str(np.array(diff).mean()))
plt.hist(diff,bins=bins)
plt.hist2d(filt_delT,filt_ypred,bins=(bins_x,bins_y))
plt.show()
np.savetxt("deltVsPredit.txt",supListArr)
#supListArray = np.array(supList)
#np.savetxt("output_"+"Layer_"+sys.argv[3]+".txt", supListArray)
