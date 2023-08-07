import numpy as np
import subprocess
import os
import cv2

'''
PARAMETERS!
'''

BACKGROUND_SIZE = 500   # Size of the image

BORDER_COLOR = (0, 0.643, 0)  # Color in BGR 
BORDER_THICKNESS = 5
BORDER_PADDING = 0

PIECE_BORDER_THICKNESS = 3
PIECE_BORDER_COLOR = (0, 1, 0)

BLACK = (0.1, 0.1, 0.1)
YELLOW= (0, 1, 1)
BLUE  = (1, 0, 0)
RED   = (0, 0, 1)

def get_color(s):
    if s == "BLA":
        return BLACK
    elif s == "YEL":
        return YELLOW
    elif s == "BLU":
        return BLUE
    elif s == "RED":
        return RED

def parse_file():
    # Read inp.txt and get the codes
    str_grid = []
    with open("inp.txt") as file:
        lines = file.readlines()
        for line in lines:
            str_grid.append(line.strip().split(","))
    
    return np.array(str_grid)

def color_code(s):
    if s == "RED":
        return 1
    elif s == "BLA":
        return 2
    elif s == "BLU":
        return 3
    elif s == "YEL":
        return 4

def get_coded_grid(color_grid):
    arg = ""
    for i in range(8):
        for j in range(8):
            arg += str(color_code(color_grid[i][j]))
            arg += " "
    return arg

color_grid = parse_file()

# Compile the cpp file
if not os.path.exists("kal.exe"):
    subprocess.run("g++ kal.cpp -o kal.exe")

# Run the cpp file and get the output

exec_outp = subprocess.check_output("./kal.exe", input=get_coded_grid(color_grid).encode())
outp = exec_outp.decode().split()

# If solution was found, continue
if len(outp) == 65:
    # Draw the background
    img = np.zeros((BACKGROUND_SIZE, BACKGROUND_SIZE))
    img = cv2.merge([img,img,img])

    wd = (BACKGROUND_SIZE-BORDER_THICKNESS-2*BORDER_PADDING)//8
    st = BORDER_THICKNESS+BORDER_PADDING

    for i in range(8):
        for j in range(8):
            x = st+j*wd
            y = st+i*wd
            cv2.rectangle(img, (x,y), (x+wd, y+wd), get_color(color_grid[i][j]), -1)

    numb_grid = np.array(outp[1:], dtype=int).reshape((8,8))
    # print(numb_grid)

    # Draw borders for the pieces and all
    for i in range(8):
        for j in range(8):
            # check horizontally
            if j+1<8 and numb_grid[i][j] != numb_grid[i][j+1]:
                cv2.line(img, (st+wd*(j+1), st+wd*i), (st+wd*(j+1), st+wd*(i+1)), PIECE_BORDER_COLOR, PIECE_BORDER_THICKNESS)
            # check vertically
            if i+1<8 and numb_grid[i][j] != numb_grid[i+1][j]:
                cv2.line(img, (st+wd*j, st+wd*(i+1)), (st+wd*(j+1), st+wd*(i+1)), PIECE_BORDER_COLOR, PIECE_BORDER_THICKNESS)

    # Draw a border around the grid
    cv2.rectangle(img, (BORDER_THICKNESS+BORDER_PADDING, BORDER_THICKNESS+BORDER_PADDING), (BACKGROUND_SIZE-BORDER_THICKNESS-BORDER_PADDING, BACKGROUND_SIZE-BORDER_THICKNESS-BORDER_PADDING), BORDER_COLOR, BORDER_THICKNESS)


    cv2.imshow('image',img)
    cv2.waitKey(0)
else:
    print("Solution not found!")