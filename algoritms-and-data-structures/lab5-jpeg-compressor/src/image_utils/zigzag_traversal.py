import numpy as np

def zigzag_traversal(A):
    height, width = A.shape
    if width != height:
        raise ValueError('zigzag traversal error: Matrix must be square')

    result = [A[0][0]]
    i = 0; j = 0
    while True:
        # move right or down
        if j + 1 < width:
            result.append(A[i][j + 1])
            j += 1
        elif i + 1 < height:
            result.append(A[i + 1][j])
            i += 1
        else:
            break
        
        # move left-down
        while (j - 1 >= 0) and (i + 1 < height):
            result.append(A[i + 1][j - 1])
            i += 1; j -= 1
        
        # move down or right 
        if i + 1 < height:
            result.append(A[i+1][j])
            i += 1
        elif j + 1 < width:
            result.append(A[i][j + 1])
            j += 1
        else:
            break

        # move right-up
        while (i - 1 >= 0) and (j + 1 < width):
            result.append(A[i - 1][j + 1])
            i -= 1; j += 1

    return result

def zigzag_traversal_inverse(traversal):
    width = height = int(np.sqrt(traversal.shape[0]))

    A = np.zeros((height, width))
    A[0][0] = traversal[0]
    i = 0; j = 0; k = 1
    while True:
        # move right or down
        if j + 1 < width:
            A[i][j + 1] = traversal[k]
            j += 1; k += 1
        elif i + 1 < height:
            A[i + 1][j] = traversal[k]
            i += 1; k += 1
        else:
            break
        
        # move left-down
        while (j - 1 >= 0) and (i + 1 < height):
            A[i + 1][j - 1] = traversal[k]
            i += 1; j -= 1; k += 1
        
        # move down or right 
        if i + 1 < height:
            A[i+1][j] = traversal[k]
            i += 1; k += 1
        elif j + 1 < width:
            A[i][j + 1] = traversal[k]
            j += 1; k += 1
        else:
            break

        # move right-up
        while (i - 1 >= 0) and (j + 1 < width):
            A[i - 1][j + 1] = traversal[k]
            i -= 1; j += 1; k += 1

    return A


#A = np.array([[0]])
#A = np.array([[1, 2], [3, 4]])
#A = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
#A = np.array([[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]])