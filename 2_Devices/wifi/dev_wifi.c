#include "dev_wifi.h"
#include "errno.h"
#include <string.h>
#include "printf.h"
#include "dev_uart.h"
#include "drv_uart.h"
#include "ring_buffer.h"
#include <stdlib.h>


//管理每次连接驱动
static void NetNodeInsert(NetNode *ptnode);
static NetNode *NetNodeFind(unsigned int port);
static NetNode *NetNodeFind_PassConid(unsigned int conID);
static void NetNodeDelete(unsigned int port);


static int WiFiBTWaitACK(const char *ack, unsigned int timeout);
static int WIFI_Test();
static int WIFI_Reset();
static int WIFI_EchoControl(int status);
static int WiFiBTSetWorkMode(int Mode);
static int WiFiBTSetReceiveMode(unsigned char mode);


static int WifiBT_Init(struct WiFiBtDev *ptdev);
static int WifiBT_WIFIConnect(struct WiFiBtDev *ptdev, const char *username, const char *password);
static int WifiBT_WIFIDisConnect(struct WiFiBtDev *ptdev);
static int WifiBT_NetConnect(struct WiFiBtDev *ptdev, unsigned char type, const char *ip, unsigned int port);
static int WifiBT_NetDisconnect(struct WiFiBtDev *ptdev, unsigned int port);
static int WifiBT_Write(struct WiFiBtDev *ptdev, unsigned int port, unsigned char *buf, unsigned int length);
static int WifiBT_Read(struct WiFiBtDev *ptdev, unsigned int port, unsigned char *buf, unsigned int length);
static int WifiBT_WIFIStaStatus(struct WiFiBtDev *ptdev, unsigned int timeout);
static int WifiBT_WIFIScan(struct WiFiBtDev *ptdev, WiFiApInfo *apList, unsigned char maxAp, unsigned int timeout);
static int WiFiScanParseLine(const char *line, WiFiApInfo *pinfo);
static int WiFiBTReadFromBuffer(struct WiFiBtDev *ptdev, unsigned int port, unsigned char *buf, unsigned int length);

struct UARTDev* pUart = NULL;//wifi专用串口

static struct NetNode* gHeadNet = NULL;//连接头节点

//wifi对象
WiFiBtDevice gWiFiBtDevice = {
    .name = "WIFIBT",
    .dev_status = 0,
    .net_status = 0,
    .Init = WifiBT_Init,
    .WIFIConnect = WifiBT_WIFIConnect,
    .WIFIDisConnect = WifiBT_WIFIDisConnect,
    .NetConnect = WifiBT_NetConnect,
    .NetDisconnect = WifiBT_NetDisconnect,
    .Write = WifiBT_Write,
    .Read = WiFiBTReadFromBuffer,
    .WIFIStaStatus = WifiBT_WIFIStaStatus,
    .WIFIScan = WifiBT_WIFIScan

};

WiFiBtDevice *GetWIFIBTDevice(void)
{
    return &gWiFiBtDevice;
}


static int WifiBT_Init(struct WiFiBtDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    //给wifi专用串口对象赋值
    pUart = UARTDev_Find("WIFIUART");
    if(NULL == pUart)   return -ENODEV;
    int ret = pUart->Init(pUart);
    if(ESUCCESS != ret) return ret;
    
    
    //发送AT测试通信
    ret = WIFI_Test();
    if(ESUCCESS != ret) return ret;
    //发送AT+RST 重启一下WiFi芯片
    ret = WIFI_Reset();
    if(ESUCCESS != ret) return ret;
    //关闭回显模式
    ret = WIFI_EchoControl(0);
    if(ESUCCESS != ret) return ret;
    // 设置WiFi工作模式
    WiFiBTSetWorkMode(AP_STA);
    if(ESUCCESS != ret) return ret;
    // 设置网络数据接收模式——主动模式
    ret = WiFiBTSetReceiveMode(1);
    if(ESUCCESS != ret) return ret;
    
    return ret;
}

/**
 * @brief 芯片连接WiFi
 * 
 * status username-wifi名称   password-密码
 */
