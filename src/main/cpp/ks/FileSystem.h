#pragma once

#include <ostream>
#include <string>
#include <vector>

namespace ks {

class FileSystem
{
public:
    enum FileType
    {
        e_FileTypeUnknownError = -2,
        e_FileTypeNotExists = -1,
        e_FileTypeNormalFile = 0,
        e_FileTypeDirectory = 1,
        e_FileTypeSymlink = 2,
        e_FileTypeOther = 3,
    };

    static void SetWorkingDirectory(const std::string& path);

    static const std::string GetWorkingDirectory();

    static const std::string GetFileExtension(const std::string& path);

    static const std::string GetPathOfFile(const std::string& filePath);

    explicit FileSystem(const std::string& path = ".");

    FileSystem(const std::string& path, const std::string& name);

    FileSystem(const std::string& path, const FileSystem* refParentFs, const std::string& name);

    ~FileSystem() = default;

    const std::string& GetName() const {
        return m_name;
    }

    const std::string& GetRootPath() const {
        return m_rootPath;
    }

    bool Exists(const std::string& path = "");

    std::string GetAbsolutePath(const std::string& path = "");

    std::vector<std::string> List(const std::string& path = "", bool files = true, bool dirs = true,
        bool symlinks = true, bool other = true, const std::string& extension = "");

    FileType GetFileType(const std::string& path);

    void* LoadFile(const std::string& path, size_t* size);

    // overwrites anything existing without asking
    void SaveFile(const std::string& path, const void* data, size_t size);

    friend std::ostream& operator<<(std::ostream& os, const FileSystem& o)
    {
        return os << '[' << o.m_name << "] " << o.m_rootPath;
    }

private:
    const std::string m_name;
    std::string m_rootPath;

};

}
