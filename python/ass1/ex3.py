#Assignment 1 - Q3
import time;
#Sieve of Eratosthenes algorithm

start_time = time.time();
n = 10000;
a = [1]*n;
a[0] = 0;

k = 2;

while k <= n**1/2:
    for j in range(k,n+1):
        if a[j - 1] != 0:
            if j%k == 0 and j != k:
                a[j - 1] = 0;
    k += 1;

for k in range(0,10000):
    if a[k] != 0:
        print(str(k + 1));

print();   
print("--- %s seconds ---" % (time.time() - start_time))

