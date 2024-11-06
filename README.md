# SLAM_Course_Project

Forked ORB-SLAM3 repository here:
https://github.com/gravesreid/ORB_SLAM3.git

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
```bash
docker pull gravesreid/orbslam3:latest
```
3. Run container
```bash
docker run -it \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  --name orbslam3_new \
  gravesreid/orbslam3
```
4. Run the url video feed script:
```bash
./Examples/Monocular/mono_url_video Vocabulary/ORBvoc.txt ./Examples/Monocular/picamera.yaml 
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

