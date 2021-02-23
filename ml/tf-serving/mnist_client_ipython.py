from __future__ import print_function

import sys
import threading

import grpc
import numpy
import tensorflow.compat.v1 as tf

from tensorflow_serving.apis import predict_pb2
from tensorflow_serving.apis import prediction_service_pb2_grpc
from tensorflow_serving.example import mnist_input_data


tf.compat.v1.app.flags.DEFINE_integer(
    'concurrency', 1, 'maximum number of concurrent inference requests')
tf.compat.v1.app.flags.DEFINE_integer('num_tests', 100, 'Number of test images')
tf.compat.v1.app.flags.DEFINE_string('server', 'serving:8500',
                                     'PredictionService host:port')
tf.compat.v1.app.flags.DEFINE_string('work_dir', '/tmp', 'Working directory. ')
FLAGS = tf.app.flags.FLAGS


test_data_set = mnist_input_data.read_data_sets(FLAGS.work_dir).test
channel = grpc.insecure_channel(FLAGS.server)
stub = prediction_service_pb2_grpc.PredictionServiceStub(channel)
request = predict_pb2.PredictRequest()
request.model_spec.name = 'mnist'
request.model_spec.signature_name = 'predict_images'

image, label = test_data_set.next_batch(2)
request.inputs['images'].CopyFrom(
    tf.make_tensor_proto(image, shape=image.shape))

result = stub.Predict(request)
