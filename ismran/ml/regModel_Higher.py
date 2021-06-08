import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
from helpers import *
#df = pd.read_csv(sys.argv[1],names=['layer9_z','layer8_x','layer7_z','layer8_z'])


#df = pd.read_csv(sys.argv[1],names=['start','inspected_xz','end','inspected_zx'])
#df = pd.read_csv(sys.argv[1],names=['startx','starty','startz','insy','insz','endx','endy','endz','insx'])
#print(df.head(10))
#print(df.info())

#x=df[['layer9_z','layer8_x','layer7_z']]
#y=df['layer8_z']

#x=df[['start','inspected_xz','end']]
#y=df['inspected_zx']
if int(sys.argv[3])==1:
	df = pd.read_csv(sys.argv[1],names=['start11','start2','startE','ins1','ins2','insE','end1','end2','endE','endz','ins3'])
	#x=df[['start11','start2','ins1','ins2','end1','end2']]
	x=df[['start11','start2','startE','ins1','ins2','insE','end1','end2','endE','endz']]
	y=df[['ins3']]
else:
	df = pd.read_csv(sys.argv[1],names=['start','inspected_xz','end','inspected_zx'])
	x=df[['start','inspected_xz','end']]
	y=df['inspected_zx']


create_polynomial_regression_model(x,y,int(sys.argv[2]))
