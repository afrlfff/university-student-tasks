import numpy as np

# целевая функция
def f(x): return 2*(x**2) - 9*x - 31

# производная целевой функции
def df(x): return 4*x - 9

# метод золотого сечения
def gsearch(interval, tol):
    a = interval[0]
    b = interval[1]
    phi = (1 + np.sqrt(5)) / 2
    L = b - a
    x1 = b - L / phi
    x2 = a + L / phi
    f1 = f(x1) 
    f2 = f(x2)
    neval = 2
    coord = [[x1, x2, a, b]]

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
        
        coord.append([x1, x2, a, b])

    xmin = x1
    fmin = f1

    return xmin, fmin, neval, coord

# входные переменные
a = -2
b = 10

interval = [a, b] # поисковый интервал
tol = 1e-10 # заданная точность.

answer_ = gsearch(interval, tol)
print(answer_)