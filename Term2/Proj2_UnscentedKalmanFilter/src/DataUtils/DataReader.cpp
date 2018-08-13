#include "../DataUtils/DataReader.h"
void DataUtils::DataReader::WriteResultsToFile(
    const Data::State* pState) {
	auto currentMeasurement = m_data_adapter_->GetMeasurement();
	auto currentGroundTruth = m_data_adapter_->GetGroundTruth();

	// Output measurement type "L" or "R"
	if (currentMeasurement && currentGroundTruth) {
		m_output_data_stream_ << *pState;
		m_output_data_stream_ << *currentMeasurement;
		m_output_data_stream_ << *currentGroundTruth;
		m_output_data_stream_ << m_application_->GetCurrentNIS();
		m_output_data_stream_ << std::endl;
	}
}
const Data::State* DataUtils::DataReader::ProcessData(
    const std::string& pData) {
	m_data_adapter_->SetData(pData);
	m_data_adapter_->ParseData();

	auto currentMeasurement = m_data_adapter_->GetMeasurement();
	auto currentGroundTruth = m_data_adapter_->GetGroundTruth();

	std::cout << "--------------------------------" << std::endl;
	if (currentMeasurement && currentGroundTruth) {
		std::cout << "Raw Measurement: " << *currentMeasurement << std::endl;
		std::cout << "Measurement:" << std::endl << currentMeasurement->GetVectorizedData() << std::endl;
		std::cout << "GT: " << *currentGroundTruth << std::endl;
		auto predictedState = m_application_->Run(currentMeasurement,
		                      currentGroundTruth);
		std::cout << "Pred: " << *predictedState << std::endl;
        std::cout << "--------------------------------" << std::endl;
        return predictedState;
	}
	
	return nullptr;
}