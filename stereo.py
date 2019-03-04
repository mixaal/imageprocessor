import numpy as np
import cv2
from matplotlib import pyplot as plt
import sys


class Rect(object):
    def __init__(self,xs, ys, xe, ye):
        self.xs=xs;
        self.ys=ys;
        self.xe=xe;
        self.ye=ye;


class StereoLoader(object):
    def __init__(self, left_image, right_image, width=1920, height=1080):
        self.dst1_left = cv2.imread(left_image, 0)
        self.dst2_right = cv2.imread(right_image, 0)
        if (width<0 and height<0):
            (height, width) = self.dst1_left.shape
            self.dst1_left_for_matcher = self.dst1_left
            self.dst2_right_for_matcher = self.dst2_right
        else:    
            self.dst1_left_for_matcher = cv2.resize(self.dst1_left, (width, height))
            self.dst2_right_for_matcher = cv2.resize(self.dst2_right, (width, height))
        self.im_width = width
        self.im_height = height

    def left_matcher(self):
        return self.dst1_left_for_matcher

    def right_matcher(self):
        return self.dst2_right_for_matcher

    def left_orig(self):
        return self.dst1_left

    def right_orig(self):
        return self.dst2_right

    def debug(self):
        cv2.imwrite('dst1.jpg', self.dst1_left_for_matcher)
        cv2.imwrite('dst2.jpg', self.dst2_right_for_matcher)


class FeatureFinder(object):
    def __init__(self, left_matcher, right_matcher, left_orig, right_orig):
        self.sift = cv2.xfeatures2d.SIFT_create()
        self.left_matcher = left_matcher
        self.right_matcher = right_matcher
        self.left_orig = left_orig
        self.right_orig = right_orig

    def detect(self):
        ###find the keypoints and descriptors with SIFT
        kp1, des1 = self.sift.detectAndCompute(self.left_matcher,None)
        kp2, des2 = self.sift.detectAndCompute(self.right_matcher,None)
        self.kp1 = kp1
        self.kp2 = kp2
        self.des1 = des1
        self.des2 = des2

    def key_points_left(self):
        return self.kp1

    def key_points_right(self):
        return self.kp2

    def descriptor_left(self):
        return self.des1

    def descriptor_right(self):
        return self.des2

    def debug(self):
        dbg1 = cv2.drawKeypoints(self.left_matcher,self.kp1,self.left_orig)
        dbg2 = cv2.drawKeypoints(self.right_matcher,self.kp2,self.right_orig)
        cv2.imwrite('sift1.jpg', dbg1)
        cv2.imwrite('sift2.jpg', dbg2)


