# Q4 - Random date time

import random
import locale
import time

locale.setlocale(locale.LC_ALL,'Finnish_Finland')#Set locale

#Convert milisecond to date time.
temp = time.gmtime(random.randrange(int(time.time()*1/3),int(time.time()*4/3)))
print(time.strftime('%A, %d %B %Y, %H:%M', temp))#Print with format