static int WifiBT_WIFIConnect(struct WiFiBtDev *ptdev, const char *username, const char *password)
{
    if(ptdev == NULL || username == NULL || password == NULL)  return -EINVAL;
    
    char str[100];  
    sprintf(str, "AT+WJAP=%s,%s\r\n", username, password);  
    unsigned char len = strlen(str);
    
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret) return ret;
    // 超时等待响应
    ret = WiFiBTWaitACK("WIFI_GOT_IP\r\n", 5000);//查询buffer是否有WIFI_GOT_IP
    ptdev->dev_status = 1;  // connected
    printf("%s\r\n", pRxBuffer->info.pHead);
    return ret;   
}

/**
 * @brief 芯片断开连接WiFi
 * 
 */
static int WifiBT_WIFIDisConnect(struct WiFiBtDev *ptdev)
{
    if(ptdev == NULL)  return -EINVAL;
    
    char str[50];  
    sprintf(str, "AT+WDISCONNECT\r\n");  
    unsigned char len = strlen(str);
    
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret) return ret;
    // 超时等待响应
    ret = WiFiBTWaitACK("OK\r\n", 100);
    ptdev->dev_status = 0;  // disconnected
    return ret;   
}

static int WifiBT_NetConnect(struct WiFiBtDev *ptdev, unsigned char type, const char *ip, unsigned int port)
{
    if(NULL == ptdev || NULL == ip)  return -EINVAL;
    
    if(ptdev->dev_status != 1)  return -EIO;  //保证wifi已连接才继续连接TCP
    
    NetNode *pNode = (NetNode*)malloc(sizeof(NetNode));
    if(NULL == pNode)  return -ENOMEM;
    pNode->ip = (char*)ip;
    pNode->type = type;
    pNode->port = port;
    
    if(NULL == gHeadNet)
        pNode->conID = 1;
    else
        pNode->conID = gHeadNet->conID + 1;
    
    char str[128] = {0};
    sprintf(str, "AT+SOCKET=%d,%s,%d,0,%d\r\n", type, ip, port, pNode->conID);
    unsigned char len = strlen(str);
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret)
    {
        free(pNode);
        return ret;
    }
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret)
    {
        free(pNode);
        return ret;
    }
    // 超时等待响应
    ret = WiFiBTWaitACK("OK\r\n", 500);
    if(ESUCCESS != ret)
    {
        debugprintf("%s\r\n", pRxBuffer->info.pHead);
        free(pNode);
        return ret;
    }
    ptdev->net_status = 1;  // 连接上socket服务器
    
    pNode->netbuffer = RingBufferNew(128);//？？？
    if(NULL == pNode->netbuffer)
    {
        free(pNode);
        return -ENOMEM;
    }
   
    NetNodeInsert(pNode);//conID连接管理
    return ESUCCESS;
    
}

static int WifiBT_NetDisconnect(struct WiFiBtDev *ptdev, unsigned int port)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == pUart)   return -ENODEV;
    
    NetNode *ptnode = NetNodeFind(port);
    if(NULL == ptnode)  return -ENODEV;
    
    char str[128] = {0};
    //sprintf(str, "AT+SOCKETDEL=%d\r\n", port);
    sprintf(str, "AT+SOCKETDEL=%d\r\n", ptnode->conID);
    unsigned char len = strlen(str);
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret)
    {
        return ret;
    }
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret)
    {
        return ret;
    }
    // 超时等待响应
    ret = WiFiBTWaitACK("OK\r\n", 500);
    if(ESUCCESS != ret)
    {
        debugprintf("%s\r\n", pRxBuffer->info.pHead);
        return ret;
    }
    if(NULL == gHeadNet)
        ptdev->net_status = 0;  // 断开socket服务器的连接
    
    //将这个连接节点删除
    NetNodeDelete(port);
    
    return ESUCCESS;
}

/**
 * @brief 芯片socket连接后发送信息
 * port-端口   buf-信息  length-长度
 */
static int WifiBT_Write(struct WiFiBtDev *ptdev, unsigned int port, unsigned char *buf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == buf)     return -EINVAL;
    if(0 == length)     return -EINVAL;
    if(NULL == pRxBuffer)   return -ENODEV;
    if(NULL == pUart)       return -ENODEV;
    
    //检查是否连接上网络
    if(ptdev->dev_status != 1)  return -EIO;
    
    NetNode *pNode = NetNodeFind(port);
    if(NULL == pNode)       return -ENODEV;
    char str[32] = {0};
    sprintf(str, "AT+SOCKETSEND=%d,%d\r\n", pNode->conID, length);
    unsigned char len = strlen(str);
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret)      return ret;
    // 超时等待响应
    ret = WiFiBTWaitACK(">", 100);//接收到 > 才可以开始发送数据
    if(ESUCCESS != ret) return ret;
    // 发送数据
    ret = pUart->Write(pUart, (unsigned char *)buf, length);
    if(len != ret)      return ret;
    // 超时等待响应
    ret = WiFiBTWaitACK("OK\r\n", 100);
    if(ESUCCESS != ret) return ret;
    
    return (int)length;
}

