import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
from sklearn.neural_network import MLPRegressor
from sklearn.datasets import make_regression
import pickle
import colorama
from colorama import Fore

#df = pd.read_csv(sys.argv[1],names=['layer9_z','layer8_x','layer7_z','layer8_z'])
df_0 = pd.read_csv(sys.argv[1],names=['layerIndex','barIndex','Q','delT','x','z'])
df_1 = pd.read_csv(sys.argv[2],names=['layerIndex','barIndex','Q','delT','x','z'])
df_2 = pd.read_csv(sys.argv[3],names=['layerIndex','barIndex','Q','delT','x','z'])
df_3 = pd.read_csv(sys.argv[4],names=['layerIndex','barIndex','Q','delT','x','z'])
df_4 = pd.read_csv(sys.argv[5],names=['layerIndex','barIndex','Q','delT','x','z'])
df_7 = pd.read_csv(sys.argv[6],names=['layerIndex','barIndex','Q','delT','x','z'])
df_8 = pd.read_csv(sys.argv[7],names=['layerIndex','barIndex','Q','delT','x','z'])
df_9 = pd.read_csv(sys.argv[8],names=['layerIndex','barIndex','Q','delT','x','z'])
#print(df.head(10))
#print(df.info())


x_0=df_0[['layerIndex','barIndex','Q','delT']].to_numpy()
x_1=df_1[['layerIndex','barIndex','Q','delT']].to_numpy()
x_2=df_2[['layerIndex','barIndex','Q','delT']].to_numpy()
x_3=df_3[['layerIndex','barIndex','Q','delT']].to_numpy()
x_4=df_4[['layerIndex','barIndex','Q','delT']].to_numpy()
x_7=df_7[['layerIndex','barIndex','Q','delT']].to_numpy()
x_8=df_8[['layerIndex','barIndex','Q','delT']].to_numpy()
x_9=df_9[['layerIndex','barIndex','Q','delT']].to_numpy()

#print(x_0)
#print("Shape x_0 : "+str(x_0.shape))
#print("Shape x_1 : "+str(x_1.shape))
#print("Shape x_2 : "+str(x_2.shape))
#print("Shape x_3 : "+str(x_3.shape))
#print("Shape x_4 : "+str(x_4.shape))
#print("Shape x_7 : "+str(x_7.shape))
#print("Shape x_8 : "+str(x_8.shape))
#print("Shape x_9 : "+str(x_9.shape))

y_0=df_0[['x','z']].to_numpy()
y_1=df_1[['x','z']].to_numpy()
y_2=df_2[['x','z']].to_numpy()
y_3=df_3[['x','z']].to_numpy()
y_4=df_4[['x','z']].to_numpy()
y_7=df_7[['x','z']].to_numpy()
y_8=df_8[['x','z']].to_numpy()
y_9=df_9[['x','z']].to_numpy()

#Loading all the models
model_0 = pickle.load(open("NN3_Model_Layer_0.sav", 'rb'))
model_1 = pickle.load(open("NN3_Model_Layer_1.sav", 'rb'))
model_2 = pickle.load(open("NN3_Model_Layer_2.sav", 'rb'))
model_3 = pickle.load(open("NN3_Model_Layer_3.sav", 'rb'))
model_4 = pickle.load(open("NN3_Model_Layer_4.sav", 'rb'))
model_7 = pickle.load(open("NN3_Model_Layer_7.sav", 'rb'))
model_8 = pickle.load(open("NN3_Model_Layer_8.sav", 'rb'))
model_9 = pickle.load(open("NN3_Model_Layer_9.sav", 'rb'))

diffX_Layer_0=[]
diffX_Layer_1=[]
diffX_Layer_2=[]
diffX_Layer_3=[]
diffX_Layer_4=[]
diffX_Layer_7=[]
diffX_Layer_8=[]
diffX_Layer_9=[]

diffZ_Layer_0=[]
diffZ_Layer_1=[]
diffZ_Layer_2=[]
diffZ_Layer_3=[]
diffZ_Layer_4=[]
diffZ_Layer_7=[]
diffZ_Layer_8=[]
diffZ_Layer_9=[]

