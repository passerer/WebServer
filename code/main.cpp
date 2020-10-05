#include <unistd.h>
#include "server/webserver.h"
#include "config/config.h"

int main() {
    /* 守护进程 后台运行 */
    //daemon(1, 0); 
    std::string sqlUser,sqlPwd,dbName;
    Config settings("./config.txt");
    int port = settings.Read("port", 1316); //端口
    int trigMode = settings.Read("trigMode", 3); //ET模式
    int timeoutMS = settings.Read("timeoutMS", 5000); //超时限制
    bool optLinger = settings.Read("optLinger", false); //优雅退出
    int sqlPort = settings.Read("sqlPort", 3306); //sql端口
    settings.ReadInto(sqlUser, "sqlUser"); //sql用户
    settings.ReadInto(sqlPwd, "sqlPwd");  //sql密码
    settings.ReadInto(dbName, "dbName");  //数据库名称
    int connPoolNum = settings.Read("connPoolNum", 12); //连接池数量
    int threadNum  = settings.Read("threadNum",6); //线程池数量
    bool openLog = settings.Read("openLog", false); //日志开关
    int logLevel = settings.Read("logLevel", 0); //日志等级
    int logQueSize = settings.Read("logQueSize",1024); //日志异步队列容量
     /*
    std::cout<<port<<std::endl<<trigMode<<std::endl<<timeoutMS<<std::endl<<optLinger<<std::endl<<sqlPort<<std::endl<<sqlUser
    <<std::endl<<sqlPwd<<std::endl<<dbName<<std::endl<<connPoolNum<<std::endl<<threadNum<<std::endl
    <<openLog<<std::endl<<logLevel<<std::endl<<logQueSize<<std::endl;
    */

    WebServer server(
        port, trigMode, timeoutMS, optLinger,        
        sqlPort, sqlUser.c_str(), sqlPwd.c_str(), dbName.c_str(), 
        connPoolNum, threadNum, openLog, logLevel, logQueSize);    
    server.Start();
} 
  
