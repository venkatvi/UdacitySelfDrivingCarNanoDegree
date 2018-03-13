#include "../DataUtils/DataReader.h"
void DataUtils::DataReader::WriteResultsToFile(
    const Data::State* pState) {
	auto currentMeasurement = m_data_adapter_->GetMeasurement();
	auto currentGroundTruth = m_data_adapter_->GetGroundTruth();

	// Output measurement type "L" or "R"
	m_output_data_stream_ << currentMeasurement->GetMeasurementType() << ", ";
	m_output_data_stream_ << *pState;
	m_output_data_stream_ << *currentMeasurement;
	m_output_data_stream_ << *currentGroundTruth;
	m_output_data_stream_ << std::endl;
}
const Data::State* DataUtils::DataReader::ProcessData(
    const std::string& pData) {
	m_data_adapter_->SetData(pData);
	m_data_adapter_->ParseData();

	auto currentMeasurement = m_data_adapter_->GetMeasurement();
	auto currentGroundTruth = m_data_adapter_->GetGroundTruth();

	if (currentMeasurement && currentGroundTruth) {
		auto predictedState = m_application_->Run(currentMeasurement,
		                      currentGroundTruth);
		return predictedState;
	}
	return nullptr;
}