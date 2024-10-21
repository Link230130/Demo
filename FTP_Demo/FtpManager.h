#pragma once

#include <string>
#include <vector>
#include "curl/curl.h"

class CFtpManager
{
public:
    CFtpManager();
    CFtpManager(const std::string strUsername, const std::string strPassword, const std::string strHostIP);
    ~CFtpManager();

    /**
    * @brief 创建目录及其父目录，如果目录存在，依然返回true
    * @param strDirPath
    * @return
    */
    bool    MkPath(const std::string strDirPath);

    /**
     * @brief 向FTP服务器上传本地文件
     * @param strLocalFilename 文件全路径
     * @param strRemoteDir FTP服务器的目录，不包含文件名称
     * @return
     */
    bool    UploadFile(const std::string strLocalFilename, const std::string strRemoteDir);

    /**
     * @brief 从FTP服务器下载指定文件
     * @param strRemoteFilename FTP服务器的文件全路径，如 /AAA/a.txt
     * @param strLocalDir 保存文件的目录，如 D:/AAA/
     * @return
     */
    bool    DownFile(const std::string strRemoteFilename, const std::string strLocalDir);

    /**
     * @brief 获取FTP服务器指定目录下的文件名称列表
     * @param vecFilename
     * @param strRemotePath
     * @return
     */
    bool    GetFileList(std::vector<std::string>& vecFilename, const std::string strRemotePath);

private:

    /* 对url中的特殊字符进行编码转换 */
    std::string EncodeURL(const std::string& strInput);

    /* 从文件完整路径中提取文件名称 */
    std::string GetFilenameFromPath(const std::string& strFilePath);

    /* 获取一个Url对象 */
    CURL* GetUrl();

private:
    std::string          m_strHostIP;
    std::string          m_strUsername;
    std::string          m_strPassword;
};