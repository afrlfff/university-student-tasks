import numpy as np

def grsearch(f, df, x0, tol):
    al = 0.01
    deltaX = -1
    x = x0
    k = 0
    kmax = 1000
    neval = 0
    coords = [x0]

    while True:
        grad = df(x)
        x = x + al * (-grad); neval += 1
        deltaX = al * (-grad)
        coords.append(x)
        k += 1

        if not ((np.linalg.norm(deltaX) >= tol) and (k < kmax)):
            break
    
    xmin = x
    fmin = f(xmin); neval += 1

    return xmin, fmin, neval, coords