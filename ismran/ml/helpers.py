import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
import pickle
from matplotlib import pyplot as plt

modelfilePrefix = "model_layer_"

def CreateAndSaveModel(layerNo):
	df = pd.read_csv(sys.argv[1],names=['start','inspected_xz','end','inspected_zx'])
	x=df[['start','inspected_xz','end']]
	y=df['inspected_zx']
	model = LinearRegression()
	model.fit(x,y)
	#layerNo=sys.argv[2]
	pickle.dump(model,open(modelfilePrefix+layerNo+".sav",'wb'))

def ReadModel(layerNo)
	loaded_model = pickle.load(open(modelfilePrefix+layerNo+".sav", 'rb'))
	return loaded_model
