# Regression Example With Boston Dataset: Baseline
from pandas import read_csv
from keras.models import Sequential
from keras.layers import Dense
from keras.wrappers.scikit_learn import KerasRegressor
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import KFold
# load dataset
import pandas as pd
import sys
import matplotlib.pyplot as plt
df = pd.read_csv(sys.argv[1],names=['qnear','qfar','delt','actz'])
#print(df.head(10))
#print(df.info())

x=df[['qnear','qfar','delt']]
y=df['actz']

#dataframe = read_csv("housing.csv", delim_whitespace=True, header=None)
#dataset = dataframe.values
# split into input (X) and output (Y) variables
#X = dataset[:,0:13]
#Y = dataset[:,13]
# define base model
def baseline_model():
	# create model
	model = Sequential()
	model.add(Dense(3, input_dim=3, kernel_initializer='normal', activation='relu'))
	model.add(Dense(1, kernel_initializer='normal'))
	# Compile model
	model.compile(loss='mean_squared_error', optimizer='adam')
	return model
# evaluate model
estimator = KerasRegressor(build_fn=baseline_model, epochs=100, batch_size=5, verbose=0)
kfold = KFold(n_splits=10)
print("========== MODEL BUILT ==============")
results = cross_val_score(estimator, x, y, cv=kfold)
print("Baseline: %.2f (%.2f) MSE" % (results.mean(), results.std()))