class EpipolarGeometry(object):
    def __init__(self, des_left, des_right, kp_left, kp_right):
        self.des1 = des_left
        self.des2 = des_right
        self.kp1 = kp_left
        self.kp2 = kp_right
        ###FLANN parameters
        FLANN_INDEX_KDTREE = 0
        index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
        search_params = dict(checks=50)
        self.flann = cv2.FlannBasedMatcher(index_params,search_params)
        self.pts1 = []
        self.pts2 = []
        self.gdraw = []

    def compute_fundamental_matrix(self):
        matches = self.flann.knnMatch(self.des1,self.des2,k=2)
        ###ratio test as per Lowe's paper
        for i,(m,n) in enumerate(matches):
            if m.distance < 0.8*n.distance:
                self.gdraw.append([m])
                self.pts2.append(self.kp2[m.trainIdx].pt)
                self.pts1.append(self.kp1[m.queryIdx].pt)

        self.pts1 = np.array(self.pts1)
        self.pts2 = np.array(self.pts2)
        F,mask= cv2.findFundamentalMat(self.pts1,self.pts2,cv2.FM_LMEDS)
        self.F = F
        print "F={}".format(F)
        self.mask = mask

    def left_matches(self):
        return self.pts1

    def right_matches(self):
        return self.pts2

    def get_fundamental(self):
        return self.F

    def get_mask(self):
        return self.mask

    def debug_matches(self, left_matcher, right_matcher):
        # Draw first 40 matches.
        img_match_kp = cv2.drawMatchesKnn(left_matcher,self.kp1,right_matcher,self.kp2,self.gdraw,None,flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
        #plt.imshow(img_match_kp),plt.show()
        cv2.imwrite('keypoits-match.jpg', img_match_kp)

    def debug_epipolar(self, left_matcher, right_matcher):
        # Find epilines corresponding to points in right image (second image) and
        # drawing its lines on left image

        pts1 = np.int32(self.pts1)
        pts2 = np.int32(self.pts2)
        lines1 = cv2.computeCorrespondEpilines(pts2.reshape(-1,1,2), 2,self.F)
        lines1 = lines1.reshape(-1,3)
        img5,img6 = self._drawlines(left_matcher,right_matcher,lines1,pts1,pts2)

        # Find epilines corresponding to points in left image (first image) and
        # drawing its lines on right image
        lines2 = cv2.computeCorrespondEpilines(pts1.reshape(-1,1,2), 1,self.F)
        lines2 = lines2.reshape(-1,3)
        img3,img4 = self._drawlines(right_matcher, left_matcher,lines2,pts2,pts1)

        cv2.imwrite('epipolar-left.jpg', img5)
        cv2.imwrite('epipolar-right.jpg', img3)

    def _drawlines(self,img1,img2,lines,pts1,pts2):
        ''' img1 - image on which we draw the epilines for the points in img2
            lines - corresponding epilines '''
        r,c = img1.shape
        img1 = cv2.cvtColor(img1,cv2.COLOR_GRAY2BGR)
        img2 = cv2.cvtColor(img2,cv2.COLOR_GRAY2BGR)
        for r,pt1,pt2 in zip(lines,pts1,pts2):
            color = tuple(np.random.randint(0,255,3).tolist())
            x0,y0 = map(int, [0, -r[2]/r[1] ])
            x1,y1 = map(int, [c, -(r[2]+r[0]*c)/r[1] ])
            img1 = cv2.line(img1, (x0,y0), (x1,y1), color,1)
            img1 = cv2.circle(img1,tuple(pt1),5,color,-1)
            img2 = cv2.circle(img2,tuple(pt2),5,color,-1)
        return img1,img2


class PerspectiveTransform(object):
    def __init__(self, left_matches, right_matches, mask, F, left_matcher, right_matcher):
        self.pts1=left_matches
        self.pts2=right_matches
        self.mask=mask
        self.F = F
        self.left_matcher = left_matcher
        self.right_matcher = right_matcher

    def warp_perspective(self):
        # Obtainment of the rectification matrix and use of the warpPerspective to transform them...
        pts1 = self.pts1[:,:][self.mask.ravel()==1]
        pts2 = self.pts2[:,:][self.mask.ravel()==1]

        pts1 = np.int32(pts1)
        pts2 = np.int32(pts2)

        p1fNew = pts1.reshape((pts1.shape[0] * 2, 1))
        p2fNew = pts2.reshape((pts2.shape[0] * 2, 1))

        (im_height, im_width)=self.left_matcher.shape
        print "im_height: {}, im_width={}".format(im_height, im_width)

        retBool ,rectmat1, rectmat2 = cv2.stereoRectifyUncalibrated(p1fNew,p2fNew,self.F,(im_width, im_height))

        self.left_matcher_perspective= cv2.warpPerspective(self.left_matcher,rectmat1,(im_width, im_height))
        self.right_matcher_perspective = cv2.warpPerspective(self.right_matcher,rectmat2,(im_width, im_height))

    def get_left_matcher_perspective(self):
        return self.left_matcher_perspective

    def get_right_matcher_perspective(self):
        return self.right_matcher_perspective

    def debug(self):
        cv2.imwrite('dst11.jpg', self.left_matcher_perspective)
        cv2.imwrite('dst22.jpg', self.right_matcher_perspective)


class DepthFinder(object):
    def __init__(self, left_matcher_perspective, right_matcher_perspective, lmbda=8000, sigma=1.5, wsize=7):
        self.left_matcher_perspective = left_matcher_perspective
        self.right_matcher_perspective = right_matcher_perspective
        #self.stereo_left = cv2.StereoBM_create(numDisparities=160, blockSize=63)
        #self.stereo_left.setPreFilterSize(5)
        #self.stereo_left.setPreFilterCap(1)
        #self.stereo_left.setTextureThreshold(0)
        #self.stereo_left.setUniquenessRatio(0)
        self.stereo_left = cv2.StereoSGBM_create(
            minDisparity=0,
            numDisparities=160,             # max_disp has to be dividable by 16 f. E. HH 192, 256
            blockSize=wsize,
            P1=8 * 3 * wsize ** 2,    # wsize default 3; 5; 7 for SGBM reduced size image; 15 for SGBM full size image (1300px and above); 5 Works nicely
            P2=32 * 3 * wsize ** 2,
            disp12MaxDiff=1,
            uniquenessRatio=15,
            speckleWindowSize=0,
            speckleRange=2,
            preFilterCap=63,
            mode=cv2.STEREO_SGBM_MODE_SGBM_3WAY
        )
        self.stereo_right = cv2.ximgproc.createRightMatcher(self.stereo_left)
        self.wls_filter = cv2.ximgproc.createDisparityWLSFilter(matcher_left=self.stereo_left)
        self.wls_filter.setLambda(lmbda)
        self.wls_filter.setSigmaColor(sigma)

    def compute_disparity(self):
        self.left_disparity = self.stereo_left.compute(self.left_matcher_perspective, self.right_matcher_perspective)
        self.right_disparity = self.stereo_right.compute(self.right_matcher_perspective, self.left_matcher_perspective)
        displ = np.int16(self.left_disparity)
        dispr = np.int16(self.right_disparity)
        self.filteredImg = self.wls_filter.filter(displ, self.left_matcher_perspective, None, dispr)  # important to put "imgL" here!!!

    def get(self):
        return self.filteredImg

    def debug(self):
        filteredImg = cv2.normalize(src=self.filteredImg, dst=self.filteredImg, beta=0, alpha=255, norm_type=cv2.NORM_MINMAX);
        filteredImg = np.uint8(filteredImg)
        cv2.imwrite('depth.jpg', filteredImg)


def computeROI(width, height, matcher_instance):
    min_disparity = matcher_instance.getMinDisparity()
    num_disparities = matcher_instance.getNumDisparities()
    block_size = matcher_instance.getBlockSize()

    bs2 = block_size/2
    minD = min_disparity
    maxD = min_disparity + num_disparities - 1

    xmin = maxD + bs2
    xmax = width + minD - bs2
    ymin = bs2
    ymax = height - bs2

    return Rect(xmin, ymin, xmax - xmin, ymax - ymin)


# Cesta 
#loader = StereoLoader('/home/fila/IMG_0389.JPG', '/home/fila/IMG_0390.JPG')
# Volvo
loader = StereoLoader('/home/fila/IMG_0383.JPG', '/home/fila/IMG_0384.JPG')
# Chata - Krtek
#loader = StereoLoader('/home/fila/IMG_0385.JPG', '/home/fila/IMG_0386.JPG')
# Chata - nase
#loader = StereoLoader('/home/fila/IMG_0367.JPG', '/home/fila/IMG_0368.JPG')
# Volvo2
#loader = StereoLoader('/home/fila/IMG_0417.JPG', '/home/fila/IMG_0418.JPG')

# extract features
feature_extraction=FeatureFinder(loader.left_matcher(), loader.right_matcher(), loader.left_orig(), loader.right_orig())
feature_extraction.detect()
feature_extraction.debug()

# find fundamental matrix
epipolar_geometry=EpipolarGeometry(
   feature_extraction.descriptor_left(),
   feature_extraction.descriptor_right(),
   feature_extraction.key_points_left(),
   feature_extraction.key_points_right()
)
epipolar_geometry.compute_fundamental_matrix()
epipolar_geometry.debug_matches(loader.left_matcher(), loader.right_matcher())
epipolar_geometry.debug_epipolar(loader.left_matcher(), loader.right_matcher())

perpective_transform=PerspectiveTransform(
    epipolar_geometry.left_matches(),
    epipolar_geometry.right_matches(),
    epipolar_geometry.get_mask(),
    epipolar_geometry.get_fundamental(),
    loader.left_matcher(),
    loader.right_matcher()
)
perpective_transform.warp_perspective()
perpective_transform.debug()

depth_finder=DepthFinder(
    perpective_transform.get_left_matcher_perspective(), 
    perpective_transform.get_right_matcher_perspective()
)
depth_finder.compute_disparity()
depth_finder.debug()
#plt.imshow(depth_finder.get(), 'gray'),plt.show()


#ROI=computeROI(im_width, im_height, stereo)
#print ROI

sys.exit(0)
