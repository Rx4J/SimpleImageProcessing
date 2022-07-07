# SimpleImageProcessing
## Description 
Simple Image Processing with MPI and OpenCV. 

## Build
CMake is used for building
```
cmake .
make
```

## Launch
If you want to use multiprocessing, it launch like this
```
mpirun -n 4 CourseWork test-image.bmp
```
Where:\
4 - number of launched processes (>1).\
CourseWork - executable file name.\
test-image.bmp - target image.

For standart sequential processing:
```
./CourseWork test-image.bmp
```
or
```
mpirun -n 1 CourseWork test-image.bmp
```

## Requires
- OpenCV
- OpenMPI or MPICH
- CMake
