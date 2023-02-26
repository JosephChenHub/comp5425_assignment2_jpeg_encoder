# Implement a JPEG Encoder (sequential mode) from scratch
This tutorial shows you how to implement a simple JPEG encoder (sequential mode) step by step. 

## What you should do?
Implement the following parts
- RGB to YUV color transform
- Sample image blocks, you are required to implement this method for **YUV 444**, **YUV 422**, and **YUV 420** 
- DCT (you are not allowed to use external library such as cv2.dct)

## What you will have ?
- Learn the pipeline and details of a JPEG encoder
- Implement a simple JPEG encoder for fun, e.g., given a PNG image with size over **30MB**, 
with your JPEG encoder, it can be compressed to no more than **1MB**. 


## Preparation for CPP users 
We use ``cmake >= 3.13 " to build the project, actually you can use other compilers like Visual Studio, MingW, etc.
0. build 
```
cd ./jpeg_encoder && \
mkdir build && cd build \
cmake .. \
make -j
```
use **-DBUILD_TESTS=ON** to enable tests, **-DBUILD_PYTHON_MODULE=ON** to build python interface.

usage example:
```
./build/jpeg_encoder -i ./data/sg_0.png -o ./data/sg_0_q50_444.jpg -q 50 -f 444
```
This command will generate a JPEG image with quality 50 and YUV444 format. 

## Preparation for Python users
**Method 1 (via CMake)**

The python version is  based on cpp version (Huffman coding & JPEG IO), so you still need to 
build *jpeg_py* first:
0. add the pybind11 if you need a python wrapper 
```
cd ./jpeg_encoder && \
git submodule add https://github.com/pybind/pybind11.git
```
1. build the *jpeg_py* module:
```
cd ./jpeg_encoder && \
mkdir build && cd build \
cmake .. -DBUILD_PYTHON_MODULE=ON \
make -j
```
2. add *jpeg_py* to the Python environment :
```
import sys
sys.path.append("./build") # the path -> jpeg_py module, you should first build the module via cmake first 

from jpeg_py import YUVFormat, JpegIO, HuffmanCodec
```
Then you can use the following APIs:
- YUVFormat.YUV444 : YUV444 , i.e., no subsampling for luminance & chrominance
- YUVFormat.YUV420 : YUV420 ,  the Micro Block will look like [YYYYUV][YYYYUV]... [YYYYUV], 
                     i.e., subsampling chrominance from vertical (factor 2) & horizontal (factor 2) direction
- YUVFormat.YUV422 : YUV4222,  subsampling chrominance from horizontal direction (factor 2), [YYYYUUVV][YYYYUUVV]...[YYYYUUVV]
- JpegIO.writeToFile:  write the encoded image to disk
- HuffmanCodec : huffman coding for Y, U, V blocks

**Method 2(via setup.py)**:
```
python3 setup.py build 
```
then add the './build/xxx' to your python environment, or install the package vai "python3 setup.py install"






## Reference materials:
- COMP5425, HK PolyU (lecturer: Prof. Changwen Chen)
- https://en.wikipedia.org/wiki/JPEG
- https://yasoob.me/posts/understanding-and-writing-jpeg-decoder-in-python/#discrete-cosine-transform-quantization
- Digital Video Concepts, Methods, and Metrics: Quality, Compression, Performance, and Power Trade-off Analysis
- https://users.ece.utexas.edu/~ryerraballi/MSB/pdfs/M4L1.pdf
- https://en.wikipedia.org/wiki/Chroma_subsampling#Sampling_systems_and_ratios