#include <hdf5.h>
#include <string>
#include <vector>
#include <unordered_map>

void write_dict_to_hdf5(const std::string& filename, 
                        const std::unordered_map<int, std::vector<double>>& dict_x1,
                        const std::unordered_map<int, std::vector<double>>& dict_x2,
                        const std::unordered_map<int, std::vector<double>>& dict_p2) {
                        
    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    for (const auto& entry : dict_x1) {
        std::string dataset_name = "x1_" + std::to_string(entry.first);
        hid_t dataset_id = H5Dcreate2(file_id, dataset_name.c_str(), H5T_NATIVE_DOUBLE, H5S_SIMPLE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, entry.second.data());
        H5Dclose(dataset_id);
    }

    for (const auto& entry : dict_x2) {
        std::string dataset_name = "x2_" + std::to_string(entry.first);
        hid_t dataset_id = H5Dcreate2(file_id, dataset_name.c_str(), H5T_NATIVE_DOUBLE, H5S_SIMPLE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, entry.second.data());
        H5Dclose(dataset_id);
    }

    for (const auto& entry : dict_p2) {
        std::string dataset_name = "p2_" + std::to_string(entry.first);
        hid_t dataset_id = H5Dcreate2(file_id, dataset_name.c_str(), H5T_NATIVE_DOUBLE, H5S_SIMPLE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, entry.second.data());
        H5Dclose(dataset_id);
    }

    H5Fclose(file_id);
}
