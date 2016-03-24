#include <iostream>

#include "DJIHardDriverManifold.h"
#include "conboardsdktask.h"
#include "APIThread.h"
#include <iostream>
#include <map>
#include "web++.hpp"
using namespace std;
using namespace WPP;

HardDriverManifold driver("/dev/ttyTHS1", 230400);
CoreAPI api(&driver);
ConboardSDKScript script(&api);
const int SDK_VERSION = (MAKE_VERSION(3, 1, 10, 0));

const char * get_index_html(){
    return "  <!DOCTYPE html>\n"
            "  <html>\n"
            "  <head>\n"
            "      <script src=\"http://code.jquery.com/jquery-latest.min.js\"\n"
            "              type=\"text/javascript\"></script>\n"
            "      <script type=\"text/javascript\">\n"
            "          console.log('init');\n"
            "          function takeoff(){\n"
            "              console.log(\"takeoff\");\n"
            "              $.ajax({\n"
            "                  url: \"takeoff\",\n"
            "                  success: function( data ) {\n"
            "                      console.log('takeoff ok')\n"
            "                  }\n"
            "              });\n"
            "          }\n"
            "          function release(){\n"
            "              console.log(\"release\");\n"
            "              $.ajax({\n"
            "                  url: \"/release\",\n"
            "                  success: function( data ) {\n"
            "                      console.log('release ok')\n"
            "                  }\n"
            "              });\n"
            "          }\n"
            "          function obtain(){\n"
            "              console.log(\"obtain control\");\n"
            "              $.ajax({\n"
            "                  url: \"/obtain\",\n"
            "                  success: function( data ) {\n"
            "                      console.log('obtain control ok')\n"
            "                  }\n"
            "              });\n"
            "          }\n"
            "          function landing(){\n"
            "              console.log(\"landing\");\n"
            "              $.ajax({\n"
            "                  url: \"/landing\",\n"
            "                  success: function( data ) {\n"
            "                      console.log('landing ok')\n"
            "                  }\n"
            "              });\n"
            "          }\n"
            "          function activate(){\n"
            "              console.log(\"activate\");\n"
            "              $.ajax({\n"
            "                  url: \"/activate\",\n"
            "                  success: function( data ) {\n"
            "                      console.log('activate ok')\n"
            "                  }\n"
            "              });\n"
            "          }\n"
            "      </script>\n"
            "  </head>\n"
            "  <body>\n"
            "<button onclick=\"activate()\">ACTIVATE</button>\n"
            "<button onclick=\"obtain()\">OBTAIN CONTROL</button>\n"
            "<button onclick=\"release()\">RELEASE CONTROL</button>\n"
            "  <button onclick=\"takeoff()\">START</button>\n"
            "  <button onclick=\"landing()\">FINISH</button>\n"
            "  </body>\n"
            "  </html>";
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
    script->adata.ID = id;//ID
    script->adata.reserved = 2;
    script->adata.version = SDK_VERSION;
    script->adata.encKey = key;//KEy
    script.getApi()->activate(&script->adata);
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
    driver.init();
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
