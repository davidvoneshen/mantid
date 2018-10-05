// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//     NScD Oak Ridge National Laboratory, European Spallation Source
//     & Institut Laue - Langevin
// SPDX - License - Identifier: GPL - 3.0 +

#ifndef MANTID_ISISREFLECTOMETRY_ROWLOCATION_H
#define MANTID_ISISREFLECTOMETRY_ROWLOCATION_H
#include "MantidQtWidgets/Common/Batch/RowLocation.h"

namespace MantidQt {
namespace CustomInterfaces {

std::vector<int> groupIndexesFromSelection(
    std::vector<MantidWidgets::Batch::RowLocation> const &selected);
std::vector<int> mapToContainingGroups(
    std::vector<MantidQt::MantidWidgets::Batch::RowLocation> const
        &mustNotContainRoot);
bool containsGroups(
    std::vector<MantidQt::MantidWidgets::Batch::RowLocation> const &locations);
bool isGroupLocation(
    MantidQt::MantidWidgets::Batch::RowLocation const &location);
int groupOf(MantidQt::MantidWidgets::Batch::RowLocation const &groupLocation);
bool isRowLocation(MantidWidgets::Batch::RowLocation const &location);
int rowOf(MantidQt::MantidWidgets::Batch::RowLocation const &rowLocation);
} // namespace CustomInterfaces
} // namespace MantidQt
#endif // MANTID_ISISREFLECTOMETRY_ROWLOCATION_H
