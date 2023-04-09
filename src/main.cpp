/*
接线说明:无

程序说明:通过远程HTTP链接进行OTA

注意事项:1):这是借鉴的别人的,问题是不知道如何生成远程链接
         2):bin文件(二进制文件)在对应platformIO文件下的 .pio\build\esp32-s3-devkitc-1 文件夹中的firmware.bin

函数示例:无

作者:灵首

时间:2023_4_7

*/
#include <WiFi.h>
#include <HTTPUpdate.h>

/**********根据实际修改**********/
const char *wifi_ssid = "haoze1029";        // WIFI名称，区分大小写，不要写错
const char *wifi_password = "12345678";     // WIFI密码
//远程固件链接，只支持http
const char *ota_url = "http://media-1251347578.cos.ap-beijing.myqcloud.com/firmware.bin";
/**********根据实际修改**********/



/*
# brief   当升级开始时，打印日志
# param   无
# retval   无
*/
void update_started()
{
    Serial.println("CALLBACK:  HTTP update process started");
}

//当升级结束时，打印日志
void update_finished()
{
    Serial.println("CALLBACK:  HTTP update process finished");
}



/*
# brief   当升级中，打印日志
# param  无
# retval  无
*/
void update_progress(int cur, int total)
{
    Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes[%.1f%%]...\n", cur, total, cur * 100.0 / total);
}



/*
# brief   当升级失败时，打印日志
# param   无
# retval    无
*/
void update_error(int err)
{
    Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}



/**
* @brief 固件升级函数，通过http请求获取远程固件，实现升级
*
* @param update_url 待升级远程固件bin文件的地址
* @return t_httpUpdate_return 升级最终状态
*/
t_httpUpdate_return updateBin(const char *update_url)
{
    Serial.println("start update");
    WiFiClient UpdateClient;

    httpUpdate.onStart(update_started);     //当升级开始时
    httpUpdate.onEnd(update_finished);      //当升级结束时
    httpUpdate.onProgress(update_progress); //当升级中
    httpUpdate.onError(update_error);       //当升级失败时

    t_httpUpdate_return ret = httpUpdate.update(UpdateClient, update_url);

    return ret;
}



void setup()
{
  //串口设置
  Serial.begin(9600);
  Serial.print("serial is OK!!!");

  //连接WiFi
  Serial.print("Connection WIFI");
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED){
    //等待连接wifi
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected !!!\n");

  //开始升级
  t_httpUpdate_return ret = updateBin(ota_url);
  switch (ret){
      case HTTP_UPDATE_FAILED: //当升级失败
      Serial.println("[update] Update failed.");
      break;
      case HTTP_UPDATE_NO_UPDATES: //当无升级
      Serial.println("[update] Update no Update.");
      break;
      case HTTP_UPDATE_OK: //当升级成功
      Serial.println("[update] Update ok.");
      break;
  }
}



void loop(){

}