static int WifiBT_Read(struct WiFiBtDev *ptdev, unsigned int port, unsigned char *buf, unsigned int length)
{
    if(NULL == ptdev || NULL == buf)  return -EINVAL;
    
    return ESUCCESS;
}

static int WiFiBTReadFromBuffer(struct WiFiBtDev *ptdev, unsigned int port, unsigned char *buf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == buf)     return -EINVAL;
    if(0 == length)     return -EINVAL;
    if(NULL == pRxBuffer)   return -ENODEV;
    if(NULL == pUart)       return -ENODEV;
    if(ptdev->dev_status != 1)  return -EIO;
    
    NetNode *pNode = NetNodeFind(port);//根据端口寻找Net节点
    if(NULL == pNode)       return -ENODEV;
    
    //从Net节点获取数据
    return pNode->netbuffer->Read(pNode->netbuffer, buf, length);
}


/**
 * @brief 等待wifi芯片回复信息
 * ack回复的信息
 * timeout 超时时间
 */
static int WiFiBTWaitACK(const char *ack, unsigned int timeout)
{
    char buf[128] = {0};
    unsigned char i = 0;
    while(timeout)
    {
        int ret = pRxBuffer->Read(pRxBuffer, (unsigned char *)&buf[i], 1);
        if(ret == 1)    i = (i+1)%128;
        // 判断响应
        if(strstr(buf, ack))   return ESUCCESS;
        else if(strstr(buf, "ERR")) return -EIO;
        else if(strstr(buf, "Unknown cmd")) return -EIO;
        // 超时递减
        timeout--;
        HAL_Delay(1);
    }
    return -EIO;
}

/**
 * @brief 发送基础指令AT测试WiFi芯片
 * 
 */
static int WIFI_Test()
{
    char str[8] = "AT\r\n";
    unsigned char len = strlen(str);
    
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret) return ret;
     // 超时等待响应
    ret = WiFiBTWaitACK("OK\r\n", 100);
    printf("%s\r\n", pRxBuffer->info.pHead);

    return ret;
}

/**
 * @brief 重启芯片
 * 
 */
static int WIFI_Reset()
{
    char str[8] = "AT+RST\r\n";
    unsigned char len = strlen(str);
    
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret) return ret;
     // 超时等待响应
    ret = WiFiBTWaitACK("ready\r\n", 2000);
    printf("%s\r\n", pRxBuffer->info.pHead);
    
    return ret;
}

/**
 * @brief 芯片回显模式打开关闭
 * 
 * status 0-关闭回显模式 1-开启回显模式
 */
static int WIFI_EchoControl(int status)
{
    
    char str[6];  
    sprintf(str, "ATE%d\r\n", status);  
    unsigned char len = strlen(str);
    
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret) return ret;
     // 超时等待响应
    ret = WiFiBTWaitACK("OK\r\n", 100);
    printf("%s\r\n", pRxBuffer->info.pHead);
    
    return ret;
}

/**
 * @brief 芯片wifi模式
 * 
 * status 0-关闭 1-STA 2-AP 3-AP+STA
 */
static int WiFiBTSetWorkMode(int Mode)
{
    char str[30];  
    //默认不保存flash
    sprintf(str, "AT+WMODE=%d,0\r\n", Mode);  
    unsigned char len = strlen(str);
    
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret) return ret;
     // 超时等待响应
    ret = WiFiBTWaitACK("OK\r\n", 100);
    printf("%s\r\n", pRxBuffer->info.pHead);
    
    return ret;
}

