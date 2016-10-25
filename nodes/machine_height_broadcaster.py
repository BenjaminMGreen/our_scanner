#!/usr/bin/env python  
import roslib
roslib.load_manifest('rplidar_ros')
import rospy
import tf

if __name__ == '__main__':
    rospy.init_node('machine_height_broadcaster')
    br = tf.TransformBroadcaster()
    rate = rospy.Rate(100)
    step = 0.003
    minheight = 0.000
    maxheight = 0.800
    currentheight = 0.3
    while not rospy.is_shutdown():
      while (currentheight < maxheight):
        currentheight += step
        br.sendTransform((0.000, 0.000, currentheight), (0.0, 0.0, 0.0, 1.0), rospy.Time.now(), "lidar_platform", "lidar_origin")
        rate.sleep()
      while (currentheight > minheight):
        currentheight -= step
        br.sendTransform((0.000, 0.000, currentheight), (0.0, 0.0, 0.0, 1.0), rospy.Time.now(), "lidar_platform", "lidar_origin")
        rate.sleep()
