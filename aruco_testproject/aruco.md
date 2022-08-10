
# ArUco

from http://miloq.blogspot.com/2012/12/install-aruco-ubuntu-linux.html

1. Install OpenCV

2. [Download and decompress ArUco](https://sourceforge.net/projects/aruco/files/)

3. Compile

Now, in your terminal, make sure you are within the ArUco directory and run the following commands:

```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../aruco_src
make
sudo make install
```

4. Configure

To make sure that ArUco finds the correct library files, generate a config file.

In your terminal, execute:

`sudo gedit /etc/ld.so.conf.d/aruco.conf`

Add the following line and save it:

`/usr/local/lib`

Now, in your terminal, execute:

`sudo ldconfig`

Again, execute:

`sudo gedit /etc/bash.bashrc`

Check if the following two lines are at the end of the file. If not, add them and save the file:

```
PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH
```

Finally, reboot your computer.

Note that since the installation is not global you’ve to always include pathToInstall in CMakeLists.txt file of your project as

```
# Adding local ArUco Library
include_directories(/home/<pc-name>/arucoxxx/aruco_src/include/)
link_directories(/home/<pc-name>/arucoxxx/aruco_src/lib/)
```

5. Try an example

To test an example, from your terminal enters in the ArUco folder. Once you're inside it, enters in `build/utils`. Now, execute:

`./aruco_create_board 5:2 board.png board.yml`

With this, we have created a marker board with a size of 5×2. The image is board.png and board.yml is the board configuration.