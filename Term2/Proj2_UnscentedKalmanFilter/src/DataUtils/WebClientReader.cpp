#include "../DataUtils/WebClientReader.h"
#include "../DataUtils/Utils.h"
#include <uWS/uWS.h>
/** run method implements the logic of reading
* data from web socket and processing data
* using Kalman Filter
*/
void DataUtils::WebClientReader::Run() {
  mServerHub_.onMessage([this](uWS::WebSocket<uWS::SERVER> webServer, char *pData, size_t pLength, uWS::OpCode opCode) {
    // parse data using Utils
    Utils mStringUtils_("[", "]", "null");
    // if string has data related to sensor measurement, process it further
    if (mStringUtils_.HasData(pData, pLength))
    {
      // getData after stripping the delimiters
      std::string strData =  mStringUtils_.GetData(std::string(pData));
      if (!strData.empty())
      {
        // parse data in JSON format
        auto pJSONData = nlohmann::json::parse(strData);
        if (pJSONData[0].get<std::string>() == "telemetry")
        {
          // extract sensor measurement from JSON data
          auto clientString = pJSONData[1]["sensor_measurement"];

          // process measurements, ground truth from string
          // Run Kalman Filter and get the predicted state
          auto predictedState = this->ProcessData(clientString);

          // If a state is predicted, calculate RMSE
          if (predictedState) {
            auto RMSE = this->GetApplication()->CalculateRMSE();

            //Output: 'px_est','py_est','vx_est','vy_est','px_meas','py_meas','px_gt','py_gt','vx_gt','vy_gt'
            this->WriteResultsToFile(predictedState);

            // print out RMSE
            std::cout << "RMSE: " << RMSE << std::endl;

            // Write out json data to the client
            nlohmann::json msgJson;
            msgJson["estimate_x"] = predictedState->GetPositionX();
            msgJson["estimate_y"] = predictedState->GetPositionY();
            msgJson["rmse_x"] =  RMSE(0);
            msgJson["rmse_y"] =  RMSE(1);
            msgJson["rmse_vx"] = RMSE(2);
            msgJson["rmse_vy"] = RMSE(3);
            
            auto msg = "42[\"estimate_marker\"," + msgJson.dump() + "]";
            webServer.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          }else{
            std::string msg = "42[\"manual\",{}]";
            webServer.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          }
        }
      } else {
        std::string msg = "42[\"manual\",{}]";
        webServer.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });
  mServerHub_.onHttpRequest([](uWS::HttpResponse * res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      res->end(nullptr, 0);
    }
  });

  mServerHub_.onConnection([this](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  mServerHub_.onDisconnection([this](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (mServerHub_.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
    mServerHub_.run();
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
  }
}