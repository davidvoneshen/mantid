// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//     NScD Oak Ridge National Laboratory, European Spallation Source
//     & Institut Laue - Langevin
// SPDX - License - Identifier: GPL - 3.0 +
#ifndef MANTID_ISISREFLECTOMETRY_INDEXOF_H
#define MANTID_ISISREFLECTOMETRY_INDEXOF_H
#include <algorithm>
#include <boost/optional.hpp>
#include <iterator>

namespace MantidQt {
namespace CustomInterfaces {

template <typename Container, typename Predicate>
boost::optional<int> indexOf(Container const &container, Predicate pred) {
  auto maybeItemIt = std::find_if(container.cbegin(), container.cend(), pred);
  if (maybeItemIt != container.cend())
    return static_cast<int>(std::distance(container.cbegin(), maybeItemIt));
  else
    return boost::none;
}
} // namespace CustomInterfaces
} // namespace MantidQt
#endif // MANTID_ISISREFLECTOMETRY_INDEXOF_H
