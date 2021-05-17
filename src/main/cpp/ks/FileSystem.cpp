#include "FileSystem.h"

#ifdef KS_PLATFORM_LINUX
#include <dirent.h>
#include <sys/stat.h>
#elif defined(KS_PLATFORM_WIN32)
#include <windows.h>
#endif

#include "FileSystemException.h"
#include "Logger.hpp"

namespace ks {

void FileSystem::SetWorkingDirectory(const std::string& path)
{
#ifdef KS_PLATFORM_LINUX
    if (chdir(path.c_str())) {
        throw FileSystemException(errno,
            "Setting working directory to %s failed", path);
    }
#elif defined(KS_PLATFORM_WIN32)
    if (!SetCurrentDirectoryA(path.c_str())) {
        throw FileSystemException("Setting working directory to " + path + " failed", GetLastError());
    }
#endif
}

const std::string FileSystem::GetWorkingDirectory()
{
#ifdef KS_PLATFORM_LINUX
    char buf[8192];
    char *path = getcwd(buf, sizeof(buf));

    if (path == nullptr) {
        throw FileSystemException(errno, "Getting working directory failed");
    }

    return std::string(path);
#elif defined(KS_PLATFORM_WIN32)
    char path[MAX_PATH];
    DWORD charCount = GetCurrentDirectoryA(MAX_PATH, path);

    if (charCount == 0) {
        return "";
    }

    // replace \ with /
    std::string strPath = path;

    for (size_t i = 0; i < strPath.size(); i++) {
        if (strPath[i] == '\\')
            strPath[i] = '/';
    }

    return strPath;
#endif
}

const std::string FileSystem::GetFileExtension(const std::string& path)
{
    size_t pos = path.find_last_of('.');

    if (pos == std::string::npos) {
        return std::string();
    } else {
        return path.substr(pos + 1, path.size());
    }
}

const std::string FileSystem::GetPathOfFile(const std::string& filePath)
{
    size_t pos = filePath.find_last_of('/');

    if (pos == std::string::npos) {
        return std::string();
    } else {
        std::string tmp = filePath.substr(0, pos);

        if (tmp.empty()) {
            return "/";
        } else {
            return tmp;
        }
    }
}

FileSystem::FileSystem(const std::string& path) :
    m_name(path), m_rootPath(path)
{

}

FileSystem::FileSystem(const std::string& path, const std::string& name) :
    m_name(name), m_rootPath(path)
{
    // cut off / at the end
    if (m_rootPath.at(m_rootPath.length() - 1) == '/') {
        m_rootPath = m_rootPath.substr(0, m_rootPath.length() - 1);
    }
}

FileSystem::FileSystem(const std::string& path, const FileSystem* refParentFs,
    const std::string& name) :
        m_name(name), m_rootPath(refParentFs->GetRootPath() + "/" + path)
{
    // cut off / at the end
    if (m_rootPath.at(m_rootPath.length() - 1) == '/') {
        m_rootPath = m_rootPath.substr(0, m_rootPath.length() - 1);
    }
}

bool FileSystem::Exists(const std::string& path)
{
#ifdef KS_PLATFORM_LINUX
    struct stat st {};
    return stat((m_rootPath + "/" + path).c_str(), &st) == 0;
#elif defined(KS_PLATFORM_WIN32)
    DWORD attr = GetFileAttributesA((m_rootPath + "/" + path).c_str());

    return attr != INVALID_FILE_ATTRIBUTES;
#endif
}

std::string FileSystem::GetAbsolutePath(const std::string& path)
{
#ifdef KS_PLATFORM_LINUX
    char tmp[PATH_MAX];
    std::string p = m_rootPath + "/" + path;

    if (!realpath(p.c_str(), tmp)) {
        throw FileSystemException(errno, "Could not resolve %s to absolute path", p);
    }

    return std::string(tmp);
#elif defined(KS_PLATFORM_WIN32)
    char tmp[MAX_PATH];
    memset(tmp, 0, MAX_PATH);

    std::string p = m_rootPath + "/" + path;
    DWORD len = GetFullPathNameA(p.c_str(), MAX_PATH, tmp, NULL);

    if (!len) {
        throw FileSystemException("Could not resolve " + p + " to absolute path", GetLastError());
    }

    return std::string(tmp, len);
#endif
}

std::vector<std::string> FileSystem::List(const std::string& path, bool files,
        bool dirs, bool symlinks, bool other, const std::string& extension)
{
#ifdef KS_PLATFORM_LINUX
    DIR *dir;
    struct dirent* ent;
    std::vector<std::string> resList;
    std::string p = m_rootPath + "/" + path;

    if ((dir = opendir(p.c_str()))) {

        // print all files and directories within directory
        while ((ent = readdir(dir))) {
            bool add = false;

            switch (ent->d_type) {
                // regular file
                case DT_REG:
                    if (files) {
                        add = true;
                    }

                    break;

                    // directory
                case DT_DIR:
                    if (dirs) {
                        add = true;
                    }

                    break;

                    // symbolic link
                case DT_LNK:
                    if (symlinks) {
                        add = true;
                    }

                    break;

                    // named pipe or fifo
                case DT_FIFO:

                    // local domain socket
                case DT_SOCK:

                    // char device
                case DT_CHR:

                    // block device
                case DT_BLK:

                default:
                    if (other) {
                        add = true;
                    }

                    break;
            }

            if (add) {
                // don't add . and ..
                std::string name(ent->d_name);

                if (name != "." && name != "..") {
                    if (extension.empty()) {
                        resList.push_back(name);
                    } else {
                        // get extension
                        size_t index = name.find_last_of('.');

                        // only add if matching filter and got extension
                        if (index != std::string::npos && name.substr(index + 1, name.size() - 1) == extension) {
                            resList.push_back(name);
                        }
                    }
                }
            }
        }

        closedir(dir);

        return resList;
    }

    throw FileSystemException(errno, "Listing directory contents of %s failed", p);
#elif defined(KS_PLATFORM_WIN32)
    WIN32_FIND_DATAA ffd;
    std::vector<std::string> resList;
    std::string p = m_rootPath;

    if (!path.empty()) {
        p += "/" + path;
    }

    HANDLE handle = FindFirstFileA((p + "/*").c_str(), &ffd);

    if (handle == INVALID_HANDLE_VALUE) {
        throw FileSystemException("Listing directory contents of " + p + " failed", GetLastError());
    }

   // List all the files in the directory with some info about them.
    do {
        if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && dirs) ||
                (ffd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL && files) ||
                // this includes .dll files
                (ffd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE && files) ||
                (ffd.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED && files) ||
                (other)) {
            std::string fileName = ffd.cFileName;

            // skip . and ..
            if (fileName == "." || fileName == "..") {
                continue;
            }

            if (extension == "") {
                resList.push_back(fileName);
            } else {
                // get extension
                size_t index = fileName.find_last_of(".");

                // only add if matching filter and got extension
                if (index != std::string::npos && fileName.substr(index + 1, fileName.size() - 1) == extension) {
                    resList.push_back(fileName);
                }
            }
        }
    } while (FindNextFileA(handle, &ffd) != 0);

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        FindClose(handle);
        throw FileSystemException("Listing directory contents of " + p + " failed", GetLastError());
    }

    FindClose(handle);
    return resList;
