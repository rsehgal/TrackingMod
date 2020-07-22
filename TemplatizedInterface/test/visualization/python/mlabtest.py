import numpy as np
from mayavi import mlab

data = (100, 100, 100)
data = np.zeros(data)
data[0:50, 50:70, 0:50] = 1
data[0:50, 0:20, 0:50] = 1

src = mlab.pipeline.scalar_field(data)
outer = mlab.pipeline.iso_surface(src)

mlab.show()