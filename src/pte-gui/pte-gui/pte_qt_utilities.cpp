#include "pte_qt_utilities.h"

namespace pte
{
    std::string get_file_name(const char* file_path)
    {
        //Extract the name of ref from path
        std::string path_ref = file_path;
        size_t path_ref_size = path_ref.size();
        const char* path_data = path_ref.data();
        size_t i = path_ref_size;
        for(; i>0; i--)
        {
            if(file_path[i] == '/')
                break;
        }
        if(i == 0)
        {
            std::cerr << "get_file_name: Not a valid path to reference" << std::endl;
            return "";
        }

        std::string name_of_ref = path_ref.substr(i+1,path_ref_size-i);
        size_t name_of_ref_size = name_of_ref.size();
        const char* name_data = name_of_ref.data();

        for(i=name_of_ref_size; i>0; i--)
        {
            if(name_data[i] == '.')
                break;
        }
        if(i == 0)
        {
            std::cerr << "get_file_name: File has no extension" << std::endl;
            return "";
        }

        return name_of_ref.substr(0,i);
    }
}
