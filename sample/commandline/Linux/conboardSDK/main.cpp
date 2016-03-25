#include <iostream>

#include "DJIHardDriverManifold.h"
#include "conboardsdktask.h"
#include "APIThread.h"
#include <iostream>
#include <map>
#include "web++.hpp"
using namespace std;
using namespace WPP;

#define make_vers(a, b, c, d)                                                               \
    (((a << 24) & 0xff000000) | ((b << 16) & 0x00ff0000) | ((c << 8) & 0x0000ff00) |           \
     (d & 0x000000ff))

HardDriverManifold driver("/dev/ttyTHS1", 230400);
driver.init();
CoreAPI api(&driver);
ConboardSDKScript script(&api);
APIThread *send = new APIThread(api, 1, port);
APIThread *read = new APIThread(api, 2, port);
send->start();
read->start();



const int version = (make_vers(3, 1, 10, 0));

const char * get_index_html(){
    return "  <!DOCTYPE html>\n" +
				"  <html>\n" +
				"  <head>\n" +
				"      <script src=\"http://code.jquery.com/jquery-latest.min.js\"\n" +
				"              type=\"text/javascript\"></script>\n" +
				"\n" +
				"      <script type=\"text/javascript\">\n" +
				"          console.log('init');\n" +
				"\n" +
				"          function takeoff(){\n" +
				"              console.log(\"takeoff\");\n" +
				"              $.ajax({\n" +
				"                  url: \"takeoff\",\n" +
				"                  success: function( data ) {\n" +
				"                    console.log(data);\n" +
				"                      console.log('takeoff ok')\n" +
				"                  }\n" +
				"              });\n" +
				"          }\n" +
				"\n" +
				"          function release(){\n" +
				"              console.log(\"release\");\n" +
				"              $.ajax({\n" +
				"                  url: \"/release\",\n" +
				"                  success: function( data ) {\n" +
				"                    console.log(data);\n" +
				"                      console.log('release ok')\n" +
				"                  }\n" +
				"              });\n" +
				"          }\n" +
				"\n" +
				"          function obtain(){\n" +
				"              console.log(\"obtain control\");\n" +
				"              $.ajax({\n" +
				"                  url: \"/obtain\",\n" +
				"                  success: function( data ) {\n" +
				"                    console.log(data);\n" +
				"                      console.log('obtain control ok')\n" +
				"                  }\n" +
				"              });\n" +
				"          }\n" +
				"\n" +
				"          function landing(){\n" +
				"              console.log(\"landing\");\n" +
				"              $.ajax({\n" +
				"                  url: \"/landing\",\n" +
				"                  success: function( data ) {\n" +
				"                    console.log(data);\n" +
				"                      console.log('landing ok')\n" +
				"                  }\n" +
				"              });\n" +
				"          }\n" +
				"\n" +
				"          function activate(){\n" +
				"              console.log(\"activate\");\n" +
				"              $.ajax({\n" +
				"                  url: \"/activate\",\n" +
				"                  success: function( data ) {\n" +
				"                    console.log(data);\n" +
				"                      console.log('activate ok')\n" +
				"                  }\n" +
				"              });\n" +
				"          }\n" +
				"\n" +
				"          function telemetry_data_time(){\n" +
				"              console.log(\"telemetry_data_time\");\n" +
				"              $.ajax({\n" +
				"                  url: \"/telemetry_data_time\",\n" +
				"                  success: function( data ) {\n" +
				"                    alert.log(data);\n" +
				"                      console.log('telemetry_data_status ok')\n" +
				"                  }\n" +
				"              });\n" +
				"          }\n" +
				"\n" +
				"          function telemetry_data_status(){\n" +
				"              console.log(\"telemetry_data_status\");\n" +
				"              $.ajax({\n" +
				"                  url: \"/telemetry_data_status\",\n" +
				"                  success: function( data ) {\n" +
				"                    alert.log(data);\n" +
				"                      console.log('telemetry_data_status ok')\n" +
				"                  }\n" +
				"              });\n" +
				"          }\n" +
				"\n" +
				"          function telemetry_data_battery(){\n" +
				"              console.log(\"telemetry_data_battery\");\n" +
				"              $.ajax({\n" +
				"                  url: \"/telemetry_data_battery\",\n" +
				"                  success: function( data ) {\n" +
				"                       alert.log(data);\n" +
				"                      console.log('telemetry_data_battery ok')\n" +
				"                  }\n" +
				"              });\n" +
				"          }\n" +
				"      </script>\n" +
				"  </head>\n" +
				"  <body>\n" +
				"<button onclick=\"activate()\">ACTIVATE</button>\n" +
				"<button onclick=\"obtain()\">OBTAIN CONTROL</button>\n" +
				"<button onclick=\"release()\">RELEASE CONTROL</button>\n" +
				"<button onclick=\"takeoff()\">START</button>\n" +
				"<button onclick=\"telemetry_data_time()\">TIME</button>\n" +
				"<button onclick=\"telemetry_data_status()\">STATUS</button>\n" +
				"<button onclick=\"telemetry_data_battery()\">BATTERY</button>\n" +
				"</body>\n" +
				"</html>";
}

void index(Request* req, Response* res) {
    //globalDJIScript.addTask("CA");
    //globalDJIScript.run();
    res->body << get_index_html();
}
void takeoff(Request* req, Response* res) {
    script.getFlight()->task((DJI::onboardSDK::Flight::TASK)4);
    res->body << "takeoff";
}
void landing(Request* req, Response* res) {
    script.getFlight()->task((DJI::onboardSDK::Flight::TASK)6);
    res->body << "landing";
}
void activate(Request* req, Response* res) {
    script.adata.ID = 2;//ID
    script.adata.reserved = 2;
    script.adata.version = version;
    script.adata.encKey = "key";//KEy
    script.getApi()->activate(&script.adata);
    res->body << "activated";
}
void release_control(Request* req, Response* res) {
    script.getApi()->setControl(false);
    res->body << "release_control";
}
void obtain_control(Request* req, Response* res) {
    script.getApi()->setControl(true);
    res->body << "obtain_control";
}

void telemetry_data_time(Request* req, Response* res) {
    //Telemetry
    BroadcastData bd = script.getApi()->getBroadcastData();
    res->body << bd.timeStamp.time;
}

void telemetry_data_status(Request* req, Response* res) {
    //Telemetry
    BroadcastData bd = script.getApi()->getBroadcastData();

    res->body << (int)bd.status;
}

void telemetry_data_battery(Request* req, Response* res) {
    //Telemetry
    BroadcastData bd = script.getApi()->getBroadcastData();

    res->body << (int)bd.battery;
}


int main(int argc, const char* argv[]) {

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
        server.start(5000);
    } catch(WPP::Exception e) {
        std::cerr << "WebServer: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
