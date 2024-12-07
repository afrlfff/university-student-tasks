import numpy as np

def goldensectionsearch(f, interval, tol):
    a = interval[0]
    b = interval[1]
    phi = (1 + np.sqrt(5)) / 2
    L = b - a
    x1 = b - L / phi
    x2 = a + L / phi
    f1 = f(x1) 
    f2 = f(x2)
    neval = 2

    while (L > tol):
        if f1 < f2:
            b = x2
            L = b - a
            x2 = x1
            x1 = b - L / phi
            f2 = f1
            f1 = f(x1); neval += 1
        else:
            a = x1  
            L = b - a
            x1 = x2
            x2 = a + L / phi  
            f1 = f2
            f2 = f(x2); neval += 1

    xmin = x1

    return xmin, neval


def sdsearch(f, df, x0, tol):
    x = x0
    deltaX = tol + 1
    k = 0
    kmax = 1000
    neval = 0
    coords = [x0]

    while ((np.linalg.norm(deltaX) >= tol) and (k < kmax)):
        grad = df(x); neval += 1

        f1dim = lambda alpha : (f(x + alpha * (-grad)))
        al, neval_ = goldensectionsearch(f1dim, [0, 1], tol)
        neval += neval_ 

        deltaX = al * grad
        x -= deltaX
        coords.append(x)
        k += 1
    
    xmin = x
    fmin = f(xmin); neval += 1

    return xmin, fmin, neval, coords 