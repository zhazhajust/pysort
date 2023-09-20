#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <cstring>
#include <hdf5.h>
#include "Tools/H5.hpp"
#include "Tools/listfile.hpp"
#include "Tools/write.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>

#define CHECK_ERROR(err, str) \
    if (err < 0) { \
        std::cerr << "Error: " << __FILE__ << ":" << __LINE__ << std::endl; \
        std::cerr << str << std::endl; \
        return; \
    } \

namespace py = pybind11;

class TrackParticles
{
public:
    TrackParticles(std::string wkdir): wkdir(wkdir){
        init(wkdir);
    };

    virtual ~TrackParticles(){};

    void add_particle(std::unordered_map<int, std::vector<int>>& dict_x1, const int key, const int x1) {
        auto it = dict_x1.find(key);
        if (it != dict_x1.end()) {
            it->second.push_back(x1);
        } else {
            dict_x1[key] = {x1};
        }
    };

    void add_particle(std::unordered_map<int, std::vector<double>>& dict_x1, const int key, const double x1) {
        auto it = dict_x1.find(key);
        if (it != dict_x1.end()) {
            it->second.push_back(x1);
        } else {
            dict_x1[key] = {x1};
        }
    };

    void init(std::string wkdir){
        this->filenames = readFolder(wkdir);
    }

    void iter(const std::string FILE){

        std::vector<double> x1, x3, p2;
        std::vector<std::vector<int>> tag;
        HDF5Reader reader;

        CHECK_ERROR(!reader.openFile(FILE.c_str()), "Unable to open HDF5 file: ");
        CHECK_ERROR(!reader.openDataset("p2"), "Unable to open HDF5 dataset: p2");
        CHECK_ERROR(!reader.readData(p2), "Unable to read HDF5 dataset: p2");
        CHECK_ERROR(!reader.openDataset("x1"), "Unable to open HDF5 dataset: x1");
        CHECK_ERROR(!reader.readData(x1), "Unable to read HDF5 dataset: x1");
        CHECK_ERROR(!reader.openDataset("x3"), "Unable to open HDF5 dataset: x3");
        CHECK_ERROR(!reader.readData(x3), "Unable to read HDF5 dataset: x3");
        CHECK_ERROR(!reader.openDataset("tag"), "Unable to open HDF5 dataset: tag");
        CHECK_ERROR(!reader.readData(tag), "Unable to read HDF5 dataset: tag");

        for (size_t j = 0; j < x1.size(); ++j) {
            int key = tag[j][0] + tag[j][1] * 100;
#pragma omp critical
            {
                this->add_particle(this->dict_x1, key, x1[j]);
                this->add_particle(this->dict_x2, key, x3[j]);
                this->add_particle(this->dict_p2, key, p2[j]);
            }
        };
    };

    void iter_all(){
        for(std::string filename: this->filenames){
            iter(filename);
        }
    };

    std::vector<std::string> get_filenames(){
        return this->filenames;
    };

    std::unordered_map<int, std::vector<double>>& get_data(int i){
        if(i == 0) return dict_x1;
        if(i == 1) return dict_x2;
        if(i == 2) return dict_p2;
    }
    std::string wkdir;
protected:
    std::vector<std::string> filenames;
    std::unordered_map<int, std::vector<double>> dict_x1, dict_x2, dict_p2;
};

PYBIND11_MODULE(particle_sort, m) {
    py::class_<TrackParticles>(m, "TrackParticles")
    .def(py::init<std::string>())
    .def("init", &TrackParticles::init)
    .def("iter", &TrackParticles::iter)
    .def("iter_all", &TrackParticles::iter_all)
    .def("get_filenames", &TrackParticles::get_filenames)
    .def("get_data", &TrackParticles::get_data);
}
