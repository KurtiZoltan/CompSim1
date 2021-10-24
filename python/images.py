import numpy as np
import matplotlib.pyplot as plt

M = 1
Rs = 2 * M

#planet image

[tau, t, r, theta, phi, vt, vr, vtheta, vphi] = np.loadtxt("orbit.txt").transpose()
#orbit
x = r * np.cos(phi)
y = r * np.sin(phi)
blackholephi = np.linspace(0, 2 * np.pi, 100)
blackholex = Rs * np.cos(blackholephi)
blackholey = Rs * np.sin(blackholephi)
plt.plot(blackholex, blackholey, "black", label="Black hole")
plt.plot(x, y, label="Planet orbit")
plt.plot(x[0], y[0], "ro", label="Starting position")
plt.axis("equal")
plt.xlabel("$x$")
plt.ylabel("$y$")
plt.grid()
plt.legend()
plt.savefig("../doc/figs/planetOrbit.pdf")
plt.show()
#squared
vsquared = vt**2 * (1 - Rs / r) - vr**2 / (1 - Rs / r) - vtheta**2 * r**2 - vphi**2 * r**2 * np.sin(theta)**2
plt.plot(vsquared)
plt.grid()
plt.xlabel("# of timesteps")
plt.ylabel("$v^2$")
plt.savefig("../doc/figs/planetError.pdf")
plt.show()
#stepsize
plt.plot(tau[1:] - tau[0:-1], label="Stepsize")
plt.plot(r[1:], label="Coordinate $r$")
plt.grid()
plt.xlabel("# of timesteps")
plt.ylabel("stepsize")
plt.legend()
plt.savefig("../doc/figs/planetStepsize.pdf")
plt.show()