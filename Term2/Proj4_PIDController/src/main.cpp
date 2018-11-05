#include <uWS/uWS.h>
#include <iostream>
#include <math.h>
#include "json.hpp"
#include "PID.h"
#include "InputParser.h"
#include "PIDConfiguration.h"
#include "PIDConfigurationType.h"
#include "PIDConfigurationFactory.h"
// for convenience
using json = nlohmann::json;

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main(int argc, char* argv[])
{
  uWS::Hub h;

  InputParser pParser; 
  pParser.ParseArgs(argc, argv);

  PIDConfigurationFactory pidConfigFactory;

  PIDConfigurationType steerType = PIDConfigurationType::STEER;
  PIDConfiguration steerConfig = pidConfigFactory.GetConfig(steerType); //Constructor overloads available for tweaking init p, i, d values
  steerConfig.SetTwiddleParameters(pParser.GetSteerParams());

  PID steer_pid(steerConfig);
  steer_pid.InitParameters();

  PIDConfigurationType throttleType = PIDConfigurationType::THROTTLE;
  PIDConfiguration throttleConfig = pidConfigFactory.GetConfig(throttleType); //Constructor overloads available for tweaking init p, i, d values
  throttleConfig.SetTwiddleParameters(pParser.GetThrottleParams());

  PID throttle_pid(throttleConfig);
  throttle_pid.InitParameters();

  h.onMessage([&steer_pid, &throttle_pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          const double cte = std::stod(j[1]["cte"].get<std::string>());
          const double speed = std::stod(j[1]["speed"].get<std::string>());
          const double angle = std::stod(j[1]["steering_angle"].get<std::string>());

          steer_pid.UpdateError(cte);
          const double steer_value = steer_pid.TotalError();//init_Kp * p_error + init_Kd * d_error + init_Ki * i_error;
          
          throttle_pid.UpdateError(fabs(cte));
          double throttle_value = throttle_pid.TotalError();
          throttle_value = abs(0.75-abs(throttle_value));
          
          std::cout << "CTE: " << cte << "\tSteer: " << steer_value << "\tThrottle: " << throttle_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle_value;
          //0.3; 
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
