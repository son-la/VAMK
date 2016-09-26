import functools

seq = [25,39,52,68,20,89]
fil = lambda x,y : x if x > y else y
print('Max is: ',functools.reduce(fil,seq))


