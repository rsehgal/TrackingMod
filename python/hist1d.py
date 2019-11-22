#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Fri Nov 22 10:00:24 2019

@author: rsehgal
"""

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
print(pd.__version__)
import sys
from Fit1DHelpers import *
import Fit1DHelpers

def ReadData(filename):
    df=pd.read_csv(filename,delim_whitespace=True,names=('x','y','z','scat','inten'))
    #print(df)
    #print("-------------------------------------")
    x=df['x'].values
    y=df['y'].values
    z=df['z'].values
    #print("Type of X : "+str(type(x)))
    #print(x)
    return x,y,z

np.random.seed(1)
'''
x = np.random.poisson(size=(160))
print("First Type of X : "+str(type(x)))
print(x.shape)
y = np.random.poisson(size=(160))
print("First Type of X : "+str(type(x)))
print(y.shape)
'''
x,y,z=ReadData(sys.argv[1])
print("Actual Type of X : "+str(type(x)))
print(x.shape)
print("Actual Type of Y : "+str(type(y)))
print(y.shape)
#fig, ax = plt.subplots()
#ax.set_aspect("equal")
numOfBins=201

voxelBins=np.linspace(-500,500,numOfBins)

plt1=plt.subplot(2,1,1)
histx,bins,patches = plt.hist(x,numOfBins,color='blue')
#plt2=plt.subplot(2,1,2)
#histy,binsy,patchesy = plt.hist(y,numOfBins,color='green')
#plt.show()

zList=[]
for xbinIndex in range(numOfBins):
    zList.append(histx[xbinIndex])

print("Length of ZList : "+str(len(zList)))

Z=np.array(zList)
print("Shape of Z : "+str(Z.shape))

gcen1=-150
gsigma1=100
gamp1=150
gcen2=150
gsigma2=100
gamp2=150

#popt_2gauss, pcov_2gauss= curve_fit(_2gaussian,X,Znoise,p0=[gcen1,gsigma1,gamp1,gcen2,gsigma2,gamp2])
popt_2gauss, pcov_2gauss= curve_fit(Fit1DHelpers._gaussian,voxelBins,Z,p0=[gcen1,gsigma1,gamp1,gcen2,gsigma2,gamp2])
print("Reached line 73..................")
perr_2gauss = np.sqrt(np.diag(pcov_2gauss))
pars_1 = popt_2gauss[0:3]
pars_2 = popt_2gauss[3:6]
gauss_peak_1 = gaussian(voxelBins, *pars_1)
gauss_peak_2 = gaussian(voxelBins, *pars_2)
plt.subplot(2,1,2)
#plt.plot(voxelBins,Z,'k--',label="Data")
plt.scatter(voxelBins,Z,color='k',label="Data")
plt.plot(voxelBins,gauss_peak_1,'m-',label="first fitted gaussian")
plt.plot(voxelBins,gauss_peak_2,'g-',label="second fitted gaussian")
plt.legend()
plt.show()