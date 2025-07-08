#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "serial_driver/serial_comm.hpp"


SerialComm* serial_comm_ptr = nullptr;

void cmdVelCallback(const geometry_msgs::Twist::ConstPtr& msg) {
    float vx = msg->linear.x;
    float wz = msg->angular.z;

    if (serial_comm_ptr) {
        std::vector<float> speeds = {vx, wz};
        bool success = serial_comm_ptr->sendFloatArrayCommand(speeds);
        if (!success) {
            ROS_WARN("Send Error");
        }
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "serial_cmd_sender");
    ros::NodeHandle nh("~");

    std::string port;
    int baudrate;

    // 从参数服务器读取配置或设置默认值
    nh.param<std::string>("port", port, "/dev/ttyUSB0");
    nh.param<int>("baudrate", baudrate, 115200);

    // 创建 SerialComm 实例
    SerialComm comm(port, baudrate);
    serial_comm_ptr = &comm;

    // 订阅 /cmd_vel
    ros::Subscriber sub = nh.subscribe("/cmd_vel", 10, cmdVelCallback);

    ros::spin();

    return 0;
}