static int WiFiBTSetReceiveMode(unsigned char mode)
{
    if(NULL == pRxBuffer) return -ENODEV;
    if(NULL == pUart)   return -ENODEV;
    char str[32] = {0};
    sprintf(str, "AT+SOCKETRECVCFG=%d\r\n", mode);
    unsigned char len = strlen(str);
    // 先清空buffer
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    // 再发送指令
    ret = pUart->Write(pUart, (unsigned char *)str, len);
    if(len != ret) return ret;
    // 超时等待响应
    ret = WiFiBTWaitACK("OK\r\n", 100);
    printf("%s\r\n", pRxBuffer->info.pHead);
    
    return ret;
}

/**
 * @brief 查询WiFi连接状态（发送 AT+STAINFO?，只取 +STAINFO:<status>）
 *
 * 芯片应答示例：
 *   +STAINFO:3
 *   SSID:test
 *   Password:123456789
 *   e1:f9:8a:aa:fc:4f,WPA/WPA2 TKIP,b2:e3:41:c2:b3:42,5,192.168.3.125,192.168.3.1
 *   OK
 *
 * @param timeout 等待应答超时时间（ms）
 * @return 0-4 连接状态；<0 错误（-ENODEV/-EIO）
 *   0-没有连接wifi 1-正在连接wifi或重连中 2-已连接wifi未获取IP
 *   3-已连接wifi并获取到IP 4-wifi连接失败（超过重连次数）
 *
 * 成功时会把 gWiFiBtDevice.dev_status 同步为：status==3 置 1，其余状态均置 0。
 */
int WiFiBTGetStaStatus(unsigned int timeout)
{
    if(NULL == pUart)       return -ENODEV;
    if(NULL == pRxBuffer)   return -ENODEV;

    //先清空buffer，再发送查询指令
    char cmd[] = "AT+STAINFO?\r\n";
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    ret = pUart->Write(pUart, (unsigned char *)cmd, strlen(cmd));
    if(strlen(cmd) != ret) return ret;

    //逐字节读取应答，拿到 +STAINFO:<数字> 即可返回
    char buf[32] = {0};
    unsigned int i = 0;
    while(timeout && i < sizeof(buf)-1)
    {
        unsigned char ch;
        ret = pRxBuffer->Read(pRxBuffer, &ch, 1);
        if(ret == 1)
        {
            buf[i++] = (char)ch;
            buf[i] = '\0';

            //找到 +STAINFO: 后面跟着数字就解析返回
            char *p = strstr(buf, "+STAINFO:");
            if(p)
            {
                p += strlen("+STAINFO:");
                if(*p >= '0' && *p <= '9')
                {
                    int status = 0;
                    while(*p >= '0' && *p <= '9')
                    {
                        status = status * 10 + (*p - '0');
                        p++;
                    }
                    //同步到设备结构体：只有"已连接并获取到IP"(3)才视为连接(1)，其余(0-2,4)均视为未连接(0)
                    gWiFiBtDevice.dev_status = (status == 3) ? 1 : 0;
                    return status;
                }
            }
            if(strstr(buf, "ERR"))  return -EIO;    //芯片报错
        }
        else
        {
            timeout--;
            HAL_Delay(1);
        }
    }

    return -EIO;    //超时没收到有效状态
}

/**
 * @brief 结构体成员版：查询WiFi连接状态（包装 WiFiBTGetStaStatus）
 *
 * @param ptdev   wifi设备对象
 * @param timeout 等待应答超时时间（ms）
 * @return 0-4 连接状态；<0 错误
 */
static int WifiBT_WIFIStaStatus(struct WiFiBtDev *ptdev, unsigned int timeout)
{
    if(NULL == ptdev)   return -EINVAL;
    return WiFiBTGetStaStatus(timeout);
}

/**
 * @brief 解析一行热点信息 "<index> <SSID>,<CH>,<SECURITY>,<RSSI>,<BSSID>"
 *
 * @param line  一行应答文本（已去掉行尾 \r\n）
 * @param pinfo 输出：解析结果
 * @return ESUCCESS 成功；<0 格式不符
 */
