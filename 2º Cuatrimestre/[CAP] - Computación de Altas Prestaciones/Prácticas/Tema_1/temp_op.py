import timeit
import numpy as np

def temp_op(N):
    # a, b and c are vectors of size N
    a = np.random.rand(N)
    b = np.random.rand(N)
    c = np.random.rand(N)

    result = np.zeros(N)

    beta = np.random.rand()

    start = timeit.default_timer()
    
    # RESULT = A + B + C * BETA
    result = a + b + c * beta
    
    end = timeit.default_timer()

    return end - start

def temp_op_manual_loop(N):
    # a, b and c are vectors of size N
    a = np.random.rand(N)
    b = np.random.rand(N)
    c = np.random.rand(N)

    result = np.zeros(N)

    beta = np.random.rand()

    start = timeit.default_timer()
    for i in range(N):
        result[i] = a[i] + b[i] + c[i] * beta
    end = timeit.default_timer()

    return end - start

N_values = [10, 1000, 1000000]

print("====================================================")
print("N \t| temp_op \t| temp_op_manual_loop")
print("----------------------------------------------------")
for N in N_values:
    time_temp_op = temp_op(N)
    time_temp_op_manual_loop = temp_op_manual_loop(N)
    print("{} \t| {:.6f} \t| {:.6f}".format(N, time_temp_op, time_temp_op_manual_loop))
print("====================================================")
