import mayavi.mlab
import numpy
import matplotlib.colors as colors

print(colors.colorConverter.to_rgb(1))
data = (100, 100, 100)
data = numpy.zeros(data)
data[0:50, 50:70, 0:50] = 1
data[0:50, 0:20, 0:50] = 2
data[60:61, 80:81, 60:61] = 3

col=1
xx, yy, zz = numpy.where(data == col)
mayavi.mlab.points3d(xx, yy, zz,
                     #mode="cube",
                     color=(1, 0, 0),
                     opacity=0.2,
                     scale_factor=1)


col=2
xx, yy, zz = numpy.where(data == col)
mayavi.mlab.points3d(xx, yy, zz,
                     mode="cube",
                     color=(0.65, 0.32, 0.38),
                     scale_factor=1)


col=3
xx, yy, zz = numpy.where(data == col)
mayavi.mlab.points3d(xx, yy, zz,
                     mode="cube",
                     color=(0.6, 0.2, 0.7),
                     scale_factor=1)

#mayavi.mlab.show()
