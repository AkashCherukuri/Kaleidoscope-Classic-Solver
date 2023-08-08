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

# def parse_file():
#     # Read inp.txt and get the codes
#     str_grid = []
#     with open("inp.txt") as file:
#         lines = file.readlines()
#         for line in lines:
#             str_grid.append(line.strip().split(","))
    
#     return np.array(str_grid)

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
def preprocess(board, pts2):
	# Preprocessing the image
	gray = cv2.cvtColor(board, cv2.COLOR_BGR2GRAY)
	blur = cv2.GaussianBlur(gray, (5, 5), 5, sigmaY=5)
	thresh = cv2.adaptiveThreshold(blur, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
			 cv2.THRESH_BINARY_INV, 15, 4)

	cv2.imshow("thresh", thresh)
	cv2.waitKey(0)
	
	# Detecting the contours
	try:
		contours,_ = cv2.findContours(thresh, cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)
		# print("found countour")
		cnt = sorted(contours, key=lambda x: cv2.contourArea(x), reverse = True)[0]
		
		# print("sort contour")
		a = cv2.approxPolyDP(cnt,0.01*cv2.arcLength(cnt,True),True)
		maximum = board.copy()

		cv2.drawContours(maximum, cnt, -1, (255,255,255), 10)
		# print("show")
		cv2.imshow("img", maximum)
		cv2.waitKey(0)
		if(len(a)!=4):	
			return pts2, thresh
	except:
		# print("aaa")
		return pts2, thresh
	
	# Rearranging the points
	a = a.reshape(-1, 2)
	c = np.argsort(np.sum(a, axis = 1))
	if(a[c[1]][0] < a[c[2]][0]):
		c[2],c[1] = c[1],c[2]
	c[2], c[3]=c[3], c[2]

	return (np.float32(a[c]), thresh)

def color_from_px(px):
	if max(px) < 80:
		return "BLA"	# Black
	elif px[0] == max(px):
		return "BLU"	# Blue
	elif abs(float(px[1])-float(px[2])) < 80:
		return "YEL"
	else:
		return "RED"
	

size = 28

pts2 = np.float32([[0,0],[size*8,0],[size*8,size*8],[0,size*8]])
board = cv2.imread("inp3.png")
pts1, thresh = preprocess(board, pts2)
M = cv2.getPerspectiveTransform(pts1, pts2)
board_cropped = cv2.warpPerspective(board, M, (size*8,size*8))

boardFound = False
if not (pts1==pts2).all():
	boardFound = True

if boardFound:
	color_grid = []
	st = size//2
	for i in range(8):
		tmp = []
		for j in range(8):
			tmp.append(str(color_from_px(board_cropped[st+i*size][st+j*size])))
		color_grid.append(tmp)
	

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
else:
	print("Board not found!")