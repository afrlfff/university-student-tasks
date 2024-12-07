import numpy as np

def ssearch(f, df, interval, tol):
    a_k = interval[0]
    b_k = interval[1]
    x_k = -1
    df_x = -1

    df_a = df(a_k)
    df_b = df(b_k)
    neval = 2
    coords = []

    while True:
        x_k = b_k - (df_b * (b_k - a_k)) / (df_b - df_a)
        df_x = df(x_k); neval += 1

        if not ((np.abs(df_x)) > tol) and ((np.abs(b_k - a_k)) > tol):
            break

        coords.append([x_k, a_k, b_k])

        if (df_x > 0):
            b_k = x_k
            df_b = df_x
        else:
            a_k = x_k
            df_a = df_x

    
    xmin = x_k
    fmin = f(x_k); neval += 1

    return xmin, fmin, neval, coords