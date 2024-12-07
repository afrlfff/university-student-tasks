import numpy as np

def H(x0, tol, df):
    deltaX = 0.1 * tol
    n = 2
    
    H = np.zeros((2, 2))

    for i in range(n):
        x_plus_h = np.copy(x0)
        x_minus_h = np.copy(x0)
        x_plus_h[i] += deltaX
        x_minus_h[i] -= deltaX

        grad_plus_h = df(x_plus_h)
        grad_minus_h = df(x_minus_h)

        grad = (grad_plus_h - grad_minus_h) / (2 * deltaX)

        for j in range(2):
            H[j, i] = grad[j]

    return H

def nsearch(f, df, x0, tol):
    x = x0
    deltaX = tol + 1
    kmax = 1000
    k = 0
    neval = 0
    coords = [x0]

    while (np.linalg.norm(deltaX) >= tol) and (k < kmax):
        h = H(x, tol, df); neval += 4

        r = np.linalg.lstsq(h, df(x), rcond=-1)[0]
        x = x - r
        deltaX = -r
        coords.append(x)
        neval += 1

        k += 1
    
    xmin = x
    fmin = f(x); neval += 1

    return xmin, fmin, neval, coords