# SLAM_Course_Project

Forked ORB-SLAM3 repository here:
https://github.com/gravesreid/ORB_SLAM3.git

Docker image based on: 
https://github.com/jahaniam/orbslam3_docker.git

# Requirements
## Raspberry pi:
- Raspberry pi Bookworm OS

# Raspberry pi video streaming instructions
1. run mjpeg_server.py script:
```bash
python3 mjpeg_server.py
```
2. View video feed in your web browser
If you need to get the ip address of your raspberry pi, in terminal in raspberry pi:
```bash
ifconfig
```
Then in a web browser enter: http://<Your_Pi_IP_Address>:8000

# Instructions for Docker
1. Install docker
```bash
 curl -fsSL https://get.docker.com -o get-docker.sh
 sudo sh get-docker.sh
```
```bash
sudo usermod -aG docker $USER
```
```bash
newgrp docker
```
2. Pull the container
ROS2 Version:
```bash
docker pull gravesreid/orbslam3_foxy:latest
```
Original Version:
```bash
docker pull gravesreid/orbslam3:latest
```
3. Change display permissions for docker
```bash
xhost +local:docker
```

4. Run container
```bash
docker run -it \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  gravesreid/orbslam3
```
5. Run mjpeg_server.py on your raspberry pi:
- on raspberry pi:
```bash
ifconfig
```
This is how you get the ip address for your pi
- on your ubuntu pc teminal
```bash
ssh pi@<your-pi-ip-address>
```
6. In the docker container, change the web video url address in ORB_SLAM3/Examples/Monocular/mono_url_video.cc to include the correct ip address 
7. Build ORB_SLAM3 in the docker container:
```bash
cd /ORB_SLAM3
```
If first time:
```bash
chmod +x build.sh
```
```
./build.sh
```
8. Run the url video feed script:
```bash
./Examples/Monocular/mono_url_video Vocabulary/ORBvoc.txt ./Examples/Monocular/picamera.yaml
```

# Run ROS2 node
1. Go to ros2_ws:
```bash
cd Examples/Monocular/ros2_ws
```
2. Run the node:
```bash
ros2 run orb_slam3_ros2 MonoNode /ORB_SLAM3/Vocabulary/ORBvoc.txt /ORB_SLAM3/Examples/Monocular/picamera.yaml http://192.168.1.20:8000/stream.mjpg
```

## If you make changes to the code in your container and want to save the new docker container:
```bash
docker commit [container_name_or_id] [new_image_name]
```
```bash
docker tag [new_image_name] [new_image_name]:[tag]
```
```bash
docker login
```
```bash
docker tag [new_image_name] [username]/[repository_name]:[tag]
```
```bash
docker push [username]/[repository_name]:[tag]
```



# TODO
- [ ] Noetic and Foxy Branch and Ubuntu 20.04- Kanlong (Noetic) and Adrian (Foxy)
    - [ ] Simple test using camera only
    - [ ] Physical testing on robots 
- [ ] Humble Branch-  Benny
    - [ ] Simple test using camera only
    - [ ] Physical testing on robots 
- [ ] Jazzy Branch- Reid
    - [ ] Simple test using camera only
    - [ ] Physical testing on robots

# Milestones
- [x] Project Proposal (25 October 2024)
- [ ] Project Midterm (15 November 2024)
- [ ] Presentation Slides (02 December 2024)
- [ ] Final Report (11 December 2024)

