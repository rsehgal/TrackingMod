import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
from helpers import *
from sklearn.metrics import mean_squared_error, r2_score
from scipy.stats import norm

df = pd.read_csv(sys.argv[1],names=['Q','DelT','zQ','cl'])
x=df[['Q','DelT']]
y=df['zQ']

df = pd.read_csv(sys.argv[2],names=['Q','DelT','zQ','cl'])
xtest=df[['Q','DelT']]
ytest=df['zQ']

act_pos=df['cl']
y_test_pred=create_polynomial_regression_model_2(x,y,xtest,ytest,int(sys.argv[3]))
r2_test = r2_score(act_pos, y_test_pred)
print("============ R2 Score from Actual Source Position ===========")
print(r2_test)
print("=============================================================")
bins=np.linspace(-80,80,160)
diffActPos=y_test_pred-act_pos
(mu,sigma)=norm.fit(diffActPos)
print("======= mean : sigma ========")
print(mu,sigma)
plt.hist(diffActPos,bins=bins)
plt.show()

#KNN(x,y,xtest,ytest,int(sys.argv[3]))
#DecisionTreeRegression(x,y,xtest,ytest,int(sys.argv[3]))
#RandomForest(x,y,xtest,ytest,n_est=int(sys.argv[3]))
#LinearSVR(x,y)
