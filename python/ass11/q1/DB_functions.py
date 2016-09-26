import sqlite3

def CreateDb():
    con = sqlite3.connect('product.db')
    cur = con.cursor()

    cur.execute('CREATE TABLE IF NOT EXISTS product (id INTEGER PRIMARY KEY, name text, amount int, price float)')

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


    def insert(self,name,amount,price):
        query = "INSERT INTO " + str(self.__table_name)  + " (name,amount,price) VALUES (\'"+ str(name)+"\',"+str(amount)+","+str(price)+")"
        self.__cur.execute(query)
        self.__con.commit()

    def select_all(self):
        query = "SELECT * FROM " + str(self.__table_name)
        self.__cur.execute(query)
        return self.__cur.fetchall()
    
        
    def search_name(self, name):
        query = "SELECT * FROM " + str(self.__table_name) + " WHERE name='" + str(name) + "\'"


        self.__cur.execute(query)
        return self.__cur.fetchall()


    def search_name_price(self, name, price):
        query = "SELECT * FROM " + str(self.__table_name) + " WHERE name=\'" + str(name) + "\' AND price = " + str(price)
        self.__cur.execute(query)
        return self.__cur.fetchall()


    def search_price(self, price):
        query = "SELECT * FROM " + str(self.__table_name) + " WHERE price=" + str(price)
        self.__cur.execute(query)
        return self.__cur.fetchall()



