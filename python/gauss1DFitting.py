#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Wed Nov 20 09:43:50 2019

@author: rsehgal
"""

import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from Fit1DHelpers import *
import Fit1DHelpers
'''
def gaussian(x, xc, sigma,amp):
    val= amp*np.exp( -(x-xc)**2 / (2*sigma**2)) / np.sqrt(2*np.pi*sigma**2)
    #print(val)
    return val
'''

'''
#working nicely in curve_fit
#Callable for curve_fit
def _gaussian(M,*args):
    x=M
    arr=np.zeros(x.shape)
    #print("Shape of X : "+str(x.shape))
    #print(args)
    for i in range(len(args)//3):
        arr += gaussian(x, *args[i*3:i*3+3])
    #plt.plot(x,arr)
    #plt.show()
    return arr
'''

'''
#Callable for curve_fit
def _gaussian(M,*args):
    x=M
    arr=np.zeros(x.shape)
    #print("Shape of X : "+str(x.shape))
    #print(args)
    for i in range(len(args)//3):
        #print("I : "+str(i))
        arr += gaussian(x, *args[i*3:i*3+3])
    #plt.plot(x,arr)
    #plt.show()
    return arr
'''

'''
def _2gaussian(x_array,cen1,sigma1,amp1,cen2,sigma2,amp2):
    return amp1*(1/(sigma1*(np.sqrt(2*np.pi))))*(np.exp((-1.0/2.0)*(((x_array-cen1)/sigma1)**2))) + \
            amp2*(1/(sigma2*(np.sqrt(2*np.pi))))*(np.exp((-1.0/2.0)*(((x_array-cen2)/sigma2)**2)))
'''
'''  
gprms = [(15.0,5.0,5.0),
         (30.0,3.0,10.0)
        ]
'''
gprms = (15.0,5.0,5.0,30.0,3.0,10.0)
# Standard deviation of normally-distributed noise to add in generating
# our test function to fit.
noise_sigma = 0.03

X=np.linspace(0,50,100)
# The function to be fit is Z.
Z = np.zeros(X.shape)
'''
for p in gprms:
    Z += gaussian(X,*p)
'''

Z =  Fit1DHelpers._gaussian(X,*gprms)
'''
plt.plot(X,Z)
plt.show()
'''
Znoise = Z + noise_sigma * np.random.randn(*Z.shape)

'''
# This is the callable that is passed to curve_fit. M is a (2,N) array
# where N is the total number of data points in Z, which will be ravelled
# to one dimension.
def _gaussian(M, *args):
    print("Shape of M : "+str(M.shape))
    x, y = M
    #print("Shape of x : "+str(x.shape)+" :: "+"Shape of y : "+str(y.shape))
    arr = np.zeros(x.shape)
    for i in range(len(args)//5):
       arr += gaussian(x, y, *args[i*5:i*5+5])
    return arr
'''

gcen1=15
gsigma1=1
gamp1=1
gcen2=30
gsigma2=1
gamp2=1

#popt_2gauss, pcov_2gauss= curve_fit(_2gaussian,X,Znoise,p0=[gcen1,gsigma1,gamp1,gcen2,gsigma2,gamp2])
popt_2gauss, pcov_2gauss= curve_fit(Fit1DHelpers._gaussian,X,Znoise,p0=[gcen1,gsigma1,gamp1,gcen2,gsigma2,gamp2])
perr_2gauss = np.sqrt(np.diag(pcov_2gauss))
pars_1 = popt_2gauss[0:3]
pars_2 = popt_2gauss[3:6]
gauss_peak_1 = gaussian(X, *pars_1)
gauss_peak_2 = gaussian(X, *pars_2)
print("----------------- Fitted Parameters --------------")
print(pars_1)
print(pars_2)

fit = np.zeros(Z.shape)
fit += Fit1DHelpers._gaussian(X, *popt_2gauss)

residual_gauss = Z - fit

rms = np.sqrt(np.mean(residual_gauss**2))
print('RMS residual =', rms)

plt1=plt.subplot(2,1,1)
plt2=plt.subplot(2,1,2,sharex=plt1,sharey=plt1)
plt1.plot(X,Z,'k--',label="Without Noise")
plt1.scatter(X,Znoise,label="Data With Noise")
plt1.plot(X,gauss_peak_1,'m-',label="first fitted gaussian")
plt1.plot(X,gauss_peak_2,'g-',label="second fitted gaussian")
plt1.legend()

#plt.subplot(2,1,2,sharex=True)
plt2.scatter(X,residual_gauss,color="red",label="Residual")
plt2.legend()


plt.show()
