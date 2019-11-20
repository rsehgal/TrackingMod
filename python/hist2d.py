import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

np.random.seed(1)

x = np.random.poisson(size=(160))
y = np.random.poisson(size=(160))

#fig, ax = plt.subplots()
#ax.set_aspect("equal")
hist, xbins, ybins, im = plt.hist2d(x,y, bins=range(6))
xbins=xbins[0:len(xbins)-1]
ybins=ybins[0:len(ybins)-1]
xx,yy=np.meshgrid(xbins,ybins)

fig = plt.figure()
ax = fig.gca(projection='3d')
#ax.plot_surface(xx, yy, Z, cmap='plasma')
#plt.show()

supList=[]
for xbinIndex in xbins:
    subList=[]
    print("------------------------------")
    for ybinIndex in ybins:
        subList.append(hist[xbinIndex,ybinIndex])
        print(hist[xbinIndex,ybinIndex])
    supList.append(subList)

Z=np.array(supList)
ax.plot_surface(xx, yy, Z, cmap='plasma')
#print(xbins,ybins)
plt.show()
