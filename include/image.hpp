///
/// A wrapper for image data (force continuous)
/// @author: Joseph Chen
/// @email: zuyao.chen@connect.polyu.hk
/// @date: Feb. 22, 2023
/// 

#pragma once

#include "common.hpp"
#include <type_traits>
#include <cstdint>
#include <vector>
#include <utility>


#ifdef __cplusplus
extern "C" {
#endif
extern uint8_t* read_stb_rgb(const char* file, int &width, int &height, int &channels);

#ifdef __cplusplus
}
#endif

///
/// the Image object is stored as continous HWC format 
///
///
template <typename Dtype>
class Image {
private:
    int _rows;
    int _cols;
    int _channels;
    std::vector<Dtype> _data;
    
    void swap(Image& other) noexcept {
        std::swap(_rows, other._rows);
        std::swap(_cols, other._cols);
        std::swap(_channels, other._channels);
        std::swap(_data, other._data);
    }
public:
    // constructors
    Image(): _rows(0), _cols(0), _channels(0){ }
    Image(const int rows, const int cols, int channels):
        _rows(rows), _cols(cols), _channels(channels),
        _data(rows * cols * channels) {
    }
    Image(const Dtype* data, const int rows, const int cols, const int channels):
        _rows(rows), _cols(cols), _channels(channels), 
        _data(data, data + rows * cols * channels)  {
        ASSERT(rows > 0 && cols > 0 && channels > 0, "rows/cols/channels must be greater than zero !");      
    }
    // Constructor for uint8_t data
    template<typename U = Dtype, typename std::enable_if<std::is_same<U, uint8_t>::value>::type* = nullptr>
    Image(const char* filename) {
        uint8_t* data = read_stb_rgb(filename, _cols, _rows, _channels); 
        ASSERT(data, "Failed to open rgb image:" + std::string(filename));
        _data.resize(_rows * _cols * _channels);
        std::memcpy(_data.data(), data, _rows * _cols * _channels * sizeof(Dtype));
        free(data);
    }
    ~Image() {}
    // copy constructor
    Image(const Image& rhs) noexcept:
        _rows(rhs._rows), _cols(rhs._cols), _channels(rhs._channels),
        _data(rhs._data) { }
    // move constructor
    Image(Image&& rhs) noexcept :
        _rows(rhs._rows), _cols(rhs._cols), _channels(rhs._channels),
        _data(std::move(rhs._data)) {
        rhs._rows = 0; rhs._cols = 0; rhs._channels = 0; 
    }
    // copy assignment operator
    Image& operator=(Image rhs) noexcept {
        this->swap(rhs); 
        return *this;
    }
    // move assignment operator
    Image& operator=(Image&& rhs) noexcept {
        if (this != &rhs) {
            _rows = rhs._rows;
            _cols = rhs._cols;
            _channels = rhs._channels;
            _data = std::move(rhs._data);
            rhs._rows = 0;
            rhs._cols = 0;
            rhs._channels = 0;           
        }
        return *this;
    }    

public:
    template <typename U>
    void copyFrom(const Image<U> &rhs) {
        //Image<Dtype> tmp(rhs.rows(), rhs.cols(), rhs.channels());
        //for(size_t y = 0; y < rhs.rows(); ++y) {
        //    for(size_t x = 0; x < rhs.cols(); ++x) {
        //        for(size_t  c = 0; c < rhs.channels(); ++c) {
        //             tmp(y,x,c) = static_cast<Dtype>(rhs(y,x, c));
        //        }
        //    }
        //}
        //this->swap(tmp); 
        if (_rows != rhs.rows() || _cols != rhs.cols() || _channels != rhs.channels()) {
            _data.reserve(rhs.numel());
            _rows = rhs.rows(); _cols = rhs.cols(); _channels = rhs.channels(); 
        }
        std::transform(rhs.data(), rhs.data() + rhs.numel(), _data.begin(),
                       [](const auto& val) { return static_cast<Dtype>(val); });
    }
    size_t numel() const { return _rows * _cols * _channels; }
    size_t rows() const { return _rows; }
    size_t cols() const { return _cols; }
    size_t channels() const { return _channels; }
    Dtype* data() { return _data.data();}
    const Dtype* data() const { return _data.data();}
    
    Dtype& operator()(size_t y, size_t x, size_t c) {
        ASSERT(x >= 0 && x < _cols && y >= 0 && y < _rows
              && c >= 0 && c < _channels,
              "make sure x: [0, cols), y: [0, rows), c:[0, channels) !");
        return _data[y * _cols * _channels + x * _channels + c];      
    }
    const Dtype& operator()(size_t y, size_t x, size_t c) const {
        ASSERT(x >= 0 && x < _cols && y >= 0 && y < _rows
              && c >= 0 && c < _channels,
              "make sure x: [0, cols), y: [0, rows), c:[0, channels) !");
        return _data[y * _cols * _channels + x * _channels + c];      
    }

    // create a copy of the image with the same dimensions and pixel type
    Image clone() const {
        Image out(_rows, _cols, _channels);
        std::copy(_data.begin(), _data.end(), out._data.begin());
        return out;
    }
    // subscript operator for 2D access
    Dtype* operator[](int row) {
        ASSERT(row >= 0 && row < _rows, "row index out of bounds!");
        return &_data[row * _cols * _channels];
    }
    const Dtype* operator[](int row) const {
        ASSERT(row >= 0 && row < _rows, "row index out of bounds!");
        return &_data[row * _cols * _channels];
    }

    // This member function is only available when Dtype is uint8_t
    template <typename T = Dtype>
    typename std::enable_if<std::is_same<T, uint8_t>::value, Image<uint8_t>>::type
    load_rgb_image(const char* filename) {
        uint8_t* data = read_stb_rgb(filename, _cols, _rows, _channels); 
        ASSERT(data != nullptr, "Failed to open rgb image:" + std::string(filename));
        _data = data; 
    }

    Dtype get_pixel(const int y, const int x, const int c) const {
       ASSERT(this->data() != nullptr, "empty image");
       ASSERT(c >= 0 && c < _channels, "check input channel !");

       return (x >= 0 && x <_cols && y >= 0 && y < _rows) ? (*this)(y, x, c) : 0;
    }

    Image roi_sampling(const int x0, const int y0, const int x1, const int y1,
                       const int sx, const int sy, std::vector<int> channels) const {
        ASSERT(this->data() != nullptr && this->numel() > 0, " image is empty!");
        ASSERT(sx >= 1 && sy >= 1, "sampling factor sx/sy < 1 !");

        int dw = (x1 - x0) / sx;
        int dh = (y1 - y0) / sy; 
        Image dst(dh, dw, channels.size());

        for(size_t y = 0; y < y1 - y0; y += sy) {
            for(size_t x = 0; x < x1 - x0; x += sx) {
                for(int c = 0; c < dst._channels; ++c) {
                    float val = 0;
                    for(int j = 0; j < sy; ++j) {
                        for(int i = 0; i < sx; ++i) {
                            val += get_pixel(y0+y+j, x0+x+i, channels[c]);  
                        }
                    }
                    val /= (sx*sy);
                    dst(int(y/sy), int(x/sx), c) = static_cast<Dtype>(val);
                }
            }
        }
        return dst;
     }

}; // end of class Image

// end of file
