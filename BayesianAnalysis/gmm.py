import numpy as np
from sklearn import mixture
from matplotlib import pyplot as plt
np.random.seed(1)
g = mixture.GMM(n_components=2)
#g.weights_=[0.0,0.1]
print(g.weights_)
#print(g.means_)
obs = np.concatenate((np.random.randn(10, 1),10 + np.random.randn(10, 1)))
g.verbose = 1 
#g.n_iter = 100 
#g.tol = 1e-15
g.fit(obs)
print("numOfIter :" + str(g.n_iter))
print(g.weights_)
print(g.means_)
print(g.covars_)
bins = np.linspace(-10, 14, 100)
plt.hist(obs,bins,alpha=.60)
plt.show()
