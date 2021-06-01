import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
df = pd.read_csv('_simulated_10L_training.txt',names=['layer9_z','layer8_x','layer7_z','layer8_z'])
#print(df.head(10))
#print(df.info())
x=df[['layer9_z','layer8_x','layer7_z']]
y=df['layer8_z']
print(x.head(10))

x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.85)

#x = np.array([5, 15, 25, 35, 45, 55]).reshape((-1, 1))
#y = np.array([5, 20, 14, 32, 22, 38])

#model = LinearRegression().fit(x, y)
#r_sq = model.score(x, y)
#print('coefficient of determination:', r_sq)

model = LinearRegression()
model.fit(x_train,y_train)
r_sq=model.score(x_test,y_test)
print('coefficient of determination:', r_sq)
