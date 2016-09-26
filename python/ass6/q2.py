import re

text = "name=Milk;amount=200;unit_price=0.9\nname=Bread;amount=134;unit_price=3.48\nname=Butter;amount=58;unit_price=1.65\nname=Cheese;amount=260;unit_price=4.35"

new = re.split("\s*;*=*",text)
for i in range(len(new)//6):
    print('{:8}{}'.format(new[i*6 + 1],float(new[i*6+3]) * float(new[i*6+5]))) 


