import datetime;

class Project:

    def __init__(self, name, budget, startdate, enddate):#Constructor
        self.__name = name
        self.__budget = budget
        self.__startdate = startdate
        self.__enddate = enddate
        
    def __del__(self):#Destructor
        pass

    def __str__(self):#Tostring
        return "Name: " + self.__name + " _  Budget: " + str(self.__budget) + " _  Start date: " + str(self.__startdate) + " _ End date: " + str(self.__enddate)


    def getInfo(self):
        print("Name: ",self.__name," _  Budget: ",self.__budget," _  Start date: ",self.__startdate," _ End date: ",self.__enddate)

    def search(self, name):
        if name == self.__name:
            self.getInfo()
        else:
            print("Project not found")


    def update(self, name, budget):
        if name == self.__name:
            self.__budget = budget
        else:
            print("Project not found")
    def compare(self,other):
        return self.__dict__ == other.__dict__


        

project1 = Project("P1",500,datetime.datetime.strptime('05/10/09', '%d/%m/%y'),datetime.datetime.strptime('05/10/09', '%d/%m/%y'))

project2 = Project("P2",500,datetime.datetime.strptime("01/01/13", "%d/%m/%y"),datetime.datetime.strptime("01/12/13", "%d/%m/%y"))

project3 = Project("P3",500,datetime.datetime.strptime("01/01/13", "%d/%m/%y"),datetime.datetime.strptime("01/12/13", "%d/%m/%y"))

project4 = Project("P3",500,datetime.datetime.strptime("01/01/13", "%d/%m/%y"),datetime.datetime.strptime("01/12/13", "%d/%m/%y"))

print("Get info:")
print(project1)
print(project2)
#project1.getInfo()
#project2.getInfo()

print("Search:")

project1.search("P2")
project1.search("P1")

print("Update: ")

project2.update("P2",2000)
project2.getInfo()

lst = []

lst.append(project1)
lst.append(project2)
lst.append(project3)
lst.append(project4)

print("Search: ")
for i in range(len(lst)-1):
    for j in range(i+1,len(lst)):
        if (lst[i].compare(lst[j])):
            lst[i].getInfo()
            lst[j].getInfo()
            print()
        

