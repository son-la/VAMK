import math as m;

def sum_of_sine(numbers):
    my_sum = 0
    for i in range(len(numbers)):
        my_sum = my_sum + m.sin(numbers[i])**2 + m.log(numbers[i],3)  + 1/(numbers[i]**5)

    print(my_sum)


sum_of_sine([1,2,3,4,5,6,7,8])

