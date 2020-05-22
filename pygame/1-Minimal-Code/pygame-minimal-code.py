'''
Pygame intro tutorial. This covers the minimal amount of pygame code required 
to run a pygame program.

@dependencies
python 3
pygame

@author: Matt Marti
@date: 2019-03-23
'''

import pygame

#Main
def main():
    
    # Initialize pygame module
    pygame.init()
    
    # Load and set logo
    logoimage = pygame.image.load("heavy breathing - Imgur.jpg")
    pygame.display.set_icon(logoimage)
    pygame.display.set_caption("minimal program tutorial")
    
    # Create surface on screen
    screen = pygame.display.set_mode((528,528));
    
    # Loop keep window open
    mainwindowbool = True;
    while mainwindowbool:
        
        # Event handle from event queue
        for event in pygame.event.get():
            
            # Only act on event QUIT
            if event.type == pygame.QUIT:
                mainwindowbool = False;
    
# Run Main
if __name__ == "__main__":
    main()