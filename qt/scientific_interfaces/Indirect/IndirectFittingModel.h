// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2007 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

#include "IIndirectFitData.h"
#include "IIndirectFitOutput.h"
#include "IIndirectFitResult.h"
#include "IndexTypes.h"
#include "IndirectFitData.h"
#include "IndirectFitOutput.h"
#include "ParameterEstimation.h"

#include "DllConfig.h"
#include "MantidAPI/IAlgorithm.h"
#include "MantidAPI/IFunction_fwd.h"

#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace MantidQt {
namespace CustomInterfaces {
namespace IDA {

enum class FittingMode { SEQUENTIAL, SIMULTANEOUS };
extern std::unordered_map<FittingMode, std::string> fitModeToName;

class IndirectFittingModel;

using DefaultParametersType =
    IndexCollectionType<TableDatasetIndex,
                        std::unordered_map<std::string, ParameterValue>>;

/*
    IndirectFittingModel - Provides methods for specifying and
    performing a QENS fit, as well as accessing the results of the fit.
*/
class MANTIDQT_INDIRECT_DLL IndirectFittingModel : public IIndirectFitResult {
public:
  IndirectFittingModel();
  virtual ~IndirectFittingModel() = default;

  // IIndirectFitData
  bool hasWorkspace(std::string const &workspaceName) const;
  Mantid::API::MatrixWorkspace_sptr getWorkspace(TableDatasetIndex index) const;
  Spectra getSpectra(TableDatasetIndex index) const;
  virtual bool isMultiFit() const;
  TableDatasetIndex numberOfWorkspaces() const;
  int getNumberOfSpectra(TableDatasetIndex index) const;
  int getNumberOfDomains() const;
  FitDomainIndex getDomainIndex(TableDatasetIndex dataIndex,
                                WorkspaceIndex spectrum) const;
  std::vector<double> getQValuesForData() const;
  virtual std::vector<std::pair<std::string, int>> getResolutionsForFit() const;
  // void setFittingData(PrivateFittingData &&fittingData);
  void clearWorkspaces();
  void clear();

  void setSpectra(const std::string &spectra, TableDatasetIndex dataIndex);
  void setSpectra(Spectra &&spectra, TableDatasetIndex dataIndex);
  void setSpectra(const Spectra &spectra, TableDatasetIndex dataIndex);
  void addWorkspace(const std::string &workspaceName);
  void addWorkspace(const std::string &workspaceName,
                    const std::string &spectra);
  void addWorkspace(const std::string &workspaceName, const Spectra &spectra);
  virtual void removeWorkspace(TableDatasetIndex index);

  // IIndirectFitRegion
  virtual std::pair<double, double>
  getFittingRange(TableDatasetIndex dataIndex, WorkspaceIndex spectrum) const;
  virtual std::string getExcludeRegion(TableDatasetIndex dataIndex,
                                       WorkspaceIndex index) const;

  void setStartX(double startX, TableDatasetIndex dataIndex,
                 WorkspaceIndex spectrum);
  void setStartX(double startX, TableDatasetIndex dataIndex);
  void setEndX(double endX, TableDatasetIndex dataIndex,
               WorkspaceIndex spectrum);
  void setEndX(double endX, TableDatasetIndex dataIndex);
  void setExcludeRegion(const std::string &exclude, TableDatasetIndex dataIndex,
                        WorkspaceIndex spectrum);

  // Functions concerned with naming
  virtual std::string createDisplayName(const std::string &formatString,
                                        const std::string &rangeDelimiter,
                                        TableDatasetIndex dataIndex) const;
  std::string createOutputName(const std::string &formatString,
                               const std::string &rangeDelimiter,
                               TableDatasetIndex dataIndex) const;