for i in range(len(x_0)):
	x0=[x_0[i]]
	x0Arr=np.array(x0)
	y_p_0=model_0.predict(x0Arr)
	#print(Fore.BLUE+"Actual Value : "+str((y_0[i][0],y_0[i][1])))
	#print(Fore.RED+"Predicted Value : "+str((y_p_0[0][0],y_p_0[0][1])))

	#print(y_0,y_p_0)
	y_p_1=model_1.predict(np.array([x_1[i]]))
	y_p_2=model_2.predict(np.array([x_2[i]]))
	y_p_3=model_3.predict(np.array([x_3[i]]))
	y_p_4=model_4.predict(np.array([x_4[i]]))
	y_p_7=model_7.predict(np.array([x_7[i]]))
	y_p_8=model_8.predict(np.array([x_8[i]]))
	y_p_9=model_9.predict(np.array([x_9[i]]))

	diffX_Layer_0.append(y_0[i][0]/10.-y_p_0[0][0]/10.)
	diffX_Layer_1.append(y_1[i][0]/10.-y_p_1[0][0]/10.)
	diffX_Layer_2.append(y_2[i][0]/10.-y_p_2[0][0]/10.)
	diffX_Layer_3.append(y_3[i][0]/10.-y_p_3[0][0]/10.)
	diffX_Layer_4.append(y_4[i][0]/10.-y_p_4[0][0]/10.)
	diffX_Layer_7.append(y_7[i][0]/10.-y_p_7[0][0]/10.)
	diffX_Layer_8.append(y_8[i][0]/10.-y_p_8[0][0]/10.)
	diffX_Layer_9.append(y_9[i][0]/10.-y_p_9[0][0]/10.)
	
	diffZ_Layer_0.append(y_0[i][1]/10.-y_p_0[0][1]/10.)
	diffZ_Layer_1.append(y_1[i][1]/10.-y_p_1[0][1]/10.)
	diffZ_Layer_2.append(y_2[i][1]/10.-y_p_2[0][1]/10.)
	diffZ_Layer_3.append(y_3[i][1]/10.-y_p_3[0][1]/10.)
	diffZ_Layer_4.append(y_4[i][1]/10.-y_p_4[0][1]/10.)
	diffZ_Layer_7.append(y_7[i][1]/10.-y_p_7[0][1]/10.)
	diffZ_Layer_8.append(y_8[i][1]/10.-y_p_8[0][1]/10.)
	diffZ_Layer_9.append(y_9[i][1]/10.-y_p_9[0][1]/10.)

fig, axs = plt.subplots(3, 3)
axs[0, 0].set_title('Hist DiffX Layer 0')
axs[0,0].hist(diffZ_Layer_0)
axs[0, 1].set_title('Hist DiffX Layer 1')
axs[0,1].hist(diffZ_Layer_1)
axs[0, 2].set_title('Hist DiffX Layer 2')
axs[0,2].hist(diffZ_Layer_2)
axs[1, 0].set_title('Hist DiffX Layer 3')
axs[1,0].hist(diffZ_Layer_3)
axs[1, 1].set_title('Hist DiffX Layer 4')
axs[1, 1].hist(diffZ_Layer_4)
axs[1, 2].set_title('Hist DiffX Layer 7')
axs[1, 2].hist(diffZ_Layer_7)
axs[2, 0].set_title('Hist DiffX Layer 8')
axs[2, 0].hist(diffZ_Layer_8)
axs[2, 1].set_title('Hist DiffX Layer 9')
axs[2, 1].hist(diffZ_Layer_9)

#plt.hist(diffX_Layer_1)
#plt.hist(diffX_Layer_2)
#plt.hist(diffX_Layer_3)
#plt.hist(diffX_Layer_4)
#plt.hist(diffX_Layer_7)
#plt.hist(diffX_Layer_8)
#plt.hist(diffX_Layer_9)

plt.show()

#x=df[['layer9_z','layer8_x','layer7_z']]
#y=df['layer8_z']
#x=df[['layerIndex','barIndex','Q','delT']]
#y=df[['x','z']]



