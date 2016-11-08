#include "MantidQtCustomInterfaces/Tomography/TomoToolConfigDialogTomoPy.h"
#include "MantidQtCustomInterfaces/Tomography/ToolConfigTomoPy.h"

namespace MantidQt {
namespace CustomInterfaces {

const std::string TomoToolConfigDialogTomoPy::DEFAULT_TOOL_NAME = "TomoPy";
const std::string TomoToolConfigDialogTomoPy::DEFAULT_TOOL_METHOD = "gridrec";

void TomoToolConfigDialogTomoPy::setupDialogUi() {
  m_tomoPyUi.setupUi(m_dialog);
  m_tomoPyUi.comboBox_method->clear();

  const auto methods = ToolConfigTomoPy::methods();
  const size_t methodsSize = methods.size();
  for (size_t i = 0; i < methodsSize; i++) {
    m_tomoPyUi.comboBox_method->addItem(
        QString::fromStdString(methods[i].second));
  }
}

void TomoToolConfigDialogTomoPy::initialiseDialog() { m_dialog = new QDialog; }

void TomoToolConfigDialogTomoPy::setupToolSettingsFromPaths() {
  // TODO: for the output path, probably better to take the sample path,
  // then up one level
  m_toolSettings = std::make_shared<ToolConfigTomoPy>(
      m_runPath, m_pathOut + m_localOutNameAppendix, m_paths.pathDarks(),
      m_paths.pathOpenBeam(), m_paths.pathSamples());
}
void TomoToolConfigDialogTomoPy::setupMethodSelected() {
  // move to member/global variable and use more space OR keep here
  const auto methods = ToolConfigTomoPy::methods();

  const int mi = m_tomoPyUi.comboBox_method->currentIndex();
  // TODO maybe comboBox_method->currentText?
  m_toolMethod = methods[mi].first;
}

/** Calls the execute of the QDialog
*/
int TomoToolConfigDialogTomoPy::executeQt() { return m_dialog->exec(); }

} // Custominterfaces
} // MantidQt
