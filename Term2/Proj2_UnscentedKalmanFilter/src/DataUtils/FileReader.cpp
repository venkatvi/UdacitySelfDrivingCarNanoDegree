#include "../DataUtils/FileReader.h"

#include <iostream>
#include <fstream>
void DataUtils::FileReader::Run() {
  m_file_reader_.open(m_file_name_, std::ifstream::in);

  std::string line;
  while (!m_file_reader_.eof()) {
    line.clear();
    getline(m_file_reader_, line);

    // if line starts with "L" or "R"
    if (line[0] == 'L' || line[0] == 'R') {
      auto predictedState = ProcessData(line);
      if (predictedState) {
        auto RMSE = m_application_->CalculateRMSE();
        //Output: "L/R"  'px_est','py_est','vx_est','vy_est',
        // 'px_meas','py_meas',
        // 'px_gt','py_gt','vx_gt','vy_gt'
        WriteResultsToFile(predictedState);
        std::cout << "RMSE: " << RMSE << std::endl;
        //getchar();
      }
    }
  }
  m_file_reader_.close();

  //m_application_->CalculateNIS();
}
