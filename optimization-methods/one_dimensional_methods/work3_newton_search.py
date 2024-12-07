import numpy as np

def nsearch(f, df, ddf, x0, tol):
    x_k = x0
    f_k = f(x_k)
    df_k = df(x_k)
    neval = 2
    coords = [x_k]

    while (np.abs(df_k) > tol):
        df_k = df(x_k)
        x_k = x_k - df_k / ddf(x_k)
        f_k = f(x_k)
        neval += 3
        coords.append(x_k)
    
    xmin = x_k
    fmin = f_k

    return xmin, fmin, neval, coords