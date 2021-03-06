import random

class Player:
    #static
    NumOfPlayers = 0
    
    #Ctor    
    def __init__(self,name,stake):
        self.__name = name
        self.__stake = stake
        self.__point = 0
        self.__status = 1

    
    @property
    def name(self):
        return self.__name
    @property
    def stake(self):
        return self.__stake
    @property
    def point(self):
        return self.__point

    @point.setter
    def point(self,p):
        self.__point = p



#Methods
    def __str__(self):
        return 'Name: ' + self.__name + '  Stake: ' + str(self.__stake) + ' Point: ' + str(self.__point)
    
    def play(self):
        if self.__status == -1:
            return -1
        print(self.__name,', Want to play? (y/n): ',end='')
        play = input()
        if play == 'y':
            self.__point = self.__point + random.randrange(1,13)
        if play != 'y' or self.__point > 21:
            Player.NumOfPlayers = Player.NumOfPlayers - 1
            self.__status = -1
            return -1
        else:
            return 1
            

        
class Game:
#Methods
    def start(self):

        self.__player = []
        self.__game_status = {}
        
        numOfPlayer = eval(input('Enter number of player: '))
        #Check input
        if numOfPlayer < 2:
         return 0

        Player.NumOfPlayers = numOfPlayer

        #Initialize players
        for i in range(numOfPlayer):
            print('\tPlayer ',i + 1,' name: ',end='')
            __name = input()
            print('\tPlayer ',i + 1,' stake: ',end='')
            __stake = eval(input())
            
            self.__game_status[__name] = 0
            self.__player.append(Player(__name,__stake))

        #Play
        i = 0
        while(1):
            if Player.NumOfPlayers == 0: #No one plays
                break
            elif(self.__player[i].play()):
                #Update point
                self.__game_status[self.__player[i].name] = self.__player[i].point

            #Update player status
            for n in range(numOfPlayer):
                print(self.__player[n])
                   
            i = i + 1
            if i == numOfPlayer:
                i = 0

        #Final result    
        print()
        print('Finale result:')
        
        for i in range(numOfPlayer):
            print(self.__player[i])

        print()
            
        temp_win = Player('temp',0)#Winner
        for i in range(numOfPlayer):
            temp = self.__game_status[self.__player[i].name]#Point
            if temp == 21:
                temp_win = self.__player[i]
                break
            elif temp > 21:
                continue
            elif temp > temp_win.point:
                temp_win = self.__player[i]
            else:
                continue

        print()

        #Winner
        if temp_win.name == 'temp':
            print('Due')
        else:
            print('Winner is: ')
            print(temp_win)
    
            
                    
#Main program
new_game = Game()
new_game.start()
