import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped
import matplotlib.pyplot as plt

class PoseVisualizer(Node):
    def __init__(self):
        super().__init__('pose_visualizer')
        self.subscription = self.create_subscription(
            PoseStamped,
            'pose',  # Ensure this matches the topic name in your C++ node
            self.listener_callback,
            10)
        self.x_data = []
        self.z_data = []

        # Setup matplotlib
        plt.ion()
        self.fig, self.ax = plt.subplots()
        self.line, = self.ax.plot([], [], 'ro-', label="ORB-SLAM3 Path")
        self.ax.set_xlim(-10, 10)
        self.ax.set_ylim(-10, 10)
        self.ax.set_xlabel("X (meters)")
        self.ax.set_ylabel("Z (meters)")
        self.ax.legend()
        plt.show()

    def listener_callback(self, msg):
        # Extract x and z from the pose
        x = msg.pose.position.x
        z = msg.pose.position.z

        # Append to path
        self.x_data.append(x)
        self.z_data.append(z)

        # Update plot
        self.line.set_xdata(self.x_data)
        self.line.set_ydata(self.z_data)
        self.ax.relim()
        self.ax.autoscale_view()
        plt.draw()
        plt.pause(0.01)


def main(args=None):
    print("Starting pose_visualizer node")
    rclpy.init(args=args)
    node = PoseVisualizer()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()