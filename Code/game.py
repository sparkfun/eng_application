import pygame, sys
import threading
from pygame.locals import *

#initialize pygame
pygame.init()

#declare frames per second
FPS = 30
fpsClock = pygame.time.Clock()

#Display Settings
DISPLAYSURF = pygame.display.set_mode((500,500))
pygame.display.set_caption('Kristofer Klipfel Game')
BLACK = (  0,   0,   0)

#Character Locations
direction = 'right'
direction2 = 'left'
direction3 = 'right'
direction4 = 'left'
pacX = 10
pacY = 10
ghostX = 10
ghostY = 100
ghost2X = 300
ghost2Y = 400
ghost3X = 100
ghost3Y = 300
ghost4X = 200
ghost4Y = 200

#Load Images
pacImg = pygame.image.load('Pacman.png')
cherryImg = pygame.image.load('4.png')
ghostImg = pygame.image.load('ghost.png')
winImg = pygame.image.load('win.png')

#Frame Background Refresh
def background():
	DISPLAYSURF.fill(BLACK)

#Display image on Win
def win():
	DISPLAYSURF.blit(winImg, (1, 240))

#Pac Location
def pacman():
	DISPLAYSURF.blit(pacImg, (pacX, pacY))

#Ghost Locations
def ghosts():
	DISPLAYSURF.blit(ghostImg, (ghostX, ghostY))
	DISPLAYSURF.blit(ghostImg, (ghost2X, ghost2Y))
	DISPLAYSURF.blit(ghostImg, (ghost3X, ghost3Y))
	DISPLAYSURF.blit(ghostImg, (ghost4X, ghost4Y))

#Cherry Location
def cherry():
	DISPLAYSURF.blit(cherryImg, (240, 450))

#Loop Function
while True:
	background()
	pacman()
	ghosts()
	cherry()
	
	#Update Ghost Locations
	if direction == "right":
		ghostX += 5
		if ghostX > 450:
			direction = "left"
	if direction2 == "right":
		ghost2X += 10
		if ghost2X > 450:
			direction2 = "left"
	if direction3 == "right":
		ghost3X += 5
		if ghost3X > 450:
			direction3 = "left"
	if direction4 == "right":
		ghost4X += 10
		if ghost4X > 450:
			direction4 = "left"
	if direction == "left":
		ghostX -= 5
		if ghostX < 10:
			direction = "right"
	if direction2 == "left":
		ghost2X -= 10
		if ghost2X < 10:
			direction2 = "right"
	if direction3 == "left":
		ghost3X -= 5
		if ghost3X < 10:
			direction3 = "right"
	if direction4 == "left":
		ghost4X -= 10
		if ghost4X < 10:
			direction4 = "right"

	#Keyboard button inputs		
	events = pygame.event.get()
	for event in events:
	    if event.type == pygame.KEYDOWN:
	        if event.key == pygame.K_LEFT:
	        	if pacX >= 10:
	            		pacX -= 10
	            		
	        if event.key == pygame.K_RIGHT:
	            if pacX <= 450:
	            		pacX += 10
	            		
	        if event.key == pygame.K_UP:
	        	if pacY >= 10:
	            		pacY -= 10
	            		
	        if event.key == pygame.K_DOWN:
	            if pacY <= 450:
	            		pacY += 10	
	        
	#Win Condition            		
	if pacY == 460 and pacX == 240:
		background()
		win()		

	#Reset if hit by ghost
	if pacX == ghostX and (pacY == ghostY or pacY == ghostY+10 or pacY == ghostY -10 or pacY == ghostY+20 or pacY == ghostY -20):
		pacX = 10
		pacY = 10
	if pacX == ghost2X and (pacY == ghost2Y or pacY == ghost2Y+10 or pacY == ghost2Y -10 or pacY == ghost2Y+20 or pacY == ghost2Y -20):
		pacX = 10
		pacY = 10
	if pacX == ghost3X and (pacY == ghost3Y or pacY == ghost3Y+10 or pacY == ghost3Y -10 or pacY == ghost3Y+20 or pacY == ghost3Y -20):
		pacX = 10
		pacY = 10
	if pacX == ghost4X and (pacY == ghost4Y or pacY == ghost4Y+10 or pacY == ghost4Y -10 or pacY == ghost4Y+20 or pacY == ghost4Y -20):
		pacX = 10
		pacY = 10

	#Update display every frame
	pygame.display.update()
	fpsClock.tick(FPS)