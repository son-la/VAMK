# Q1 - Falling ball

import turtle as tt
from time import sleep

h = 250;
g = 10;

ground = tt.Turtle()
ground.hideturtle()
ground.goto(-100,0)
ground.goto(100,0)
ground.penup()

ball = tt.Turtle()
ball.penup()
ball.goto(0,h)
ball.pendown()
ball.hideturtle()
ball.shape('circle')
ball.shapesize(2,2,0)
ball.color('red')
ball.showturtle()
ball.penup()

p = input('Enter command (\'f\' for fire, \'q\' for quit): ')
while p != 'f' and p != 'q':
    p = input('Enter command: ')
    
if p == 'f':
    while h > 25:
        h = h - g;
        ball.goto(0,h)
        ball.showturtle()
        sleep(0.1)
        ball.hideturtle()
        print('High: ',h-20)
elif p == 'q':
    pass

                        
    
    
    
