#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist

def send_cmd_vel():
    rospy.init_node('cmd_vel_test_pub', anonymous=True)
    pub = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    rate = rospy.Rate(10)  # 10 Hz

    while not rospy.is_shutdown():
        twist = Twist()
        twist.linear.x = 0.5  # 线速度 0.1 m/s
        twist.linear.y = 0.0
        twist.linear.z = 0.0

        twist.angular.x = 0.0
        twist.angular.y = 0.0
        twist.angular.z = 0.0  # 角速度 0.3 rad/s

        rospy.loginfo("Publishing test /cmd_vel: linear.x=%.3f, angular.z=%.3f", twist.linear.x, twist.angular.z)
        pub.publish(twist)

        rate.sleep()

if __name__ == '__main__':
    try:
        send_cmd_vel()
    except rospy.ROSInterruptException:
        pass