#endif
}

FileSystem::FileType FileSystem::GetFileType(const std::string& path)
{
#ifdef KS_PLATFORM_LINUX
    struct stat st {};
    std::string p = m_rootPath + "/" + path;

    if (!stat(p.c_str(), &st)) {
        // normal file
        if (S_ISREG(st.st_mode)) {
            return e_FileTypeNormalFile;
            // dir
        } else if (S_ISDIR(st.st_mode)) {
            return e_FileTypeDirectory;
            // symlink
        } else if (S_ISLNK(st.st_mode)) {
            return e_FileTypeSymlink;
            // block device
        } else if (S_ISBLK(st.st_mode)) {
            return e_FileTypeOther;
            // char device
        } else if (S_ISCHR(st.st_mode)) {
            return e_FileTypeOther;
            // named pipe, fifo
        } else if (S_ISFIFO(st.st_mode)) {
            return e_FileTypeOther;
        } else {
            return e_FileTypeUnknownError;
        }
    } else {
        switch (errno) {
            case ENOENT:
                return e_FileTypeNotExists;
            default:
                return e_FileTypeUnknownError;
        }
    }
#elif defined(KS_PLATFORM_WIN32)
    DWORD fileAttributes = GetFileAttributesA((m_rootPath + path).c_str());

    if (fileAttributes == INVALID_FILE_ATTRIBUTES && (GetLastError() == ERROR_FILE_NOT_FOUND ||
            GetLastError() == ERROR_PATH_NOT_FOUND)) {
        return e_FileTypeNotExists;
    } else if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        return e_FileTypeDirectory;
    } else if (fileAttributes & FILE_ATTRIBUTE_NORMAL) {
        return e_FileTypeNormalFile;
    } else {
        return e_FileTypeUnknownError;
    }
#endif
}

void* FileSystem::LoadFile(const std::string& path, size_t* size)
{
    void* buffer;
    FILE* file;
    const std::string filePath = m_rootPath + "/" + path;

    KS_LOG_DEBUG("Loading file %s", filePath);

    file = fopen(filePath.c_str(), "rb");

    if (!file) {
        throw FileSystemException(errno, "Opening file %s failed", filePath);
    }

    fseek(file, 0, SEEK_END);
    *size = (size_t) ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = malloc(*size);
    if (fread(buffer, *size, 1, file) != 1) {
        free(buffer);
        *size = 0;
        throw FileSystemException(errno, "Reading file %s failed", filePath);
    }

    fclose(file);

    return buffer;
}

void FileSystem::SaveFile(const std::string& path, const void* data,
        size_t size)
{
    FILE* file;
    const std::string filePath = m_rootPath + "/" + path;

    KS_LOG_DEBUG("Saving file %s", filePath);

    file = fopen(filePath.c_str(), "w+");

    if (!file) {
        throw FileSystemException(errno, "Opening file %s failed", filePath);
    }

    if (fwrite(data, size, 1, file) != 1) {
        throw FileSystemException(errno, "Writing file %s failed", filePath);
    }

    fclose(file);
}

}