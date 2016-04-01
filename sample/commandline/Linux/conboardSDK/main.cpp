#include <iostream>

#include "DJIHardDriverManifold.h"
#include "conboardsdktask.h"
#include "APIThread.h"
#include <iostream>
#include <map>
#include "web++.hpp"
#include "cmdIO.h"
#include "cmdFollow.h"
#include <string>
using namespace std;
using namespace WPP;
using namespace DJI::onboardSDK;
#define make_vers(a, b, c, d)                                                               \
    (((a << 24) & 0xff000000) | ((b << 16) & 0x00ff0000) | ((c << 8) & 0x0000ff00) |           \
     (d & 0x000000ff))
HardDriverManifold driver("/dev/ttyUSB0", 230400);
CoreAPI api(&driver);
ConboardSDKScript script(&api);

const int version = (make_vers(3, 1, 10, 0));

const char * get_index_html(){
}

void index(Request* req, Response* res) {
    //globalDJIScript.addTask("CA");
    //globalDJIScript.run();
    res->body << std::string("  <!DOCTYPE html>\n") +
                "  <html>\n" +
                "  <head>\n" +
                "      <script src=\"http://code.jquery.com/jquery-latest.min.js\"\n" +
                "              type=\"text/javascript\"></script>\n" +
                "\n" +
                "      <script type=\"text/javascript\">\n" +
                "          console.log(\"init\");\n" +
                "\n" +
                "          function takeoff(){\n" +
                "              console.log(\"takeoff\");\n" +
                "              $.ajax({\n" +
                "                  url: \"takeoff\",\n" +
                "                  success: function( data ) {\n" +
                "                      console.log(\"takeoff ok\")\n" +
                "                  }\n" +
                "              });\n" +
                "          }\n" +
                "\n" +
                "          function release(){\n" +
                "              console.log(\"release\");\n" +
                "              $.ajax({\n" +
                "                  url: \"/release_control\",\n" +
                "                  success: function( data ) {\n" +
                "                      console.log(\"release ok\")\n" +
                "                  }\n" +
                "              });\n" +
                "          }\n" +
                "\n" +
                "          function obtain(){\n" +
                "              console.log(\"obtain control\");\n" +
                "              $.ajax({\n" +
                "                  url: \"/obtain_control\",\n" +
                "                  success: function( data ) {\n" +
                "                      console.log(\"obtain control ok\")\n" +
                "                  }\n" +
                "              });\n" +
                "          }\n" +
                "\n" +
                "          function landing(){\n" +
                "              console.log(\"landing\");\n" +
                "              $.ajax({\n" +
                "                  url: \"/landing\",\n" +
                "                  success: function( data ) {\n" +
                "                      console.log(\"landing ok\")\n" +
                "                  }\n" +
                "              });\n" +
                "          }\n" +
                "\n" +
                "          function activate(){\n" +
                "              console.log(\"activate\");\n" +
                "              $.ajax({\n" +
                "                  url: \"/activate\",\n" +
                "                  success: function( data ) {\n" +
                "                      console.log(\"activate ok\")\n" +
                "                  }\n" +
                "              });\n" +
                "          }\n" +
                "\n" +
                "          function telemetry_data_time(){\n" +
                "              console.log(\"telemetry_data_time\");\n" +
                "              $.ajax({\n" +
                "                  url: \"/telemetry_data_time\",\n" +
                "                  success: function( data ) {\n" +
                "                      alert(data);\n" +
                "                  }\n" +
                "              });\n" +
                "          }\n" +
                "\n" +
                "          function telemetry_data_status(){\n" +
                "              console.log(\"telemetry_data_status\");\n" +
                "              $.ajax({\n" +
                "                  url: \"/telemetry_data_status\",\n" +
                "                  success: function( data ) {\n" +
                "                      alert(data);\n" +
                "                  }\n" +
                "              });\n" +
                "          }\n" +
                "\n" +
                "          function telemetry_data_battery(){\n" +
                "              console.log(\"telemetry_data_battery\");\n" +
                "              $.ajax({\n" +
                "                  url: \"/telemetry_data_battery\",\n" +
                "                  success: function( data ) {\n" +
                "                      alert(data);\n" +
                "                  }\n" +
                "              });\n" +
                "          }\n" +
                "          function follow_point(){\n" +
                "              console.log(\"follow_point\");\n" +
                "              $.ajax({\n" +
                "                  url: \"/follow_point?x=\"  + $(\"#x_selector\").val() + \"&y=\" + $(\"#y_selector\").val() +\"&z=\" + $(\"#z_selector\").val() +\"&yaw=\" + $(\"#yaw_selector\").val()" + ",\n"
                "                  success: function( data ) {\n" +
                "                      console.log(data);\n" +
                "                  }\n" +
                "              });\n" +
                "          }\n" +
                "      </script>\n" +
                "  </head>\n" +
                "  <body>\n" +
                "<button onclick=\"activate()\">ACTIVATE</button>\n" +
                "<button onclick=\"obtain()\">OBTAIN CONTROL</button>\n" +
                "<button onclick=\"release()\">RELEASE CONTROL</button>\n" +
                "  <button onclick=\"takeoff()\">START</button>\n" + "<button onclick=\"landing()\">LANDING</button>\n" +

                "  <button onclick=\"telemetry_data_time()\">TIME</button>\n" +
                "  <button onclick=\"telemetry_data_status()\">STATUS</button>\n" +
                "  <button onclick=\"telemetry_data_battery()\">BATTERY</button><br>\n" +

          "<p>X</p><select name=\"x_sel\" id=\"x_selector\"><option value=\"0\">0</option><option value=\"0.25\">0.25</option><option value=\"0.5\">0.5</option><option value=\"0.75\">0.75</option><option value=\"1\">1</option><option value=\"1.25\">1.25</option><option value=\"1.5\">1.5</option><option value=\"1.75\">1.75</option><option value=\"2\">2</option><option value=\"-0.25\">-0.25</option><option value=\"-0.5\">-0.5</option><option value=\"-0.75\">-0.75</option><option value=\"-1\">-1</option><option value=\"-1.25\">-1.25</option><option value=\"-1.5\">-1.5</option><option value=\"-1.75\">-1.75</option><option value=\"-2\">-2</option></select>\n"+
          "<p>Y</p><select name=\"y_sel\" id=\"y_selector\"><option value=\"0\">0</option><option value=\"0.25\">0.25</option><option value=\"0.5\">0.5</option><option value=\"0.75\">0.75</option><option value=\"1\">1</option><option value=\"1.25\">1.25</option><option value=\"1.5\">1.5</option><option value=\"1.75\">1.75</option><option value=\"2\">2</option><option value=\"-0.25\">-0.25</option><option value=\"-0.5\">-0.5</option><option value=\"-0.75\">-0.75</option><option value=\"-1\">-1</option><option value=\"-1.25\">-1.25</option><option value=\"-1.5\">-1.5</option><option value=\"-1.75\">-1.75</option><option value=\"-2\">-2</option></select>\n"+
          "<p>Z</p><select name=\"z_sel\" id=\"z_selector\"><option value=\"0\">0</option><option value=\"0.25\">0.25</option><option value=\"0.5\">0.5</option><option value=\"0.75\">0.75</option><option value=\"1\">1</option><option value=\"1.25\">1.25</option><option value=\"1.5\">1.5</option><option value=\"1.75\">1.75</option><option value=\"2\">2</option><option value=\"-0.25\">-0.25</option><option value=\"-0.5\">-0.5</option><option value=\"-0.75\">-0.75</option><option value=\"-1\">-1</option><option value=\"-1.25\">-1.25</option><option value=\"-1.5\">-1.5</option><option value=\"-1.75\">-1.75</option><option value=\"-2\">-2</option></select>\n"+
          "<p>YAW</p><select name=\"yaw_sel\" id=\"yaw_selector\"><option value=\"0\">0</option><option value=\"0.25\">0.25</option><option value=\"0.5\">0.5</option><option value=\"0.75\">0.75</option><option value=\"1\">1</option><option value=\"1.25\">1.25</option><option value=\"1.5\">1.5</option><option value=\"1.75\">1.75</option><option value=\"2\">2</option><option value=\"-0.25\">-0.25</option><option value=\"-0.5\">-0.5</option><option value=\"-0.75\">-0.75</option><option value=\"-1\">-1</option><option value=\"-1.25\">-1.25</option><option value=\"-1.5\">-1.5</option><option value=\"-1.75\">-1.75</option><option value=\"-2\">-2</option></select>\n"+
          "  <button onclick=\"follow_point()\">FOLLOW</button>\n" +
                "  </body>\n" +
                "  </html>\n";
}
void takeoff(Request* req, Response* res) {
  script.getFlight()->task((DJI::onboardSDK::Flight::TASK)4);
  sleep(3);
  cout << "takeoff" << endl;
  res->body << "takeoff";
}
void landing(Request* req, Response* res) {
  script.getFlight()->task((DJI::onboardSDK::Flight::TASK)6);
  sleep(3);
  cout << "landing"  << endl;
  res->body << "landing";
}
void activate(Request* req, Response* res) {
  script.adata.ID = 1005002;//ID
  script.adata.reserved = 2;
  script.adata.version = version;
  script.adata.encKey = "390b75d49d51e9cea4f8724bfc4b1549eb2a325c3239d1e3b6ea3cccd37b7b9f";//KEy

  script.getApi()->activate(&script.adata);
  sleep(3);
  cout << "activated" << endl;
  res->body << "activated";
}
void release_control(Request* req, Response* res) {
  script.getApi()->setControl(false);
  cout << "release control" << endl;
  sleep(3);
  res->body << "release_control";
}
void obtain_control(Request* req, Response* res) {
  cout << "obtain control function" << endl;
  script.getApi()->setControl(true);
  sleep(3);
  cout << "control obtained" << endl;
  res->body << "obtain_control";
}

