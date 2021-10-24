import numpy as np
import matplotlib.pyplot as plt
from numpy.core.fromnumeric import ndim

frametimes = np.linspace(0, 3000, 3000)

[tau, t, r, theta, phi, vt, vr, vtheta, vphi] = np.loadtxt("orbit.txt").transpose()
tau = tau - 3
frames = np.zeros((0, 8))
for time in frametimes:
    i = 0
    while (tau[i + 1] < time):
        i += 1
    smallerTau = tau[i]
    biggerTau = tau[i + 1]
    s = (time - smallerTau) / (biggerTau - smallerTau)
    smallerState = np.array([t[i], r[i], theta[i], phi[i], vt[i], vr[i], vtheta[i], vphi[i]], ndmin=2,)
    biggerState = np.array([t[i + 1], r[i + 1], theta[i + 1], phi[i + 1], vt[i + 1], vr[i + 1], vtheta[i + 1], vphi[i + 1]], ndmin=2)
    state = smallerState + s * (biggerState - smallerState)
    frames = np.append(frames, state, axis=0)

np.savetxt("../src/frames.txt", frames)