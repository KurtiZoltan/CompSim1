import numpy as np
import matplotlib.pyplot as plt

M = 1
Rs = 2 * M

#planet image

[tau, t, r, theta, phi, vt, vr, vtheta, vphi] = np.loadtxt("planet.txt").transpose()
x = r * np.cos(phi)
y = r * np.sin(phi)

blackholephi = np.linspace(0, 2 * np.pi, 100)
blackholex = Rs * np.cos(blackholephi)
blackholey = Rs * np.sin(blackholephi)
plt.plot(blackholex, blackholey, "black")

plt.plot(x, y)
plt.axis("equal")
plt.xlabel("$x$")
plt.ylabel("$y$")
plt.grid()
plt.savefig("../doc/figs/planetOrbit.pdf")
plt.show()

vsquared = vt**2 * (1 - Rs / r) - vr**2 / (1 - Rs / r) - vtheta**2 * r**2 - vphi**2 * r**2 * np.sin(theta)**2
plt.plot(vsquared)
plt.grid()
plt.xlabel("# of timesteps")
plt.ylabel("$v^2$")
plt.savefig("../doc/figs/planetError.pdf")
plt.show()

plt.plot(tau[1:] - tau[0:-1])
plt.grid()
plt.xlabel("# of timesteps")
plt.ylabel("stepsize")
plt.savefig("../doc/figs/planetStepsize.pdf")
plt.show()