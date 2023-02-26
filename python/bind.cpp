#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "JpegIO.hpp"
#include "HuffmanCodec.hpp"
#include "JpegDCT.hpp"

#include "image.hpp"

namespace py = pybind11;
using namespace std;







// Binding code
PYBIND11_MODULE(jpeg_py, m) {
    m.doc() = "JPEG IO writer & Huffman coding";
    py::enum_<YUVFormat>(m, "YUVFormat")
            .value("YUV444", YUVFormat::YUV444)
            .value("YUV420", YUVFormat::YUV420)
            .value("YUV422", YUVFormat::YUV422)
            .export_values();

    m.def("read_rgb_image", [](const char* file) {
          int w, h, c;
          uint8_t* data = read_stb_rgb(file, w, h, c);
          ASSERT(data, " Failed to open rgb image:" + std::string(file));  
          py::array dst = py::memoryview::from_buffer(
                  data,                                    // buffer pointer
                { h, w, c },                                  // shape (rows, cols)
                { sizeof(uint8_t) * w * c, sizeof(uint8_t) * c, sizeof(uint8_t) }   // strides in bytes
             );
          return dst;
        });

    /*
    m.def("fdct2d8x8", []( 
              py::array_t<int, py::array::c_style|py::array::forcecast> &block) {
             if (block.size() != 64) {
                throw std::runtime_error("block must have size of 64 !");
             }
             static JpegDCT m;
             py::buffer_info buf = block.request();
             py::array_t<int, py::array::c_style|py::array::forcecast> tmp(buf);
             auto tmp_buf = tmp.request();
             int* ptr = static_cast<int*>(tmp_buf.ptr);
             for(int i = 0; i < 64; ++i) ptr[i] <<= 2;

             m.fdct2d8x8(ptr, nullptr); 
             return tmp;
         }, "apply FDCT for a 2d 8x8 block ",  py::arg("block")
        );
    */ 
    py::class_<JpegIO>(m, "JpegIO")
        .def(py::init<>())
        .def_static("writeToFile",
            [](
                 const std::string& dst_file, py::array_t<char, py::array::c_style | py::array::forcecast> &buffer,
                 const std::vector<std::vector<int>>& quant_tab,
                 const std::vector<std::vector<uint8_t>>& huf_ac_tab,
                 const std::vector<std::vector<uint8_t>>& huf_dc_tab,
                 int w, int h, YUVFormat format) {
             // Get pointers to the underlying data of the vectors
             const int* quant_ptr[2] = {quant_tab[0].data(), quant_tab[1].data()};
             const uint8_t* huf_ac_ptr[2] = {huf_ac_tab[0].data(), huf_ac_tab[1].data()};
             const uint8_t* huf_dc_ptr[2] = {huf_dc_tab[0].data(), huf_dc_tab[1].data()};

             // Call the C++ function with the pointers
             JpegIO::writeToFile(dst_file.c_str(), buffer.data(), buffer.size(), quant_ptr, huf_ac_ptr, huf_dc_ptr, w, h, format);
             }, 
            "Write JPEG image to file.",
            py::arg("dst_file"),
            py::arg("buffer"),
            py::arg("quant_tab"),
            py::arg("huf_ac_tab"),
            py::arg("huf_dc_tab"),
            py::arg("width"),
            py::arg("height"),
            py::arg("format")
        );

 
    py::class_<HuffmanCodec>(m, "HuffmanCodec")
        .def(py::init<>())
        .def("encode", [](HuffmanCodec &self,
                          const py::array_t<int, py::array::c_style | py::array::forcecast> &y_blocks,
                          const py::array_t<int, py::array::c_style | py::array::forcecast> &u_blocks,
                          const py::array_t<int, py::array::c_style | py::array::forcecast> &v_blocks,
                          int w, int h, YUVFormat format) {
            if (y_blocks.ndim() != 2 || y_blocks.shape(1) != 64) {
                throw std::runtime_error("y_blocks must have shape (N, 64)");
            }
            if (u_blocks.ndim() != 2 || u_blocks.shape(1) != 64) {
                throw std::runtime_error("u_blocks must have shape (N, 64)");
            }
            if (v_blocks.ndim() != 2 || v_blocks.shape(1) != 64) {
                throw std::runtime_error("v_blocks must have shape (N, 64)");
            }

            const int n = y_blocks.shape(0);
            const int *y_ptr = y_blocks.data();
            const int *u_ptr = u_blocks.data();
            const int *v_ptr = v_blocks.data();
            return self.encode(y_ptr, u_ptr, v_ptr, w, h, format);
        }, "Huffman encode for blocks", 
        py::arg("y_blocks"), 
        py::arg("u_blocks"), 
        py::arg("v_blocks"), 
        py::arg("width"), 
        py::arg("height"), 
        py::arg("format") 
        )
        .def("getResult", [](HuffmanCodec &self, const int data_len) {
            char* tmp = self.getResult(); 
            py::array_t<char, py::array::c_style | py::array::forcecast> dst(data_len);
            py::buffer_info buf = dst.request();
            char* ptr = static_cast<char*>(buf.ptr);
            for(size_t i = 0; i < data_len; ++i) ptr[i] = tmp[i];
            return dst;
         }, "get encoded bytes" 
        );
 
}

