import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import numpy as np

def func(x,a,b,c):
    return a*np.exp(-b*x)+c
    
def funcGauss(x,amp,mean,sigma):
    val=amp*np.exp(-(x-mean)**2 /  2*sigma**2) / np.sqrt(2*np.pi*sigma**2)
    
def GenData():
    xdata = np.linspace(0, 4, 50)
    y = func(xdata, 2.5, 1.3, 0.5)
    np.random.seed(1729)
    y_noise = 0.2 * np.random.normal(size=xdata.size)
    ydata = y + y_noise
    return xdata,ydata

def Fit(xdata,ydata):
    popt, pcov = curve_fit(func, xdata, ydata)
    return popt


xdata,ydata=GenData()
plt.plot(xdata,ydata,'b-',label='data')
popt=Fit(xdata,ydata)
plt.plot(xdata,func(xdata,*popt),'r-',label='fit')
plt.legend()
plt.show()
