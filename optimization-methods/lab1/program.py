import numpy as np

# целевая функция
def f(x): return 2*(x**2) - 9*x - 31

# производная целевой функции
def df(x): return 4*x - 9

# метод дихотомии
def bsearch(interval, tol):
    a = interval[0]
    b = interval[1]
    coords = []
    x_k = -1
    g = df(a)
    neval = 1

    while ((np.abs(b - a) > tol) and (np.abs(g) > tol)):
        x_k = (a + b) / 2
        coords.append(x_k)
        df_k = df(x_k); neval += 1

        if df_k > 0:
            b = x_k
        else:
            a = x_k
            g = df_k

    xmin = x_k
    fmin = f(x_k); neval += 1
    return xmin, fmin, neval, coords

# Входные параметры
a = -2
b = 10

interval = [a, b] # поисковый интервал (массив из двух значений в порядке возрастания),
tol = 1e-10 # заданная точность.

answer_ = bsearch(interval, tol)

print(answer_)