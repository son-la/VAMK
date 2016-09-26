# Q3 - Crying face

import turtle as tt
from time import sleep

wd = tt.Screen()

face = tt.Turtle()

#face
face.circle(100)

#Left eye
face.penup()
face.goto(-40,60)
face.pendown()
face.circle(30)

#Right eye
face.penup()
face.goto(40,60)
face.pendown()
face.circle(30)

#Mounth
face.penup()
face.goto(-20,30)
face.pendown()
face.goto(0,40)
face.goto(20,30)

#Nose
face.penup()
face.goto(0,70)
face.pendown()
face.goto(-5,50)
face.goto(0,45)
face.goto(5,50)
face.goto(0,70)
face.goto(0,45)

#Move turtle
face.penup()
face.goto(-200,200)

#Tear

tear = tt.Turtle()
tear.hideturtle()
tear.penup()
tear.shape("circle")
tear.shapesize(0.5,0.5,0.5)
tear.color("blue")

i = 1;
while i < 10:
    tear.goto(50,60 - i*5)
    tear.showturtle()
    sleep(0.1)
    tear.hideturtle()
    i = i + 1
    if i == 8:
        i = 1
    
