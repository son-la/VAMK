#Q5 - Table

name = ['Name','Apple','Orange','Peach','Passion']
unit_price = ['Unit Price','1.98','0.99','1.99','3.45']
amount = ['Amount','150','250','180','235']

for i in range(len(name)):
    print("{:<15}{:^10}{:>5}".format(name[i],unit_price[i],amount[i]))
