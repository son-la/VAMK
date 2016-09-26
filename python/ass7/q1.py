def sl_sum(n,m):
    if m == 0:
        return n
    elif m > 0:
        n = sl_sum(n,m-1) + 1
    else:
        n = sl_sum(n,m+1) - 1
    return n

print(sl_sum(-37,-3))
