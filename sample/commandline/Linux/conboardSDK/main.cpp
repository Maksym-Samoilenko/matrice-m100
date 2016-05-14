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

void telemetry_data_gps(Request* req, Response* res) {
  BroadcastData bd = script.getApi()->getBroadcastData();
  res->body << bd.pos.longitude << "!" << bd.pos.latitude << "!" <<bd.pos.altitude << "!" << (float)bd.battery << endl;
}

void follow_point_by_gps(Request* req, Response* res) {
    res->body << "Followed by GPS point";
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
        server.get("/takeoff", &takeoff);
        server.get("/landing", &landing);
        server.get("/obtain_control", &obtain_control);
        server.get("/release_control", &release_control);
        server.get("/activate", &activate);
        server.get("/telemetry_data_time", &telemetry_data_time);
        server.get("/telemetry_data_status", &telemetry_data_status);
        server.get("/telemetry_data_battery", &telemetry_data_battery);
        server.get("/follow_point", &follow_point);
        server.get("/follow_point_by_gps", &follow_point_by_gps);
        server.get("/telemetry_data_gps", &telemetry_data_gps);
        server.start(5000);
    } catch(WPP::Exception e) {
        std::cerr << "WebServer: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
