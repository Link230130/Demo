#include <QApplication>

#include <QWidget>
#include <stdio.h>
#include <curl/curl.h>

#if 0
#include "FtpManage.h"

int main(void)
{
    // 按格式初始化Ftp，第一个"1"是用户名，第二个"1"是用户名的密码，"ftp://192.168.1.1/"是FTP的ip地址，后面记得加'/'
    FtpManage Aftp = FtpManage("forftp", "123", "ftp://192.168.230.1/");

    // 本地某个文件，需要绝对路径
    std::string localfile = "D:/test_ftp_upload.txt";

    // Ftp服务器上某个文件，需要绝对路径
    std::string remotefile = "/test_file.txt";

    // 从Ftp上下载来保存到本地的目录路径,需以“/”结尾
    std::string localpath = "D:/";

    // 在FTP上创建文件夹，需要指名路径+文件夹名称，最后以'/'结尾
    std::string newdiretoty = "/ne111##new1/";

    // 将localfile对应的文件上传到192.168.1.1的某个路径,remoteDirectory代表上传到的FTP路径
    std::string remoteDirectory = "/";

    // listpath 从FTP哪个文件夹下载文件
    std::string listpath = "/";

    /*第二部分：函数调用方式 */
    // 上传文件
    //Aftp.Upload(localfile.c_str(), remoteDirectory.c_str());

    // 将Ftp上的remotefile对应的文件下载到本地localpath对应的目录下  
    //Aftp.DownloadFile(remotefile.c_str(), localpath.c_str());

    // 将Ftp上listpath文件夹里面所有文件下载到本地localpath目录下
    //Aftp.DownloadAllFiles(listpath.c_str(), localpath.c_str());

    //在Ftp上创建newdiretoty对应的目录
    Aftp.Createdir(newdiretoty.c_str());

    // 查看ftp某个目录下有哪些文件
    //for (const auto& n : Aftp.GetFilesName(listpath))
    //{
    //    std::cout << n << std::endl;
    //}
    system("pause");
    return 0;
}
#else
int main(void)
{
    CURL *curl = curl_easy_init();
    if (curl) {
        char *output = curl_easy_escape(curl, "data to convert", 15);
        if (output) {
            printf("Encoded: %s\n", output);
            curl_free(output);
        }
        curl_easy_cleanup(curl);
    }
}
#endif