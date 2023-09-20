#include <iostream>
#include <hdf5.h>
#include <vector>
#include <string>

class HDF5Reader {
private:
    hid_t file_id;
    hid_t dataset_id;
    hid_t dataspace_id;

public:
    HDF5Reader() : file_id(-1), dataset_id(-1), dataspace_id(-1) {}

    ~HDF5Reader() {
        close();
    }

    bool openFile(const char* filename) {
        file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
        if (file_id < 0) {
            std::cerr << "Unable to open file." << std::endl;
            return false;
        }

        return true;
    }

    bool openDataset(const char* dataset_name) {
        // Close any previously opened dataset
        if (dataset_id >= 0) {
            H5Dclose(dataset_id);
            dataset_id = -1;
        }

        dataset_id = H5Dopen2(file_id, dataset_name, H5P_DEFAULT);
        if (dataset_id < 0) {
            std::cerr << "Unable to open dataset." << std::endl;
            close();
            return false;
        }

        dataspace_id = H5Dget_space(dataset_id);
        if (dataspace_id < 0) {
            std::cerr << "Unable to get dataspace." << std::endl;
            close();
            return false;
        }

        return true;
    }

    bool readData(std::vector<double>& data_buffer) {
        if (dataset_id < 0) {
            std::cerr << "Dataset not open." << std::endl;
            return false;
        }

        hsize_t dims[1];
        int ndims = H5Sget_simple_extent_dims(dataspace_id, dims, NULL);
        if (ndims != 1 || dims[0] <= 0) {
            std::cerr << "Invalid dataset dimensions." << std::endl;
            close();
            return false;
        }

        int buffer_length = static_cast<int>(dims[0]);
        data_buffer.resize(buffer_length);

        herr_t status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_buffer.data());
        if (status < 0) {
            std::cerr << "Unable to read dataset." << std::endl;
            close();
            return false;
        }

        return true;
    }

    bool readData(std::vector<std::vector<int>>& data_buffer){//std::vector<std::vector<int>>& data_buffer) {
        hsize_t dims[2];
        int ndims = H5Sget_simple_extent_dims(dataspace_id, dims, NULL);
        if (ndims != 2 || dims[0] <= 0 || dims[1] <= 0) {
            std::cerr << "Invalid dataset dimensions. " << ndims << std::endl;
            close();
            return false;
        }

        int rows = static_cast<int>(dims[0]);
        int cols = static_cast<int>(dims[1]);

        std::vector<int> data(dims[0] * dims[1]);

        herr_t status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data.data());
        if (status < 0) {
            std::cerr << "Unable to read dataset." << std::endl;
            close();
            return false;
        }

        // Process the data (printing in this case)
        data_buffer = std::vector<std::vector<int>> (dims[0], std::vector<int>(dims[1]));
        for (hsize_t i = 0; i < dims[0]; ++i) {
            for (hsize_t j = 0; j < dims[1]; ++j) {
                data_buffer[i][j] = data[i * dims[1] + j];
            }
        }

        return true;
    }

    void close() {
        if (dataspace_id >= 0)
            H5Sclose(dataspace_id);

        if (dataset_id >= 0)
            H5Dclose(dataset_id);

        if (file_id >= 0)
            H5Fclose(file_id);

        dataspace_id = dataset_id = file_id = -1;
    }
};
