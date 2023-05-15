#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "control.h"

#define ServoPort D3  
//SSID and Password of your WiFi router
const char* ssid = "Redmi9C";
const char* password = "11112222";
//const char* ssid = "Wifi FREE";
//const char* password = "comatkhau";

Servo myservo; 
ESP8266WebServer server(80);

//LED Connections
const int BlueLED = 16; // D0
const int RedLED = 5;    // D1 
const int GreenLED = 4; // D2

//=======================================================================
//                    xử lý trang chính
//=======================================================================
void handleRoot() {
  Serial.println("Root Page Requested");
  server.send(200, "text/html", MAIN_page);
}

//=======================================================================
//                    Xử lý bộ màu
//=======================================================================
void handleForm() {
  //Màu sắc riêng biệt được gửi qua javascript
  String red = server.arg("r");
  String green = server.arg("g");
  String blue = server.arg("b");
  int r = red.toInt();
  int g = green.toInt();
  int b = blue.toInt();

  Serial.print("Red:");Serial.println(r);
  Serial.print("Green:");Serial.println(g);
  Serial.print("Blue:");Serial.println(b);
  
  //ESP hỗ trợ analogWrite Tất cả IO đều là PWM
  analogWrite(RedLED,r);
  analogWrite(GreenLED,g);
  analogWrite(BlueLED,b);

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Updated-- Press Back Button");
 
  delay(500);  
}
void handleServo(){

  String POS = server.arg("servoPOS");

  int pos = POS.toInt();

  myservo.write(pos);   //--> Di chuyển động cơ servo theo giá trị POS

  delay(15);

  Serial.print("Servo Angle:");

  Serial.println(pos);

  server.send(200, "text/plane","");

}
void handleFeed(){

  myservo.write(110);   //--> Di chuyển động cơ servo theo giá trị POS

  delay(1000);

  myservo.write(45);

  server.send(200, "text/plane","");

}
void handleTurnoff(){

  analogWrite(RedLED,LOW);
  analogWrite(GreenLED,LOW);
  analogWrite(BlueLED,LOW);

  delay(1000);

  server.send(200, "text/plane","");

}
//=======================================================================
//                    CÀI ĐẶT
//=======================================================================
void setup(){
  Serial.begin(115200);   //Bắt đầu kết nối nối tiếp  
  delay(500);
  myservo.attach(ServoPort);
  
  pinMode(RedLED,OUTPUT);
  pinMode(GreenLED,OUTPUT);
  pinMode(BlueLED,OUTPUT);
  
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);     //Kết nối với bộ định tuyến WiFi của bạn
  Serial.println("connecting");
  
  // Chờ kết nối
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  

  //Nếu kết nối thành công hiển thị địa chỉ IP trong màn hình nối tiếp
  Serial.println("");
  Serial.print("Connected to ");  
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Địa chỉ IP được gán cho ESP của bạn
  
  server.on("/", handleRoot);  //Liên kết hàm xử lý với đường dẫn
  server.on("/setRGB",handleForm);
  server.on("/setPOS",handleServo);
  server.on("/feed",handleFeed);
  server.on("/turnoff",handleTurnoff);
    
  server.begin();                           //Bắt đầu server
  Serial.println("HTTP server started");
}
//=======================================================================
//                    LOOP
//=======================================================================
void loop(){
  server.handleClient();
}