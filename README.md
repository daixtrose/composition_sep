# composition_sep

## Prerequisites / System Setup

Ubuntu 18.04 with

### ROS2

```bash
sudo apt-get install ros-eloquent-ros2bag ros-eloquent-rosbag2* ros-eloquent-ros1-bridge ros-eloquent-rosbag2-storage-default-plugins-dbgsym 
sudo apt install build-essential git python3-colcon-common-extensions python3-pip python-rosdep python3-vcstool wget
sudo apt install ros-eloquent-launch*
```

### Latest cmake

```bash
sudo snap install cmake --classic --channel=latest/stable
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt install g++-9
sudo apt install gcc-9
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9
```

In `.bashrc` add the following line

```bash
export PATH=/snap/bin:$PATH
```

## Build

```bash
export AMENT_CURRENT_PREFIX=/opt/ros/eloquent
export AMENT_PREFIX_PATH=/opt/ros/eloquent
export PYTHONPATH=/opt/ros/eloquent/lib/python3.6/site-packages
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/opt/ros/eloquent ..
make
```

## Run

```bash
export LD_LIBRARY_PATH=/opt/ros/eloquent/lib
./manual_composition
```

## Package and Install

```bash
cpack
sudo dpkg -i composition-1.0.0_amd64.deb
/opt/composition-1.0.0/bin/manual_composition
```
