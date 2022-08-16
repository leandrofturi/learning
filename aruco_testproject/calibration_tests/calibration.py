import numpy as np
import cv2 as cv
import glob

# termination criteria
criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 10, 0.001)

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((4*6,3), np.float32)
objp[:,:2] = np.mgrid[0:6,0:4].T.reshape(-1,2)

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

# python gen_pattern.py -o out.svg -r 7 -c 5 -T checkerboard -s 35 -u mm -w 216 -h 279

images = glob.glob('/dados/20220812_calib_hd-checkerboard/calib/*.png')

for fname in images:
    print(fname)
    img = cv.imread(fname)
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

    # Find the chess board corners
    ret, corners = cv.findChessboardCorners(gray, (6,4), cv.CALIB_CB_ADAPTIVE_THRESH+cv.CALIB_CB_NORMALIZE_IMAGE )

    # If found, add object points, image points (after refining them)
    if ret == True:
        objpoints.append(objp)

        corners2 = cv.cornerSubPix(gray,corners, (11,11), (-1,-1), criteria)
        imgpoints.append(corners)

        # Draw and display the corners
        cv.drawChessboardCorners(img, (6,4), corners2, ret);
        cv.imshow('img', img)
        cv.waitKey(500)

ret, mtx, dist, rvecs, tvecs = cv.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)
# [fx 0 cx; 0 fy cy; 0 0 1]
# [k1,k2,p1,p2,k3,k4,k5,k6,s1,s2,s3,s4,taux,tauy] of 4, 5, 8, 12 or 14 elements.

mean_error = 0
for i in range(len(objpoints)):
    imgpoints2, _ = cv.projectPoints(objpoints[i], rvecs[i], tvecs[i], mtx, dist)
    error = cv.norm(imgpoints[i], imgpoints2, cv.NORM_L2)/len(imgpoints2)
    mean_error += error

print( "total error: {}".format(mean_error/len(objpoints)) )

cv.destroyAllWindows()