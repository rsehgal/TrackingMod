import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
print(pd.__version__)
import sys
from FitHelpers import *
import FitHelpers

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

#plt.hist(x,numOfBins)
#plt.hist(y,numOfBins)
#plt.show()
hist, xbins, ybins, im = plt.hist2d(x,y, bins=[voxelBins,voxelBins])
#plt.show()
xbins=xbins[0:len(xbins)-1]
ybins=ybins[0:len(ybins)-1]

print("XBins : "+str(xbins)+" : YBins : "+str(ybins))

#xx,yy=np.meshgrid(xbins,ybins)
histBins=np.linspace(-500,500,numOfBins-1)
xx,yy=np.meshgrid(histBins,histBins)
xdata = np.vstack((xx.ravel(), yy.ravel()))

fig = plt.figure()
ax = fig.gca(projection='3d')
#ax.plot_surface(xx, yy, Z, cmap='plasma')
#plt.show()

supList=[]
#for xbinIndex in xbins:
for xbinIndex in range(numOfBins-1):
    subList=[]
    print("------------------------------")
    #for ybinIndex in ybins:
    for ybinIndex in range(numOfBins-1):
        subList.append(hist[xbinIndex,ybinIndex])
        #print(hist[xbinIndex,ybinIndex])
    supList.append(subList)


Z=np.array(supList)

print("Shape of xx : "+str(xx.shape))
print("Shape of yy : "+str(yy.shape))
print("Shape of Z : "+str(Z.shape))
ax.plot_surface(xx, yy, Z, cmap='plasma',antialiased=False)
#ax.plot_surface(xx, yy, Z, cmap='summer', rstride=1, cstride=1, alpha=None, antialiased=False)
#print(xbins,ybins)




#Trying fitting stuff
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++
gcenx1=-125
gceny1=-125
gcenx2=-125
gceny2=125
gcenx3=125
gceny3=-125
gcenx4=125
gceny4=125

gsigmax1=50
gsigmay1=50
gamp1=20

gsigmax2=35
gsigmay2=59
gamp2=15

gsigmax3=10
gsigmay3=25
gamp3=5

gsigmax4=60
gsigmay4=30
gamp4=8

gparamList=[gcenx1,gceny1,gsigmax1,gsigmay1,gamp1, gcenx2,gceny2,gsigmax2,gsigmay2,gamp2, gcenx3,gceny3,gsigmax3,gsigmay3,gamp3, gcenx4,gceny4,gsigmax4,gsigmay4,gamp4 ]


#gsigmax2=1
#gsigmay2=1
#gamp2=1

#popt_2gauss, pcov_2gauss= curve_fit(_2gaussian,X,Znoise,p0=[gcen1,gsigma1,gamp1,gcen2,gsigma2,gamp2])
#popt_2gauss, pcov_2gauss= curve_fit(FitHelpers._gaussian,xdata,Z.ravel(),p0=[gcenx1,gceny1,gsigmax1,gsigmay1,gamp1,gcenx2,gceny2,gsigmax2,gsigmay2,gamp2])
popt_2gauss, pcov_2gauss= curve_fit(FitHelpers._gaussian,xdata,Z.ravel(),p0=gparamList)
perr_2gauss = np.sqrt(np.diag(pcov_2gauss))
pars_1 = popt_2gauss[0:5]
pars_2 = popt_2gauss[5:10]
pars_3 = popt_2gauss[10:15]
pars_4 = popt_2gauss[15:20]

#gauss_peak_1 = gaussian(X, *pars_1)
#gauss_peak_2 = gaussian(X, *pars_2)
print("----------------- Fitted Parameters --------------")
print(pars_1)
print(pars_2)
print(pars_3)
print(pars_4)

fig2 = plt.figure()
ax2 = fig2.gca(projection='3d')

Zfit=gaussianData(xx,yy,*popt_2gauss)
ax2.plot_surface(xx, yy, Zfit, cmap='plasma')
plt.show()
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#plt.show()
