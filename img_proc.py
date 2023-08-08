import cv2
import numpy as np

def preprocess(board, pts2):
	# Preprocessing the image
	gray = cv2.cvtColor(board, cv2.COLOR_BGR2GRAY)
	blur = cv2.GaussianBlur(gray, (5, 5), 5, sigmaY=5)
	thresh = cv2.adaptiveThreshold(blur, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
			 cv2.THRESH_BINARY_INV, 11, 2)
	
	cv2.imshow("blur", blur)
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
		# cv2.imshow("img", maximum)
		# cv2.waitKey(0)
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
board = cv2.imread("inp2.png")
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
	print(color_grid)