void telemetry_data_time(Request* req, Response* res) {
  BroadcastData bd =(&script)->getApi()->getBroadcastData();
  sleep(3);
  cout << "time" << endl;
  res->body << bd.timeStamp.time;
}

void telemetry_data_status(Request* req, Response* res) {
  script.getApi()->setSyncFreq(1);
  BroadcastData bd = script.getApi()->getBroadcastData();
  sleep(3);
  cout << "status" << endl;
  res->body << bd.status;
}

void telemetry_data_battery(Request* req, Response* res) {
  BroadcastData bd = script.getApi()->getBroadcastData();
  sleep(1);
  res->body << (float)bd.battery;
}

void follow_point(Request* req, Response* res) {
FlightData data;
data.flag = 0x90;
std::map<std::string, std::string>::iterator iter;
for (iter = req->query.begin(); iter != req->query.end(); ++iter) {
std::string val = iter->first;
  if(val.compare("x") == 0){
    data.x = stod(iter->second);
    std::cout << iter->first<< " = " << iter->second  << "  " << data.x << std::endl;
  }
  else if(val.compare("y") == 0){
    data.y = stod(iter->second);
    std::cout << iter->first<< " = " << iter->second << "  " << data.y << std::endl;
  }
  else if(val.compare("z") == 0){
    data.z = stod(iter->second);
    std::cout << iter->first<< " = " << iter->second << "  " << data.z  << std::endl;
  }
  else if(val.compare("yaw") == 0){
    data.yaw= stod(iter->second);
    std::cout << iter->first<< " = " << iter->second  << "  " << data.yaw << std::endl;
  }
}
  script.getFlight()->setFlight(&data);
  sleep(1);
  res->body << "follow mission";
}


int main(int argc, const char* argv[]) {
    driver.init();
    APIThread send(&api, 1);
    APIThread read(&api, 2);
    send.createThread();
    read.createThread();
    try {
        std::cout << "Listening on port 5000" << std::endl;
        WPP::Server server;
        server.get("/", &index);
        server.get("/takeoff", &takeoff);
        server.get("/landing", &landing);
        server.get("/obtain_control", &obtain_control);
        server.get("/release_control", &release_control);
        server.get("/activate", &activate);
        server.get("/telemetry_data_time", &telemetry_data_time);
        server.get("/telemetry_data_status", &telemetry_data_status);
        server.get("/telemetry_data_battery", &telemetry_data_battery);
        server.get("/follow_point", &follow_point);
        server.start(5000);
    } catch(WPP::Exception e) {
        std::cerr << "WebServer: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
