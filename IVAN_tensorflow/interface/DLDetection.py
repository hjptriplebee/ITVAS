import numpy as np
import os
import tensorflow as tf
import sys
from PIL import Image

sys.path.append('/opt/models/research/')
sys.path.append('/opt/models/research/slim/')
from object_detection.utils import label_map_util
import time

def load_tf_model(gpu_num, model_path):

    os.environ['CUDA_VISIBLE_DEVICES'] = str(gpu_num);
    PATH_TO_CKPT = model_path;

    PATH_TO_LABELS = '../interface/vehicle_label_map.pbtxt'

    NUM_CLASSES = 1

    # Load a (frozen) Tensorflow model into memory.
    detection_graph = tf.Graph()
    with detection_graph.as_default():
        od_graph_def = tf.GraphDef()
        with tf.gfile.GFile(PATH_TO_CKPT, 'rb') as fid:
            serialized_graph = fid.read()
            od_graph_def.ParseFromString(serialized_graph)
            tf.import_graph_def(od_graph_def, name='')

    label_map = label_map_util.load_labelmap(PATH_TO_LABELS)
    categories = label_map_util.convert_label_map_to_categories(label_map, max_num_classes=NUM_CLASSES, use_display_name=True)
    category_index = label_map_util.create_category_index(categories)

    with detection_graph.as_default():
        with tf.Session(graph=detection_graph) as sess:
            #while True:  # for image_path in TEST_IMAGE_PATHS:    #changed 20170825
            # Definite input and output Tensors for detection_graph
            image_tensor = detection_graph.get_tensor_by_name('image_tensor:0')
            # Each box represents a part of the image where a particular object was detected.
            detection_boxes = detection_graph.get_tensor_by_name('detection_boxes:0')
            # Each score represent how level of confidence for each of the objects.
            # Score is shown on the result image, together with the class label.
            detection_scores = detection_graph.get_tensor_by_name('detection_scores:0')
            detection_classes = detection_graph.get_tensor_by_name('detection_classes:0')

    temp = {}
    temp['sess'] = sess
    temp['image_tensor'] = image_tensor
    temp['detection_boxes'] = detection_boxes
    temp['detection_scores'] = detection_scores
    temp['detection_classes'] = detection_classes
    return temp

def detect(img, temp):
    sess = temp['sess']
    image_tensor = temp['image_tensor']
    detection_boxes = temp['detection_boxes']
    detection_scores = temp['detection_scores']
    detection_classes = temp['detection_classes']

    result = []
    im_height,im_width,_ = img.shape
    image_np_expanded = np.expand_dims(img, axis=0)
    start = time.clock()
    (boxes, scores, classes) = sess.run([detection_boxes, detection_scores, detection_classes], feed_dict={image_tensor: image_np_expanded})
    # print("Time used:", time.clock() - start)
    min_score_thresh = 0.8
    boxes=np.squeeze(boxes)
    classes=np.squeeze(classes).astype(np.int32)
    scores=np.squeeze(scores)
    for i, box in enumerate(boxes):
        if scores[i] > min_score_thresh:
            ymin, xmin, ymax, xmax = box
            result.append(scores[i])
            result.append(int(xmin*im_width))
            result.append(int(ymin*im_height))
            result.append(int(xmax*im_width))
            result.append(int(ymax*im_height))
            #result.append(category_index[classes[i]]['name'])
            result.append('vehicle')
    return result      
