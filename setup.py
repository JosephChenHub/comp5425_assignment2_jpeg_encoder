import setuptools
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

class BuildExtCommand(build_ext):
    def run(self):
        # Put your custom commands here
        print('Building C++ extension module')
        build_ext.run(self)

setup(
    name='jpeg_py',
    version='0.1',
    ext_modules=[
        Extension(
            'jpeg_py',
            sources=[
                'python/bind.cpp', 
                'src/HuffmanCodec.cpp',
                'src/JpegIO.cpp',
                'src/JpegZigzag.cpp',
                'src/JpegColor.cpp',
                'src/image.cpp',
                '3rdparty/bitstr.cpp'
            ],
            include_dirs=[
                'include',
                'pybind11/include'
            ],
            language='c++',
            extra_compile_args=['-std=c++14', '-O3']
        )
    ],
    cmdclass={
        'build_ext': BuildExtCommand
    },
    #setup_requires=['pybind11>=2.6'],
    author='Joseph Chen',
    author_email='zuyao.chen@connect.polyu.hk',
    description='Python wrapper for C++ JPEG encoder',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    url='https://github.com/JosephChenHub/jpeg_encoder_tutorial',
    packages=setuptools.find_packages(),
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
)

