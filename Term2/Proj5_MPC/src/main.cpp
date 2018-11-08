#include <math.h>
#include <uWS/uWS.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "MPC.h"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.rfind("}]");
  if (found_null != string::npos) {
    return "";
  } else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 2);
  }
  return "";
}

// Evaluate a polynomial.
double polyeval(Eigen::VectorXd coeffs, double x) {
  double result = 0.0;
  for (int i = 0; i < coeffs.size(); i++) {
    result += coeffs[i] * pow(x, i);
  }
  return result;
}

// Fit a polynomial.
// Adapted from
// https://github.com/JuliaMath/Polynomials.jl/blob/master/src/Polynomials.jl#L676-L716
Eigen::VectorXd polyfit(Eigen::VectorXd xvals, Eigen::VectorXd yvals,
                        int order) {
  assert(xvals.size() == yvals.size());
  assert(order >= 1 && order <= xvals.size() - 1);
  Eigen::MatrixXd A(xvals.size(), order + 1);

  for (int i = 0; i < xvals.size(); i++) {
    A(i, 0) = 1.0;
  }

  for (int j = 0; j < xvals.size(); j++) {
    for (int i = 0; i < order; i++) {
      A(j, i + 1) = A(j, i) * xvals(j);
    }
  }

  auto Q = A.householderQr();
  auto result = Q.solve(yvals);
  return result;
}

int main() {
  uWS::Hub h;

  // MPC is initialized here!
  MPC mpc;

  h.onMessage([&mpc](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    string sdata = string(data).substr(0, length);
    cout << sdata << endl;
    if (sdata.size() > 2 && sdata[0] == '4' && sdata[1] == '2') {
      string s = hasData(sdata);
      if (s != "") {
        auto j = json::parse(s);
        string event = j[0].get<string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          vector<double> ptsx = j[1]["ptsx"];
          vector<double> ptsy = j[1]["ptsy"];
          double px = j[1]["x"];
          double py = j[1]["y"];
          double psi = j[1]["psi"];
          double v = j[1]["speed"];

          /* STEP1: Line fit 
          * Using the points received from simulator fit a track line and compute CTE
          */

          // 1.1 Transform data to (0, 0) for a good line fit 
          vector<double> waypoints_x; 
          vector<double> waypoints_y;
          for (int i=0; i< ptsx.size(); i++){
            // subtract x and y component 
            double shift_x = ptsx[i] - px;
            double shift_y = ptsy[i] - py;

            // rotation about origin to right to make psi 0
            waypoints_x.push_back(shift_x * cos(0 - psi) - shift_y * sin(0 - psi));
            waypoints_y.push_back(shift_x * sin(0 - psi) + shift_y * cos(0 - psi));
          }

          // 1.2 Converter to transform double vector to Eigen::Vector to be used with poly fit
          double* xPtr = &waypoints_x[0];
          Eigen::Map<Eigen::VectorXd> ptsx_transform(xPtr, 6); 

          double* yPtr = &waypoints_y[0];
          Eigen::Map<Eigen::VectorXd> ptsy_transform(yPtr, 6); 

          // 1.3  Get line fit coefficients from poly fit 
          auto coeffs = polyfit(ptsx_transform, ptsy_transform, 3);

          // 1.4 Calculate CTE by fitting (0, 0) point with the line coefficients
          double cte = polyeval(coeffs, 0); 

          /* 1.5 Calculate ePSI by calculating angle 
          * Use first coefficient since starting point is 0,0 and other components 
          * will cancel out 
          */
          double epsi = -atan(coeffs[1]); 

          /* STEP 2: Compute new trajectory using Model Predictive Control Algorithm 
          * given the CTE and epsi
          */
          // 2.1 Initialize state 
          Eigen::VectorXd state(6); 

          // Set x, y = 0, 0; psi = 0 (due to previous transformations)
          state << 0, 0, 0, v, cte, epsi;


          // 2.2 Solve for new trajectory points using MPC 
          auto vars = mpc.Solve(state, coeffs);

          // Get desired steering angle and throttle value 
          double predicted_steer_value = vars[0];
          double predicted_throttle_value = vars[1]; 

          /* STEP 3: Display Projected Trajectory and Reference Line 
          */ 
          // 3.1 Green line 
          vector<double> mpc_x_vals;
          vector<double> mpc_y_vals;

          // MPC provides (x,y) values as alternating values of a vector
          for (int i = 2; i < vars.size(); i++){
            if( i%2 == 0){
              mpc_x_vals.push_back(vars[i]);
            }
            else{
              mpc_y_vals.push_back(vars[i]);
            }
          }

          // 3.2 Display the waypoints/reference line
          // Yellow line 
          vector<double> next_x_vals;
          vector<double> next_y_vals;

          int num_points = 100; 
          double interval = 0.25; 
          for (int i = 0; i < num_points; i+=3){
            next_x_vals.push_back(i * interval) ; 
            next_y_vals.push_back(polyeval(coeffs, i  * interval)); 
          }

          
          /* STEP 4: SEND Output to Simulator 
          */
          json msgJson;

          // Divide by deg2rad(25) before sending steering value to simulator for value to be [-1, 1]
          msgJson["steering_angle"] = predicted_steer_value/(deg2rad(25));
          msgJson["throttle"] = predicted_throttle_value;

          //.. add (x,y) points of the mpc predicted trajectory, 
          // points are in reference to the vehicle's coordinate system,
          // the points in the simulator are connected by a Green line

          msgJson["mpc_x"] = mpc_x_vals;
          msgJson["mpc_y"] = mpc_y_vals;

          
          //.. add (x,y) points of the reference trajectory, 
          // points are in reference to the vehicle's coordinate system
          // the points in the simulator are connected by a Yellow line

          msgJson["next_x"] = next_x_vals;
          msgJson["next_y"] = next_y_vals;


          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          // Latency
          // The purpose is to mimic real driving conditions where
          // the car does actuate the commands instantly.
          this_thread::sleep_for(chrono::milliseconds(100));
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the
  // program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data,
                     size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1) {
      res->end(s.data(), s.length());
    } else {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
