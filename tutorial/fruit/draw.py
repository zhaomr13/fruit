from .result import *

# definition of fit function
def sig(x):
    alpha = -0.00018*sigma**2 + 0.011*sigma + 1.975
    return A * crystal_ball(x, mean, sigma, alpha, 1)

def bkg(x):
    return A * exp(x/p0)

def f(x):
    return f*sig(x) + (1-f)*bkg(x)

dat = data("data.root")

m0 = pdg.psi2s.mass/pdg.MeV
c = canvas(m0-10, m0+10)
draw(dat, c)
draw(dat, f)
draw(dat, sig)
draw(dat, bkg)

c.save_as("result.png")
