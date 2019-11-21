#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Thu Nov 21 10:27:34 2019

@author: rsehgal
"""
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def gaussian(x,y, xc, yc,sigma_x,sigma_y,amp):
    #print(xyMesh.dtype)
    #val= amp*np.exp( -(x-xc)**2 / (2*sigma**2)) / np.sqrt(2*np.pi*sigma**2)
    #print(val)
    #(x,y)=xyMesh
    gauss = amp*np.exp(-((x-xc)**2/(2*sigma_x**2)+(y-yc)**2/(2*sigma_y**2)))/(2*np.pi*sigma_x*sigma_y)
    return gauss #np.ravel(gauss)

def gaussianData(xx,yy,*args):
    x,y=xx,yy
    
    #print("Leng of X : "+str(len(x))+" : Leng of Y : "+str(len(y)))
    arr=np.zeros((len(x),len(y)))
        
    #print("Shape of X : "+str(x.shape))
    #print(args)
    for i in range(len(args)//5):
        #print("I : "+str(i))
        arr += gaussian(x, y, *args[i*5:i*5+5])
    #plt.plot(x,arr)
    #plt.show()
    return arr

#Callable for curve_fit
#def _gaussian(xx,yy,*args):
def _gaussian(xy,*args):
    #x,y=xx,yy
    x,y=xy
    #print("Leng of X : "+str(len(x))+" : Leng of Y : "+str(len(y)))
    #arr=np.zeros((len(x),len(y)))
    arr=np.zeros(x.shape)
    
    #print("Shape of X : "+str(x.shape))
    #print(args)
    for i in range(len(args)//5):
        #print("I : "+str(i))
        arr += gaussian(x, y, *args[i*5:i*5+5])
    #plt.plot(x,arr)
    #plt.show()
    return arr