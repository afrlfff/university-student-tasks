import numpy as np

def bsearch(f, df, interval, tol):
    a = interval[0]
    b = interval[1]
    coords = []
    x_k = -1
    g = df(a)
    neval = 1

    while ((np.abs(b - a) > tol) and (np.abs(g) > tol)):
        x_k = (a + b) / 2
        coords.append(x_k)
        df_k = df(x_k); neval += 2

        if df_k > 0:
            b = x_k
        else:
            a = x_k
            g = df_k

    xmin = x_k
    fmin = f(x_k); neval += 1
    return xmin, fmin, neval, coords