static int WiFiScanParseLine(const char *line, WiFiApInfo *pinfo)
{
    const char *p = line;
    const char *q = NULL;
    unsigned int n = 0;

    /* <index>：行首序号，丢弃（数组下标就是顺序） */
    while(' ' == *p)  p++;
    if(*p < '0' || *p > '9')    return -EINVAL;
    while(*p >= '0' && *p <= '9')  p++;
    while(' ' == *p)  p++;

    /* <SSID>：到第一个 ',' */
    q = strchr(p, ',');
    if(NULL == q)   return -EINVAL;
    n = (unsigned int)(q - p);
    if(0 == n || n >= sizeof(pinfo->ssid))  return -EINVAL;
    memcpy(pinfo->ssid, p, n);
    pinfo->ssid[n] = '\0';
    p = q + 1;

    /* <CH>：到 ','（atoi 遇非数字自动停） */
    q = strchr(p, ',');
    if(NULL == q)   return -EINVAL;
    pinfo->channel = (unsigned char)atoi(p);
    p = q + 1;

    /* <SECURITY>：到 ','，超长截断 */
    q = strchr(p, ',');
    if(NULL == q)   return -EINVAL;
    n = (unsigned int)(q - p);
    if(n >= sizeof(pinfo->security))  n = sizeof(pinfo->security) - 1;
    memcpy(pinfo->security, p, n);
    pinfo->security[n] = '\0';
    p = q + 1;

    /* <RSSI>：到 ','，可能带负号 */
    q = strchr(p, ',');
    if(NULL == q)   return -EINVAL;
    pinfo->rssi = (signed char)atoi(p);
    p = q + 1;

    /* <BSSID>：行尾剩余全部 */
    n = strlen(p);
    if(n >= sizeof(pinfo->bssid))  n = sizeof(pinfo->bssid) - 1;
    memcpy(pinfo->bssid, p, n);
    pinfo->bssid[n] = '\0';

    return ESUCCESS;
}

/**
 * @brief 结构体成员版：扫描周围WiFi热点，刷新WiFi列表
 *
 * 指令/应答（AT+WSCAN）：
 *   +WSCAN:index SSID,CH,SECURITY,RSSI,BSSID     ← 表头行，跳过
 *   <index> <SSID>,<CH>,<SECURITY>,<RSSI>,<BSSID> ← 热点行，逐行解析
 *   ...
 *   OK                                            ← 应答结束
 *
 * 注意：pRxBuffer 只有100字节且满则覆盖，应答整体远超100字节，
 * 所以必须边收边逐行解析（流式），不能攒完整个应答再处理。
 *
 * @param ptdev   wifi设备对象
 * @param apList  输出：热点数组（调用方分配）
 * @param maxAp   数组容量（热点数超过容量时多余的行丢弃，但会收完整个应答）
 * @param timeout 空闲超时（ms，只在收不到数据时递减；扫描本身需数秒，建议 8000~10000）
 * @return >=0 实际扫到的热点数；<0 错误（-EINVAL/-ENODEV/-EIO 含超时未收到 OK）
 *
 * 阻塞函数，期间独占 wifi 串口，请在非UI线程或能接受阻塞数秒的任务里调用。
 */
static int WifiBT_WIFIScan(struct WiFiBtDev *ptdev, WiFiApInfo *apList, unsigned char maxAp, unsigned int timeout)
{
    if(NULL == ptdev)               return -EINVAL;
    if(NULL == apList || 0 == maxAp)  return -EINVAL;
    if(NULL == pUart)               return -ENODEV;
    if(NULL == pRxBuffer)           return -ENODEV;

    char cmd[] = "AT+WSCAN\r\n";
    int ret = pRxBuffer->Clear(pRxBuffer);
    if(ESUCCESS != ret) return ret;
    ret = pUart->Write(pUart, (unsigned char *)cmd, strlen(cmd));
    if(strlen(cmd) != ret) return ret;

    char line[96];              /* 单行拼包缓冲（最长行≈90字节，超长部分丢弃） */
    unsigned char li = 0;       /* 当前行已收字节数 */
    unsigned char count = 0;    /* 已解析的热点数 */

    while(timeout)
    {
        unsigned char ch;
        ret = pRxBuffer->Read(pRxBuffer, &ch, 1);
        if(1 == ret)
        {
            if('\n' == ch)      /* 一行收完，去 \r 后分类处理 */
            {
                line[li] = '\0';
                if(li > 0 && '\r' == line[li-1])  line[li-1] = '\0';

                if(line[0] >= '0' && line[0] <= '9')       /* 热点数据行 */
                {
                    if(count < maxAp && ESUCCESS == WiFiScanParseLine(line, &apList[count]))
                        count++;
                    /* 容量已满或解析失败的行：丢弃，但继续收直到 OK，保证应答收干净 */
                }
                else if(0 == strcmp(line, "OK"))           /* 应答结束 */
                {
                    return (int)count;
                }
                else if(NULL != strstr(line, "ERR") || NULL != strstr(line, "Unknown cmd"))
                {
                    return -EIO;
                }
                /* 其余行（+WSCAN 表头、空行等）：跳过 */
                li = 0;
            }
            else if(li < sizeof(line) - 1)
            {
                line[li++] = (char)ch;
            }
            /* 单行超长：不再保存，静默丢弃直到换行 */
        }
        else
        {
            timeout--;         /* 只在收不到数据时递减：连发数据不会吃掉超时 */
            HAL_Delay(1);
        }
    }

    return -EIO;   /* 超时没等到 OK */
}



