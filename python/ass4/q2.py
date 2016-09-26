# Q2 - Funny face

import turtle as tt
wd = tt.Screen()

face = tt.Turtle()

#face
face.circle(100)

#Left eye
face.penup()
face.goto(-50,60)
face.pendown()
face.circle(30)

#Right eye
face.penup()
face.goto(50,60)
face.pendown()
face.circle(30)

#Mounth
face.penup()
face.goto(-20,30)
face.pendown()
face.goto(0,20)
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
