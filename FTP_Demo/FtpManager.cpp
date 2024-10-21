#include "FtpManager.h"

#include <iostream>

using namespace std;

//接收消息回调函数
size_t WriteFilenameCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

CFtpManager::CFtpManager()
{

}

CFtpManager::CFtpManager(const string strUsername, const string strPassword, const string strHostIP)
    : m_strUsername(strUsername)
    , m_strPassword(strPassword)
    , m_strHostIP(strHostIP)
{
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK)
    {
        // LOG ERROR
    }
}

CFtpManager::~CFtpManager()
{
    curl_global_cleanup();
}

bool CFtpManager::MkPath(const std::string strDirPath)
{
    if (strDirPath.empty())
    {
        return false;
    }

    CURL* pHandle = GetUrl();
    if (nullptr == pHandle)
    {
        return false;
    }

    string strUrl = m_strHostIP + EncodeURL(strDirPath);
    // 指定url
    curl_easy_setopt(pHandle, CURLOPT_URL, strUrl.c_str());
    //如果目录不存在则创建一个
    curl_easy_setopt(pHandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
    CURLcode res = curl_easy_perform(pHandle);
    if (res != CURLE_OK)
    {
        std::cout << "Directory:" << strDirPath << "create failed!" << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(pHandle);
        return false;
    }
    else
    {
        std::cout << "Create directory:" << strDirPath << " successfully!" << std::endl;
    }
    curl_easy_cleanup(pHandle);
    return true;
}

bool CFtpManager::UploadFile(const std::string strLocalFilePath, const std::string strRemoteDir)
{
    CURL* pHandle = GetUrl();
    if (nullptr == pHandle)
    {
        return false;
    }
    std::string strUrl = m_strHostIP + EncodeURL(strRemoteDir) + GetFilenameFromPath(strLocalFilePath);

    FILE* pFile = nullptr;
    errno_t ret = fopen_s(&pFile, strLocalFilePath.c_str(), "rb");

    bool bRet = false;

    if (ret == 0)
    {
        curl_off_t fsize = 0;
        fseek(pFile, 0, SEEK_END);
        // 获取文件大小
        fsize = ftell(pFile);
        // 启用uploading
        curl_easy_setopt(pHandle, CURLOPT_UPLOAD, 1L);
        // 设置上传的地址
        curl_easy_setopt(pHandle, CURLOPT_URL, strUrl.c_str());
        // 设置上传的文件
        curl_easy_setopt(pHandle, CURLOPT_READDATA, pFile);
        // 设置上传的大小
        curl_easy_setopt(pHandle, CURLOPT_INFILESIZE, fsize);
        // 开始上传
        CURLcode ret = curl_easy_perform(pHandle);
        if (ret == CURLE_OK)
        {
            bRet = true;
        }
        else
        {
            bRet = false;
        }
        fclose(pFile);
    }

    curl_easy_cleanup(pHandle);
    return bRet;
}

bool CFtpManager::DownFile(const std::string strRemoteFilePath, const std::string strLocalDir)
{
    CURL* pHandle = GetUrl();
    if (nullptr == pHandle)
    {
        return false;
    }
    std::string strUrl = m_strHostIP + EncodeURL(strRemoteFilePath);
    std::string strLocalFilePath = strLocalDir + GetFilenameFromPath(strRemoteFilePath);
    FILE* pFile = nullptr;
    errno_t err = fopen_s(&pFile, strLocalFilePath.c_str(), "wb");

    bool bRet = false;

    if (err == 0)
    {
        curl_easy_setopt(pHandle, CURLOPT_WRITEDATA, pFile);
        curl_easy_setopt(pHandle, CURLOPT_URL, strLocalFilePath.c_str());
        CURLcode resCode = curl_easy_perform(pHandle);
        if (resCode == CURLE_OK)
        {
            bRet = true;
        }
        else
        {
            bRet = false;
        }
        fclose(pFile);
    }
    curl_easy_cleanup(pHandle);
    return bRet;
}

bool CFtpManager::GetFileList(std::vector<std::string>& vecFilename, const std::string strRemotePath)
{
    CURL* pHandle = GetUrl();
    if (nullptr == pHandle)
    {
        return false;
    }

    std::string strUrl = m_strHostIP + EncodeURL(strRemotePath);

    std::string strFilename;

    bool bRet = false;

    curl_easy_setopt(pHandle, CURLOPT_URL, strUrl.c_str());
    // 只返回文件
    curl_easy_setopt(pHandle, CURLOPT_DIRLISTONLY, 1L);
    curl_easy_setopt(pHandle, CURLOPT_WRITEDATA, &strFilename);
    curl_easy_setopt(pHandle, CURLOPT_WRITEFUNCTION, WriteFilenameCallback);
    if (curl_easy_perform(pHandle) == CURLE_OK)
    {
        vecFilename.clear();
        size_t nStartPos = 0;
        size_t nEndPos = 0;
        while ((nEndPos = strFilename.find('\n', nStartPos)) != std::string::npos)
        {
            std::string strName = strFilename.substr(nStartPos, nEndPos - nStartPos - 1);
            vecFilename.emplace_back(strName);
            nStartPos = nEndPos + 1;
        }
        bRet = true;
    }
    else
    {
        bRet = false;
    }
    curl_easy_cleanup(pHandle);

    return bRet;
}

std::string CFtpManager::EncodeURL(const std::string & strInput)
{
    CURL* pHandle = curl_easy_init();
    if (nullptr == pHandle)
    {
        return std::string();
    }
    char* pOutput = curl_easy_escape(pHandle, strInput.c_str(), strInput.length());
    if (pOutput)
    {
        std::string strEncoded(pOutput);
        curl_free(pOutput);
        curl_easy_cleanup(pHandle);
        return strEncoded;
    }
    curl_easy_cleanup(pHandle);
    return std::string();
}

std::string CFtpManager::GetFilenameFromPath(const std::string & strFilePath)
{
    if (strFilePath.empty())
    {
        return strFilePath;
    }

    std::string strRet;
    size_t nLastBlank = strFilePath.find_last_not_of(' ');
    if (nLastBlank != std::string::npos)
    {
        // 去除末尾的空格
        strRet = strFilePath.substr(0, nLastBlank + 1);
    }
    else {
        return std::string();
    }

    // 查找末尾的斜杠或者反斜杠
    size_t nLastSlashPos = strRet.find_last_of("/\\");
    if (nLastSlashPos != std::string::npos)
    {
        return strRet.substr(nLastSlashPos + 1);
    }

    return strRet;
}

CURL * CFtpManager::GetUrl()
{
    CURL* pUrl = curl_easy_init();
    // 长连接
    curl_easy_setopt(pUrl, CURLOPT_TCP_KEEPALIVE, 1L);
    // 用户密码
    curl_easy_setopt(pUrl, CURLOPT_USERPWD, (m_strUsername + ':' + m_strPassword).c_str());
    return pUrl;
}
