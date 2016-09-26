def sum_cal(string):
    string = string.split('\n');
    my_sum = 0;
    for i in range(len(string)):
        temp = string[i].replace('=',';').split(';')
        my_sum = my_sum + float(temp[3])*float(temp[5]);

    return my_sum

print(sum_cal('name=Milk;amount=200;unit_price=0.9\nname=Bread;amount=134;unit_price=3.48\nname=Butter;amount=58;unit_price=1.65\nname=Cheese;amount=260;unit_price=4.35'))