  // IIndirectFitResult
  bool isPreviouslyFit(TableDatasetIndex dataIndex,
                       WorkspaceIndex spectrum) const override;
  virtual boost::optional<std::string> isInvalidFunction() const override;
  std::vector<std::string> getFitParameterNames() const override;
  virtual Mantid::API::MultiDomainFunction_sptr
  getFittingFunction() const override;
  void setFitFunction(Mantid::API::MultiDomainFunction_sptr function) override;
  void setDefaultParameterValue(const std::string &name, double value,
                                TableDatasetIndex dataIndex) override;
  std::unordered_map<std::string, ParameterValue>
  getParameterValues(TableDatasetIndex dataIndex,
                     WorkspaceIndex spectrum) const override;
  std::unordered_map<std::string, ParameterValue>
  getFitParameters(TableDatasetIndex dataIndex, WorkspaceIndex spectrum) const;
  std::unordered_map<std::string, ParameterValue>
  getDefaultParameters(TableDatasetIndex dataIndex) const;

  // IIndirectFitOutput
  void addSingleFitOutput(const Mantid::API::IAlgorithm_sptr &fitAlgorithm,
                          TableDatasetIndex index);
  virtual void addOutput(Mantid::API::IAlgorithm_sptr fitAlgorithm);

  // Generic
  void setFittingMode(FittingMode mode);
  FittingMode getFittingMode() const;

  // To remove if possible

  template <typename F>
  void applySpectra(TableDatasetIndex index, const F &functor) const;
  boost::optional<ResultLocationNew>
  getResultLocation(TableDatasetIndex dataIndex, WorkspaceIndex spectrum) const;
  Mantid::API::WorkspaceGroup_sptr getResultWorkspace() const;
  Mantid::API::WorkspaceGroup_sptr getResultGroup() const;
  virtual Mantid::API::IAlgorithm_sptr getFittingAlgorithm() const;
  Mantid::API::IAlgorithm_sptr getSingleFit(TableDatasetIndex dataIndex,
                                            WorkspaceIndex spectrum) const;
  Mantid::API::IFunction_sptr getSingleFunction(TableDatasetIndex dataIndex,
                                                WorkspaceIndex spectrum) const;
  std::string getOutputBasename() const;

  void cleanFailedRun(const Mantid::API::IAlgorithm_sptr &fittingAlgorithm);
  void
  cleanFailedSingleRun(const Mantid::API::IAlgorithm_sptr &fittingAlgorithm,
                       TableDatasetIndex index);
  DataForParameterEstimationCollection
  getDataForParameterEstimation(const EstimationDataSelector &selector) const;

protected:
  // virtual void addWorkspace(Mantid::API::MatrixWorkspace_sptr workspace,
  //                           const Spectra &spectra);
  Mantid::API::IAlgorithm_sptr getFittingAlgorithm(FittingMode mode) const;
  Mantid::API::IAlgorithm_sptr
  createSequentialFit(Mantid::API::IFunction_sptr function) const;
  Mantid::API::IAlgorithm_sptr createSimultaneousFit(
      const Mantid::API::MultiDomainFunction_sptr &function) const;
  Mantid::API::IAlgorithm_sptr createSimultaneousFitWithEqualRange(
      const Mantid::API::IFunction_sptr &function) const;
  virtual Mantid::API::MultiDomainFunction_sptr getMultiDomainFunction() const;
  virtual std::unordered_map<std::string, std::string>
  mapDefaultParameterNames() const;
  std::string createSingleFitOutputName(const std::string &formatString,
                                        TableDatasetIndex index,
                                        WorkspaceIndex spectrum) const;
  void addNewWorkspace(const Mantid::API::MatrixWorkspace_sptr &workspace,
                       const Spectra &spectra);
  void removeFittingData(TableDatasetIndex index);

private:
  std::vector<std::string> getWorkspaceNames() const;
  std::vector<double> getExcludeRegionVector(TableDatasetIndex dataIndex,
                                             WorkspaceIndex index) const;

  void removeWorkspaceFromFittingData(TableDatasetIndex const &index);

