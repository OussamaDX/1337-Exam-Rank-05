#include <iostream>
#include <sys/stat.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <dirent.h>

int check_path(const char *path)
{
    struct stat s;
    if (stat(path, &s) == 0)
    {
        if (s.st_mode & S_IFDIR) //read // write
        {
            std::cout << "It's a directory\n";
            if (access(path, R_OK) == -1 || access(path, W_OK) == -1)
            {
                std::cout << "Access denied" << std::endl;
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else if (s.st_mode & S_IFREG)
        {
            std::cout << "It's a file\n";
            if (access(path, W_OK) == -1 || access(path, X_OK) == -1)
            {
                std::cout << "File Access denied" << std::endl;
                return 0;
            }
            else
            {
                return 2;
            }
        }
        else
        {
            std::cout << "Unknown type\n";
            return 0; // Return 403 for forbidden
        }
    }
    else
    {
        std::cout << "This file does not exist\n";
        return 0; // Return 404 for not found
    }
}


//check is file or directory

//check the permession


// void remove_directory(const char *path,bool is_directory)
// {
//     DIR *dirp = opendir(path);
//     struct dirent* dent;
//     while ((dirp = readdir(path)) != NULL)
//     {
//         if (strcmp(dent->d_name, ".")!= 0 && strcmp(dent->d_name, "..")!= 0)
//         {
//             if (is_directory)
//             {
//                 remove_directory(path,true);
//             }
//             else
//             {
//                 if (unlink(path) == -1)
//                 {
//                     exit(1);
//                 }
//             }
//         }
//     }
// }

void remove_directory(const char *path,bool is_directory)
{
    DIR *dirp = opendir(path);
    if (!dirp)
    {
        std::cerr << "Error opening directory " << path << std::endl;
        exit(1);
    }
    struct dirent *dent;
    while ((dent = readdir(dirp)) != NULL)
    {
        if (strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0)
        {
            std::string full_path = std::string(path) + "/" + dent->d_name;
            if (is_directory)
            {
                std::cout << full_path <<"\n";
                // std::cout << "hello";
                // exit(1);
                remove_directory(full_path.c_str(),true);
            }
            else
            {
                if (unlink(full_path.c_str()) == -1)
                {
                    std::cerr << "Error deleting file " << full_path << std::endl;
                    exit(1);
                }
            }
        }
    }
    closedir(dirp);
    if (remove(path) == -1)
    {
        std::cerr << "Error deleting directory " << path << std::endl;
        exit(1);
    }
}


int main()
{
	const char* file = "folder";

    int i = check_path(file);

    if (i == 2) // file
    {
        if (unlink(file) == -1)
        {
            std::cout << "File not found\n";
        }
        std::cout << "file is remove" << std::endl;
    }
    else if (i == 1) //is directory : 
    {
        std::cout << i << "\n";
        remove_directory(file,true);
    }
    else
        std::cout << "the message is : " << i; 

    return 0;
}
