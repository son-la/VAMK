def sl_sum(n_sum,n):
    if n <= 0:
        return n_sum
       
    n_sum = sl_sum(n_sum,n-1) + 1/(3*n + 2)
    return n_sum

n_sum = 0
print(sl_sum(n_sum,3))

    
