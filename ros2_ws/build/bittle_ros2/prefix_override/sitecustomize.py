import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/ORB_SLAM3/Examples/ros2_ws/install/bittle_ros2'
