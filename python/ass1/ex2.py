#Assignment 1 - Q2

a = 0
b = 1
for i in range(1,10):
    print(a + b,' ',end='')
    a,b=b,a+b

print();

a = 0
b = 1
c = 1
print(1,' ',1,' ',end='')
for i in range(1,10):
    print(a + b + c,' ',end='')
    a,b,c=b,c,a+b+c

print();

a = 3;
b = 1;
for i in range(1,10):
    if i%2 == 0:
        print(a,' ',end='');
        b += a;
        a = a + 3;
    else:
        if b == 1:
            print(1,' ',end='');
        else:
            print(b,' ',end='');
            b+=b;
