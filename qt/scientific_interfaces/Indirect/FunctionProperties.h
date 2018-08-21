#include "MantidAPI/IFunction.h"

#include <boost/optional.hpp>

#include <unordered_map>

namespace Mantid {
namespace API {
class MatrixWorkspace;
} // namespace API
} // namespace Mantid

namespace MantidQt {
namespace MantidWidgets {

struct BoundaryConstraint {
  boost::optional<double> lowerBound;
  boost::optional<double> upperBound;
};

struct ParameterValue {
  explicit ParameterValue(double val) : value(val), error() {}
  ParameterValue(double val, double err) : value(val), error(err) {}
  double value;
  boost::optional<double> error;
};

class FunctionProperties {
public:
  FunctionProperties();

  bool isTied(std::string const &parameterName) const;
  bool isFixed(std::string const &parameterName) const;
  bool isConstrained(std::string const &parameterName) const;
  std::string const &getTie(std::string const &parameterName) const;
  std::string getTieOr(std::string const &parameterName,
                       std::string const &defaultValue) const;
  boost::optional<double>
  getParameterLowerBound(std::string const &parameterName) const;
  boost::optional<double>
  getParameterUpperBound(std::string const &parameterName) const;
  void tie(std::string const &parameterName, std::string const &expression);
  void removeTie(std::string const &parameterName);
  void clearTies();
  void setConstraint(std::string const &parameterName, double lowerBound,
                     double upperBound);
  void setLowerBound(std::string const &parameterName, double bound);
  void setUpperBound(std::string const &parameterName, double bound);
  void removeLowerBound(std::string const &parameterName);
  void removeUpperBound(std::string const &parameterName);
  void removeConstraints(std::string const &parameterName);
  void clearConstraints();

  template <typename F> void forEachTie(F const &functor) const;
  template <typename F> void forEachConstraint(F const &functor) const;
  template <typename Predicate> void removeTieIf(Predicate const &predicate);
  template <typename Predicate>
  void removeConstraintIf(Predicate const &predicate);

protected:
  void fixParameterTo(std::string const &parameterName, double value);
  template <typename Predicate>
  typename std::vector<std::pair<std::string, BoundaryConstraint>>::iterator
  findConstraintIf(Predicate const &predicate) const;
  typename std::vector<std::pair<std::string, BoundaryConstraint>>::iterator
  findConstraintOf(std::string const &parameterName) const;

  std::unordered_set<std::string> m_fixed;
  std::unordered_map<std::string, std::string> m_ties;
  std::vector<std::pair<std::string, BoundaryConstraint>> m_constraints;
};

class LocalFunctionProperties : public FunctionProperties {
public:
  explicit LocalFunctionProperties(
      boost::shared_ptr<Mantid::API::MatrixWorkspace const> ws,
      std::size_t wsIndex);

  template <typename F> void forEachParameter(F const &functor) const;
  template <typename F> void forEachAttribute(F const &functor) const;

  boost::shared_ptr<Mantid::API::MatrixWorkspace const> getWorkspace() const;
  std::size_t getWorkspaceIndex() const;
  Mantid::API::IFunction::Attribute const &
  getAttribute(std::string const &name) const;
  double getParameterValue(std::string const &parameterName) const;
  boost::optional<double>
  getParameterError(std::string const &parameterName) const;
  void setParameterValue(std::string const &parameterName, double value);
  void setParameter(std::string const &parameterName,
                    ParameterValue const &value);
  void setAttribute(std::string const &name,
                    Mantid::API::IFunction::Attribute const &attribute);
  void resizeVectorAttribute(std::string const &name, std::size_t size);
  void fixParameter(std::string const &parameterName);

private:
  ParameterValue const &getParameter(std::string const &parameterName) const;
  ParameterValue &getParameter(std::string const &parameterName);

  std::unordered_map<std::string, ParameterValue> m_parameters;
  std::unordered_map<std::string, Mantid::API::IFunction::Attribute>
      m_attributes;
  boost::shared_ptr<Mantid::API::MatrixWorkspace const> m_workspace;
  std::size_t m_workspaceIndex;
};

template <typename F>
void FunctionProperties::forEachTie(F const &functor) const {
  for (const auto &tie : m_ties)
    functor(tie.first, tie.second);
}

template <typename F>
void FunctionProperties::forEachConstraint(F const &functor) const {
  for (const auto &constraint : m_constraints)
    function(constraint.second);
}

template <typename Predicate>
void FunctionProperties::removeTieIf(Predicate const &predicate) {
  for (auto it = m_ties.begin(); it != m_ties.end();) {
    if (predicate(tie.first, tie.second))
      it = m_ties.erase(tie.first);
    else
      ++it;
  }
}

template <typename Predicate>
typename std::vector<std::pair<std::string, BoundaryConstraint>>::iterator
FunctionProperties::findConstraintIf(Predicate const &predicate) const {
  return std::find_if(m_constraints.begin(), m_constraints.end(), predicate);
}

template <typename Predicate>
void FunctionProperties::removeConstraintIf(Predicate const &predicate) {
  m_constraints.erase(
      std::remove_if(m_constraints.begin(), m_constraints.end(), predicate),
      m_constraints.end());
}

template <typename F>
void LocalFunctionProperties::forEachParameter(F const &functor) const {
  for (const auto &parameter : m_parameters)
    functor(parameter.first, parameter.second);
}

template <typename F>
void LocalFunctionProperties::forEachAttribute(F const &functor) const {
  for (const auto &attribute : m_attributes)
    functor(attribute.first, attribute.second);
}

} // namespace MantidWidgets
} // namespace MantidQt