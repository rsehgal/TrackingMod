import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from matplotlib import pyplot as plt
from helpers import *
import colorama
from colorama import Fore
print(Fore.RED+"Processing File :"+sys.argv[1])
df = pd.read_csv(sys.argv[1],names=['Q','DelT','zQ','pos'])
x=df[['Q','DelT']]
y=df['zQ']
#print(df['pos'])
#pos=int(df['pos'])
save_regression_model(x,y,sys.argv[2],int(sys.argv[3]),int(sys.argv[4]))