  Mantid::API::IAlgorithm_sptr
  createSequentialFit(const Mantid::API::IFunction_sptr &function,
                      const std::string &input) const;
  virtual Mantid::API::IAlgorithm_sptr sequentialFitAlgorithm() const;
  virtual Mantid::API::IAlgorithm_sptr simultaneousFitAlgorithm() const;
  virtual std::string sequentialFitOutputName() const = 0;
  virtual std::string simultaneousFitOutputName() const = 0;
  virtual std::string singleFitOutputName(TableDatasetIndex index,
                                          WorkspaceIndex spectrum) const = 0;
  virtual std::unordered_map<std::string, ParameterValue>
  createDefaultParameters(TableDatasetIndex index) const;

  virtual std::string getResultXAxisUnit() const;
  virtual std::string getResultLogName() const;

  bool isPreviousModelSelected() const;

  virtual IndirectFitOutput
  createFitOutput(Mantid::API::WorkspaceGroup_sptr resultGroup,
                  Mantid::API::ITableWorkspace_sptr parameterTable,
                  Mantid::API::WorkspaceGroup_sptr resultWorkspace,
                  const FitDataIterator &fitDataBegin,
                  const FitDataIterator &fitDataEnd) const;
  virtual IndirectFitOutput
  createFitOutput(Mantid::API::WorkspaceGroup_sptr resultGroup,
                  Mantid::API::ITableWorkspace_sptr parameterTable,
                  Mantid::API::WorkspaceGroup_sptr resultWorkspace,
                  IndirectFitData *fitData, WorkspaceIndex spectrum) const;

  void addOutput(const Mantid::API::IAlgorithm_sptr &fitAlgorithm,
                 const FitDataIterator &fitDataBegin,
                 const FitDataIterator &fitDataEnd);
  void addOutput(const Mantid::API::WorkspaceGroup_sptr &resultGroup,
                 const Mantid::API::ITableWorkspace_sptr &parameterTable,
                 const Mantid::API::WorkspaceGroup_sptr &resultWorkspace,
                 const FitDataIterator &fitDataBegin,
                 const FitDataIterator &fitDataEnd);
  void addOutput(const Mantid::API::WorkspaceGroup_sptr &resultGroup,
                 const Mantid::API::ITableWorkspace_sptr &parameterTable,
                 const Mantid::API::WorkspaceGroup_sptr &resultWorkspace,
                 IndirectFitData *fitData, WorkspaceIndex spectrum);

  virtual void addOutput(IndirectFitOutput *fitOutput,
                         Mantid::API::WorkspaceGroup_sptr resultGroup,
                         Mantid::API::ITableWorkspace_sptr parameterTable,
                         Mantid::API::WorkspaceGroup_sptr resultWorkspace,
                         const FitDataIterator &fitDataBegin,
                         const FitDataIterator &fitDataEnd) const;
  virtual void addOutput(IndirectFitOutput *fitOutput,
                         Mantid::API::WorkspaceGroup_sptr resultGroup,
                         Mantid::API::ITableWorkspace_sptr parameterTable,
                         Mantid::API::WorkspaceGroup_sptr resultWorkspace,
                         IndirectFitData *fitData,
                         WorkspaceIndex spectrum) const;

  std::unique_ptr<IIndirectFitOutput> m_fitOutput;
  // IndirectFitDataCollectionType m_fittingData;
  std::unique_ptr<IIndirectFitData> m_fitDataModel;
  Mantid::API::MultiDomainFunction_sptr m_activeFunction;
  // stores the single domain function
  Mantid::API::IFunction_sptr m_fitFunction;
  DefaultParametersType m_defaultParameters;
  // Stores whether the current fit function is the same as
  bool m_previousModelSelected;
  FittingMode m_fittingMode;
};

template <typename F>
void IndirectFittingModel::applySpectra(TableDatasetIndex index,
                                        const F &functor) const {
  // if (m_fittingData.size() > m_fittingData.zero())
  //   m_fittingData[index]->applySpectra(functor);
}

} // namespace IDA
} // namespace CustomInterfaces
} // namespace MantidQt