/**
 * @brief WiFi芯片socket连接节点插入
 * 
 * @param ptnode 要插入的节点
 */
static void NetNodeInsert(NetNode *ptnode)
{    
    if(NULL == gHeadNet)
    {
        gHeadNet = ptnode;
    }
    else
    {
        ptnode->next = gHeadNet;
        gHeadNet = ptnode;
    }
}

/**
 * @brief WiFi芯片socket连接节点寻找
 * 
 * @param port 需寻找的节点的端口号
 */
static NetNode *NetNodeFind(unsigned int port)
{    
    NetNode *ptnode = gHeadNet;
    while(NULL != ptnode)
    {
        if(ptnode->port == port)
        {
            return ptnode;
        }
        ptnode = ptnode->next;
    }
    return NULL;
}

/**
 * @brief WiFi芯片socket连接节点寻找
 * 
 * @param port 需寻找的节点的conID
 */
static NetNode *NetNodeFind_PassConid(unsigned int conID)
{    
    NetNode *ptnode = gHeadNet;
    while(NULL != ptnode)
    {
        if(ptnode->conID == conID)
        {
            return ptnode;
        }
        ptnode = ptnode->next;
    }
    return NULL;
}

/**
 * @brief WiFi芯片socket连接节点删除
 * 
 * @param port 需删除的节点的端口号
 */
static void NetNodeDelete(unsigned int port)
{    
    NetNode *ptnode = gHeadNet;
    NetNode *ptmp = ptnode;
    while(NULL != ptnode)
    {
        if(ptnode->port == port)
        {
            ptmp->next = ptnode->next;
            //free(ptnode->netbuffer);//释放内存
            return;
        }
        ptmp = ptnode;
        ptnode = ptnode->next;
    }
}


// +EVENT:SocketDown,1,3,123
// 比如接收到OK ERROR .....——WiFi芯片发送过来的数据，我们是不进行解析的，我们只解析服务器发送过来的数据
void NetSocketDataAnalysis(unsigned char data)
{
    static unsigned char buf[256];
    static unsigned int i = 0;
    static unsigned char step = 0;
    static unsigned short conID = 0;
    static unsigned short recvLen = 0;
    switch(step)
    {
        case 0: // Find '+'
        {
            if(data != '+') return;
            step = 1;
            buf[i++] = data;
            break;
        }
        case 1: // Find "+EVENT:SocketDown,"
        {
            buf[i++] = data;
            if(strstr((char*)buf, "+EVENT:SocketDown,"))//+EVENT:SocketDown,长度为18  +EVENT:SocketDown,   +abcddff:SocketDown,
            {
                memset((char*)buf, 0, i);
                i = 0;
                step = 2;
            }
            else if(i == 18)
            {
                memset((char*)buf, 0, i);
                i= 0;
                step = 0;
            }
            break;
        }
        case 2: // Get conID
        {
            if(data == ',')
            {
                step = 3;
            }
            else
            {
                conID = conID * 10 + data - '0';
            }
            break;
        }
        case 3: // Get Receive Length
        {
            if(data == ',')
            {
                step = 4;
            }
            else
            {
                recvLen = recvLen * 10 + data - '0';
            }
            break;
        }
        case 4: // Save Socket Data
        {
            NetNode * Nettemp = NetNodeFind_PassConid(conID);//根据conID寻找节点
            if(Nettemp == NULL) break;
            Nettemp->netbuffer->Write(Nettemp->netbuffer, &data, 1);
            
            
            i++;//记录收到的真实数据的个数
            if(i==recvLen)
            {
                i = 0;
                conID = 0;
                recvLen = 0;
                step = 0;
            }
            break;
        }
    }
}
