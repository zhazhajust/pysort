#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <dirent.h>

std::vector<std::string> readFolder(const std::string &image_path)
{
    std::vector<std::string> image_names;
    auto dir = opendir(image_path.c_str());
 
    if ((dir) != NULL)
    {
        struct dirent *entry;
        entry = readdir(dir);
        while (entry)
        {
            auto temp = image_path + "/" + entry->d_name;
            if (strcmp(entry->d_name, "") == 0 || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                entry = readdir(dir);
                continue;
            }
            //std::cout << temp << "\n";
            image_names.push_back(temp);
            entry = readdir(dir);
        }
    }else{
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        std::cout << "Cannot open folder: " << image_path << std::endl;
    }
    return image_names;
};
