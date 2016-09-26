import sqlite3

def CreateDb():
    con = sqlite3.connect('concert.db')
    cur = con.cursor()

    cur.execute('CREATE TABLE IF NOT EXISTS concert (id INTEGER PRIMARY KEY, title text, date text, time text, price float)')

    con.commit()
    con.close()

class DBConnection():
        
    def __init__(self,db_name, table_name):
        self.__db_name = db_name
        self.__table_name = table_name
        self.__con=sqlite3.connect(db_name)
        self.__cur = self.__con.cursor()

    def __del__(self):
        self.__con.close()

    def insert(self,title,date,time,price):
        query = "INSERT INTO " + str(self.__table_name)  + " (title,date,time,price) VALUES (\'"+ str(title)+"\',\'"+str(date)+"\',\'"+str(time)+"\',"+str(price)+")"
        self.__cur.execute(query)
        self.__con.commit()

    def select_all(self):
        query = "SELECT * FROM " + str(self.__table_name)
        self.__cur.execute(query)
        return self.__cur.fetchall()



