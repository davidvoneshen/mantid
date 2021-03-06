// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2015 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

#include "MantidCurveFitting/IFittingAlgorithm.h"

namespace Mantid {
namespace CurveFitting {
namespace Algorithms {

/**

  Evaluate a function (1D or MD) on a domain of an input workspace and save
  the result in the output workspace.
*/
class MANTID_CURVEFITTING_DLL EvaluateFunction : public IFittingAlgorithm {
public:
  const std::string name() const override;
  int version() const override;
  const std::vector<std::string> seeAlso() const override { return {"Fit"}; }
  const std::string summary() const override;

private:
  void initConcrete() override;
  void execConcrete() override;
};

} // namespace Algorithms
} // namespace CurveFitting
} // namespace Mantid
