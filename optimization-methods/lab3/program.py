import numpy as np

# целевая функция
def f(x): return 2*(x**2) - 9*x - 31

# производная целевой функции
def df(x): return 4*x - 9

# вторая производная целевой функции
def ddf(x): return 4

# метод Ньютона
def nsearch(x0, tol):
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

# входные переменные
x0 = -2
tol = 1e-10 # заданная точность.

answer_ = nsearch(-2, tol)
print(answer_)