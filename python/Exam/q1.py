import datetime

class Concert:
    
    @property
    def title(self):
        return self.__title

    @property
    def date(self):
        return self.__date

    @property
    def time(self):
        return self.__time

    @property
    def price(self):
        return self.__price


    def __init__(self,title,date,time,price):
        self.__title = title
        date = str(date).split(' ')
        self.__date = date[0]
        time = str(time).split(' ')
        self.__time = time[1]
        self.__price = price

        
    def __str__(self):
        return "Title: " + self.__title + " - Date: " + str(self.__date) + " - Time:  " + str(self.__time) + " - Price: " + str(self.__price)

    def compare(self, title):
        if title == self.__title:
            return self
        else:
            return "Not match"
            


#Main program:

c1 = Concert("Concert 1", datetime.datetime.strptime('05/10/09', '%d/%m/%y'), datetime.datetime.strptime('14:00', '%H:%M'),140)

c2 = Concert("Concert 2", datetime.datetime.strptime('05/10/09', '%d/%m/%y'), datetime.datetime.strptime('10:00', '%H:%M'),200)

c3 = Concert("Concert 3", datetime.datetime.strptime('05/10/09', '%d/%m/%y'), datetime.datetime.strptime('20:00', '%H:%M'),150)

print("Concert Information: ")
print(c1)
print(c2)
print(c3)
print()
print("Comparation: ")
print("Compare 1 and 3: ", end='')
print(c1.compare('Concert 3'))
print("Compare 1 and 1: ", end='')
print(c1.compare('Concert 1'))
