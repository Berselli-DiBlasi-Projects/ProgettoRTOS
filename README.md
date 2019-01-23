# ProgettoRTOS
Image processing. Simulate a fixed camera looking at a given portion of an animated screen and write 4 periodic tasks performing different computations on the same image and displaying the results on other images. For example, task1 displays the histogram, task2 displays the image after applying a threshold, task3 displays the difference between consecutive frames, task4 displays the result of a filter. The user must be able to vary some parameters of such tasks.

Running Image processing application:\
mkdir build\
cd build\
cmake ..\
make\
./application


Extern Dependences:

sudo apt-get install git\
sudo apt-get install build-essential\
sudo apt-get install cmake\
sudo apt-get install libsdl2-dev\
sudo apt-get install libgtkmm-3.0-dev\
sudo apt-get install liballegro4.2-dev\

//-------------------------------------------------------------------------------------

Ptask:

git clone https://github.com/glipari/ptask.git\
mkdir build\
cd build\
cmake ..\
make\
make install\


/* 
   _(PTASK ISSUE)_
   In order to compile and install ptask with allegro and the new glibc library,\
   add "#define ALLEGRO_NO_FIX_ALIASES" on top of those files:\
   examples/pcp.c\
   examples/activation_example/animation.c\
   examples/ball.c\
   examples/advanced_act_test/animation.c\
   examples/system_example/testSystem.c\
   examples/system_example/testProt.c\
   examples/system_example/testPart.c\
   examples/system_example/animation.c\
   
   or
   
   comment inside CMakeLists.txt the row 11:\
   add_subdirectory(examples)
*/
   

//-------------------------------------------------------------------------------------

Opencv:

cd ~/<my_working_directory>\
git clone https://github.com/opencv/opencv.git\
git clone https://github.com/opencv/opencv_contrib.git\
cd ~/opencv\
mkdir build\
cd build\
cmake -DOPENCV_EXTRA_MODULES_PATH=<opencv_contrib>/modules <opencv_source_directory>\
make -j4     (depending on number of cores -> nproc)\

//--------------------------------------------------------------------------------------

When Image Processing application is compiled, in order to avoid "warning:\
"_GNU_SOURCE" redefined", delete the instruction "#define _GNU_SOURCE" from pmutex.h
(inside ptask installation folder).\
Anyway, it could be also ignored.

