#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Fri Nov 22 09:55:47 2019

@author: rsehgal
"""

import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def gaussian(x, xc, sigma,amp):
    val= amp*np.exp( -(x-xc)**2 / (2*sigma**2)) / np.sqrt(2*np.pi*sigma**2)
    #print(val)
    return val


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