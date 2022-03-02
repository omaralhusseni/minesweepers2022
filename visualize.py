from re import M
from calibrate import calibrate
import pygame
import sys
import csrt
import cv2

BLACK = (0, 0, 0)
WHITE = (200, 200, 200)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
GREY = (104, 101, 115)
GREEN = (0, 255, 0)
WINDOW_WIDTH, WINDOW_HEIGHT = 1920, 1080
CONNECT_TO_ROBOT = False
CAMERA_IP = "http://192.168.32.76:8080/video"
MAP_WIDTH = 14
MAP_HEIGHT = 9

if CONNECT_TO_ROBOT:
    # next create a socket object
    import socket
    host = socket.gethostbyname(socket.gethostname())
    port = 5054
    print("current host ip: ", host)


def main():
    if CONNECT_TO_ROBOT:
        # start connection to esp
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as Socket:
            Socket.bind((host, port))
            print("Server started on:", host, port)
            print("socket is listening")
            Socket.listen()
            conn, addr = Socket.accept()
            print('Connected to', addr)

    global SCREEN, CLOCK
    pygame.init()
    SCREEN = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    CLOCK = pygame.time.Clock()
    SCREEN.fill(WHITE)
    
    points = calibrate(CAMERA_IP)

    csrt.set_ip(CAMERA_IP)

    # crop frame    
    frame = csrt.get_frame_warped_no_crop(points)
    crop_map = csrt.init_crop(frame)
    csrt.init_warped(points, crop_map)

    # for coordinates
    pygame.font.init()
    font = pygame.font.SysFont('Comic Sans MS', 30)


    #updated by esp socket server
    mine_state = "none"

    while True:
        if CONNECT_TO_ROBOT:
            conn.sendall("*".encode())
            # receive data from esp
            data = conn.recv(1024)
            if (data.decode("utf-8") == "1"):
                mine_state = "1"
            elif (data.decode("utf-8") == "2"):
                mine_state = "2"
            elif (data.decode("utf-8") == "0"):
               mine_state = "0"

            print(data, mine_state)

        # draw live video feed
        frame, x, y, w, h = csrt.get_frame_warped(points, crop_map)
        frame = cv2.circle(frame, (x, y), 10, RED) #for debuging

        pg_img = pygame.surfarray.make_surface(frame)
        pg_img = pygame.transform.scale(pg_img, (600, 400))
        SCREEN.blit(pg_img, (950, 120))

        # draw grid
        drawGrid((x, y), frame.shape, mine_state)

        # draw coords
        font_hider = pygame.Rect(200, 850, 200, 45)
        pygame.draw.rect(SCREEN, WHITE, font_hider)
        coords_surface = font.render('X' + str(x) + 'Y' + str(y), False, BLACK)
        SCREEN.blit(coords_surface, (200, 850))

        # draw mine state
        font_hider = pygame.Rect(200, 900, 200, 45)
        pygame.draw.rect(SCREEN, WHITE, font_hider)
        state_surface = font.render("mine: " + mine_state, False, BLACK)
        SCREEN.blit(state_surface, (200, 900))

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        pygame.display.update()


mines = {}


def drawGrid(pos, shape, mines_state):
    screen_x, screen_y = pos
    blockSize = 35  # Set the size of the grid block

    grid_pos_x = round((screen_x/shape[0]) * MAP_HEIGHT)
    grid_pos_y = round((screen_y/shape[1]) * MAP_WIDTH)

    for y in range(0, MAP_WIDTH):
        for x in range(0, MAP_HEIGHT):
            rect = pygame.Rect((blockSize*y)+200,
                               (blockSize*x)+120, blockSize, blockSize)

            if (grid_pos_x == x) & (grid_pos_y == y):
                pygame.draw.rect(SCREEN, RED, rect)
            else:
                pygame.draw.rect(SCREEN, GREY, rect, 1)

            if (mines_state == 'up'):
                mines[(grid_pos_x, grid_pos_y)] = '1'
            elif (mines_state == 'down'):
                mines[(grid_pos_x, grid_pos_y)] = '2'
            else:
                mines[(grid_pos_x, grid_pos_y)] = '0'

            if mines[(grid_pos_x, grid_pos_y)] == '1':
                pygame.draw.rect(SCREEN, GREEN, rect)
            elif mines[(grid_pos_x, grid_pos_y)] == '2':
                pygame.draw.rect(SCREEN, BLUE, rect)


if __name__ == '__main__':
    main()
