# -*- coding: utf-8 -*-
"""
Created on Wed Sep  5 11:54:36 2018

@author: maoyingxue
"""

import cv2
def get_label(svm,img):
    if img.shape[0] > img.shape[1]:
        img = cv2.transpose(img)
    img = cv2.resize(img, (128, 64))
    hog = cv2.HOGDescriptor((128, 64), (16, 16), (8, 8), (8, 8), 9)
    features = hog.compute(img)
    result=svm.predict(features.reshape((1,-1)))[1][0][0]
    return result


