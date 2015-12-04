from fruit import *
# definition of fit function
def func(x, mean, sigma, p0, f) :
    alpha = -0.00018*sigma**2 + 0.011*sigma + 1.975
    return f*crystal_ball(x, mean, sigma, alpha, 1) + (1-f)*exp(x/p0)

dat = data("data.root")

m0 = pdg.psi2s.mass/pdg.MeV
# definition of random variable
var = ["psi_MM"]
# definition of parameters
par = ["mean", m0, m0-10, m0+10,
       "sigma", 13, 5, 20,
       "p0", 0, -2, 2,
       "f", 0.8, 0, 1,
       ]
# fitting
result = fit(dat, func, var, par)
result.save_as("result.py")
