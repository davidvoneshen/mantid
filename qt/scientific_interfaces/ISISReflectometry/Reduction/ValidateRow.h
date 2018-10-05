// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//     NScD Oak Ridge National Laboratory, European Spallation Source
//     & Institut Laue - Langevin
// SPDX - License - Identifier: GPL - 3.0 +

#ifndef MANTID_CUSTOMINTERFACES_VALIDATEROW_H_
#define MANTID_CUSTOMINTERFACES_VALIDATEROW_H_
#include "DllConfig.h"
#include "ParseReflectometryStrings.h"
#include "Reduction/ReductionJobs.h"
#include "Row.h"
#include "TransmissionRunPair.h"
#include "ValidationResult.h"
#include <boost/optional.hpp>
namespace MantidQt {
namespace CustomInterfaces {

class MANTIDQT_ISISREFLECTOMETRY_DLL RowValidator {
public:
  ValidationResult<Row, std::vector<int>>
  operator()(std::vector<std::string> const &cellText);

private:
  boost::optional<std::vector<std::string>>
  parseRunNumbers(std::vector<std::string> const &cellText);
  boost::optional<double> parseTheta(std::vector<std::string> const &cellText);
  boost::optional<TransmissionRunPair>
  parseTransmissionRuns(std::vector<std::string> const &cellText);
  boost::optional<RangeInQ>
  parseQRange(std::vector<std::string> const &cellText);
  boost::optional<boost::optional<double>>
  parseScaleFactor(std::vector<std::string> const &cellText);
  boost::optional<std::map<std::string, std::string>>
  parseOptions(std::vector<std::string> const &cellText);

  std::vector<int> m_invalidColumns;
};

using RowValidationResult = ValidationResult<Row, std::vector<int>>;

RowValidationResult validateRow(Jobs const &jobs,
                                std::vector<std::string> const &cellText);

boost::optional<Row> validateRowFromRunAndTheta(Jobs const &jobs,
                                                std::string const &run,
                                                std::string const &theta);
} // namespace CustomInterfaces
} // namespace MantidQt
#endif // MANTID_CUSTOMINTERFACES_VALIDATEROW_H_
