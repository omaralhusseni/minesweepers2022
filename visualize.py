from calibrate import calibrate
import pygame
import sys
import csrt

BLACK = (0, 0, 0)
WHITE = (200, 200, 200)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
GREY = (104, 101, 115)
WINDOW_WIDTH, WINDOW_HEIGHT = 1920, 1080
CONNECT_TO_ROBOT = False
CAMERA_IP = "http://192.168.214.229:8080/video"

if CONNECT_TO_ROBOT:
    # next create a socket object
    import socket
    host = socket.gethostbyname(socket.gethostname())
    port = 5054
    print("current host ip: ", host)


def main():
    
    if CONNECT_TO_ROBOT:
        #start connection to esp
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as Socket:
            Socket.bind((host, port))
            print("Server started on:", host, port)
            Socket.listen()
            print ("socket is listening")  
            conn, addr = Socket.accept()
            print('Connected to', addr)

    global SCREEN, CLOCK
    pygame.init()
    SCREEN = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    CLOCK = pygame.time.Clock()
    SCREEN.fill(WHITE)
    points = calibrate(CAMERA_IP)

    csrt.set_ip(CAMERA_IP)
    csrt.init_warped(points)

    # for coordinates
    pygame.font.init()
    font = pygame.font.SysFont('Comic Sans MS', 30)



    while True:
        if CONNECT_TO_ROBOT:
            data = conn.recv(1024)
            print(data)


        # draw live video feed
        frame, x, y, w, h = csrt.get_frame_warped(points)
        pg_img = pygame.surfarray.make_surface(frame)
        pg_img = pygame.transform.scale(pg_img, (600, 400))
        SCREEN.blit(pg_img, (900, 120))

        # draw grid
        drawGrid((x, y))

        # draw coords
        font_hider = pygame.Rect(200, 800, 200, 45)
        pygame.draw.rect(SCREEN, WHITE, font_hider)
        coords_surface = font.render('X' + str(x) + 'Y'+ str(y), False, BLACK)
        SCREEN.blit(coords_surface, (200, 800))
        


        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        pygame.display.update()


def drawGrid(pos):
    x, y = pos
    blockSize = 35  # Set the size of the grid block

    grid_pos_x = (x//WINDOW_WIDTH) * 19
    grid_pos_y = (y//WINDOW_HEIGHT) * 19

    for y in range(0, 19):
        for x in range(0, 19):
            rect = pygame.Rect((blockSize*y)+200,
                               (blockSize*x)+120, blockSize, blockSize)

            if (grid_pos_x == x) & (grid_pos_y == y):
                pygame.draw.rect(SCREEN, RED, rect)
            else:
                pygame.draw.rect(SCREEN, GREY, rect, 1)


if __name__ == '__main__':
    main()
