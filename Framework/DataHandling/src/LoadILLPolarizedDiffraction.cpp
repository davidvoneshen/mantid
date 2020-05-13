// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#include "MantidDataHandling/LoadILLPolarizedDiffraction.h"
#include "MantidAPI/Axis.h"
#include "MantidAPI/FileProperty.h"
#include "MantidAPI/MatrixWorkspace.h"
#include "MantidAPI/RegisterFileLoader.h"
#include "MantidAPI/WorkspaceFactory.h"
#include "MantidAPI/WorkspaceGroup.h"
#include "MantidGeometry/Instrument/ComponentHelper.h"
#include "MantidGeometry/Instrument/ComponentInfo.h"
#include "MantidGeometry/Instrument/DetectorInfo.h"
#include "MantidKernel/DateAndTime.h"
#include "MantidKernel/OptionalBool.h"
#include "MantidKernel/Unit.h"
#include "MantidKernel/UnitFactory.h"
#include "MantidKernel/UnitLabelTypes.h"
#include "MantidKernel/V3D.h"

#include <Poco/Path.h>

namespace Mantid {
namespace DataHandling {

using namespace API;
using namespace Geometry;
using namespace Kernel;
using namespace NeXus;
using Types::Core::DateAndTime;

namespace {
// This defines the number of detector banks in D7
constexpr size_t D7_NUMBER_BANKS = 3;
// This defines the number of physical pixels in D7
constexpr size_t D7_NUMBER_PIXELS = 132;
// This defines the number of monitors in the instrument. If there are cases
// where this is no longer one this decleration should be moved.
constexpr size_t NUMBER_MONITORS = 2;
// The conversion factor from radian to degree
constexpr double DEG_TO_RAD = M_PI / 180.;
} // namespace

// Register the algorithm into the AlgorithmFactory
DECLARE_NEXUS_FILELOADER_ALGORITHM(LoadILLPolarizedDiffraction)

/// Returns confidence. @see IFileLoader::confidence
int LoadILLPolarizedDiffraction::confidence(NexusDescriptor &descriptor) const {

  // fields existent only at the ILL Diffraction
  if (descriptor.pathExists("/entry0/D7/2theta")) {
    return 80;
  } else {
    return 0;
  }
}

/// Algorithms name for identification. @see Algorithm::name
const std::string LoadILLPolarizedDiffraction::name() const {
  return "LoadILLPolarizedDiffraction";
}

/// Algorithm's version for identification. @see Algorithm::version
int LoadILLPolarizedDiffraction::version() const { return 1; }

/// Algorithm's category for identification. @see Algorithm::category
const std::string LoadILLPolarizedDiffraction::category() const {
  return "DataHandling\\Nexus;ILL\\Diffraction";
}

/// Algorithm's summary for use in the GUI and help. @see Algorithm::summary
const std::string LoadILLPolarizedDiffraction::summary() const {
  return "Loads ILL polarized diffraction nexus files.";
}

/**
 * Constructor
 */
LoadILLPolarizedDiffraction::LoadILLPolarizedDiffraction()
    : IFileLoader<NexusDescriptor>(), m_instNames({"D7"}) {}

/**
 * Initialize the algorithm's properties.
 */
void LoadILLPolarizedDiffraction::init() {
  declareProperty(std::make_unique<FileProperty>("Filename", "",
                                                 FileProperty::Load, ".nxs"),
                  "File path of the data file to load");
  declareProperty(std::make_unique<WorkspaceProperty<API::Workspace>>(
                      "OutputWorkspace", "", Direction::Output),
                  "The output workspace.");
}

/**
 * Executes the algorithm.
 */
void LoadILLPolarizedDiffraction::exec() {

  Progress progress(this, 0, 1, 1);

  m_filename = getPropertyValue("Filename");

  m_outputWorkspace = std::make_shared<API::WorkspaceGroup>();
  setProperty("OutputWorkspace", m_outputWorkspace);

  progress.report("Loading the detector polarization analysis data");
  loadData();
}

/**
 * Loads the polarized detector data, sets up workspaces and labels
 *  according to the measurement type and data dimensions
 */
void LoadILLPolarizedDiffraction::loadData() {

  // open the root entry
  NXRoot dataRoot(m_filename);

  // read each entry
  for (auto entryNumber = 0;
       entryNumber < static_cast<int>((dataRoot.groups().size()));
       entryNumber++) {
    NXEntry entry = dataRoot.openEntry("entry" + std::to_string(entryNumber));
    m_instName = entry.getString("D7/name");

    std::string start_time = entry.getString("start_time");
    start_time = m_loadHelper.dateTimeInIsoFormat(start_time);

    // check the mode of measurement and prepare axes for data
    std::vector<double> axis;
    NXInt acquisitionMode = entry.openNXInt("acquisition_mode");
    acquisitionMode.load();
    if (acquisitionMode[0] == 1) {
      NXFloat timeOfFlightInfo =
          entry.openNXFloat("D7/Detector/time_of_flight");
      timeOfFlightInfo.load();
      auto channelWidth = static_cast<double>(timeOfFlightInfo[0]);
      m_numberOfChannels = size_t(timeOfFlightInfo[1]);
      auto tofDelay = timeOfFlightInfo[2];
      for (auto ii = 0; ii <= static_cast<int>(m_numberOfChannels); ii++) {
        axis.push_back(static_cast<double>(tofDelay + ii * channelWidth));
      }
    } else {
      m_numberOfChannels = 1;
      NXFloat wavelength = entry.openNXFloat("D7/monochromator/wavelength");
      wavelength.load();
      axis.push_back(static_cast<double>(wavelength[0] * 0.9));
      axis.push_back(static_cast<double>(wavelength[0] * 1.1));
    }

    // init the workspace with proper number of histograms and number of
    // channels
    auto workspace = initStaticWorkspace();

    // check the polarization direction and set the workspace title
    std::string polDirection = entry.getString("D7/POL/actual_state");
    std::string flipperState = entry.getString("D7/POL/actual_stateB1B2");
    workspace->setTitle(polDirection.substr(0, 1) + "_" + flipperState);

    // Set x axis units
    if (acquisitionMode[0] == 1) {
      std::shared_ptr<Kernel::Units::Label> lblUnit =
          std::dynamic_pointer_cast<Kernel::Units::Label>(
              UnitFactory::Instance().create("Label"));
      lblUnit->setLabel("Time", Units::Symbol::Microsecond);
      workspace->getAxis(0)->unit() = lblUnit;
    } else {
      std::shared_ptr<Kernel::Units::Label> lblUnit =
          std::dynamic_pointer_cast<Kernel::Units::Label>(
              UnitFactory::Instance().create("Label"));
      lblUnit->setLabel("Wavelength", Units::Symbol::Angstrom);
      workspace->getAxis(0)->unit() = lblUnit;
    }

    // Set y axis unit
    workspace->setYUnit("Counts");

    // load data from file
    std::string dataName = "data/Detector_data";
    NXUInt data = entry.openNXDataSet<unsigned int>(dataName);
    data.load();

    // Assign detector counts
    for (auto ii = 0; ii < static_cast<int>(D7_NUMBER_PIXELS); ++ii) {
      auto &spectrum = workspace->mutableY(ii);
      auto &errors = workspace->mutableE(ii);
      const auto pixelNumber = ii;
      for (auto jj = 0; jj < static_cast<int>(m_numberOfChannels); ++jj) {
        unsigned int y =
            data(static_cast<int>(pixelNumber), 0, static_cast<int>(jj));
        spectrum[jj] = y;
        errors[jj] = sqrt(y);
      }
      workspace->mutableX(ii) = axis;
    }

    // load and assign monitor data
    for (auto ii = static_cast<int>(D7_NUMBER_PIXELS);
         ii < static_cast<int>(D7_NUMBER_PIXELS + NUMBER_MONITORS); ii++) {
      NXUInt monitorData = entry.openNXDataSet<unsigned int>(
          "monitor" +
          std::to_string(ii + 1 - static_cast<int>(D7_NUMBER_PIXELS)) +
          "/data");
      monitorData.load();
      auto &spectrum = workspace->mutableY(ii);
      auto &errors = workspace->mutableE(ii);
      for (auto jj = 0; jj < static_cast<int>(m_numberOfChannels); jj++) {
        unsigned int y = monitorData(0, 0, static_cast<int>(jj));
        spectrum[jj] = y;
        errors[jj] = sqrt(y);
      }
      workspace->mutableX(ii) = axis;
    }

    // load the instrument if it has not been created
    if (m_outputWorkspace->getNumberOfEntries() == 0) {
      loadInstrument(workspace);
      // rotate detectors to their position during measurement
      moveTwoThetaZero(entry, workspace);
      // sets meta data for the measurement
      loadMetaData(workspace);
    }

    // adds the current entry workspace to the output group
    m_outputWorkspace->addWorkspace(workspace);

    entry.close();
  }
  dataRoot.close();
}

/**
 * Dumps the metadata from the whole file to SampleLogs
 */
void LoadILLPolarizedDiffraction::loadMetaData(
    API::MatrixWorkspace_sptr &workspace) {

  auto &mutableRun = workspace->mutableRun();
  mutableRun.addProperty("Facility", std::string("ILL"));

  // Open NeXus file
  NXhandle nxHandle;
  NXstatus nxStat = NXopen(m_filename.c_str(), NXACC_READ, &nxHandle);

  if (nxStat != NX_ERROR) {
    m_loadHelper.addNexusFieldsToWsRun(nxHandle, workspace->mutableRun());
    NXclose(&nxHandle);
  }
}

/**
 * Initializes the output workspace based on the resolved instrument.
 * If there are multiple entries in the file and the current entry
 * is not the first one, the returned workspace is a clone
 * of the workspace from the first entry
 * @return : workspace with the correct data dimensions
 */
API::MatrixWorkspace_sptr LoadILLPolarizedDiffraction::initStaticWorkspace() {
  size_t nSpectra = D7_NUMBER_PIXELS + NUMBER_MONITORS;

  API::MatrixWorkspace_sptr workspace;

  if (m_outputWorkspace->getNumberOfEntries() == 0) {
    workspace = WorkspaceFactory::Instance().create(
        "Workspace2D", nSpectra, m_numberOfChannels + 1, m_numberOfChannels);
  } else {
    API::Workspace_sptr tmp = (m_outputWorkspace->getItem(0))->clone();
    workspace = std::dynamic_pointer_cast<API::MatrixWorkspace>(tmp);
  }
  return workspace;
}
/**
 * Runs LoadInstrument as child to link the instrument to workspace
 */
void LoadILLPolarizedDiffraction::loadInstrument(
    API::MatrixWorkspace_sptr &workspace) {
  IAlgorithm_sptr loadInst = createChildAlgorithm("LoadInstrument");
  loadInst->setPropertyValue("Filename", getInstrumentFilePath(m_instName));
  loadInst->setProperty<MatrixWorkspace_sptr>("Workspace", workspace);
  loadInst->setProperty("RewriteSpectraMap", OptionalBool(true));
  loadInst->execute();
}

/**
 * Loads twotheta for each detector pixel from the file
 * @param entry : entry from which the pixel 2theta positions will be read
 * @param bankId : bank ID for which 2theta positions will be read
 * @return : vector of pixel 2theta positions in the chosen bank
 */
std::vector<double>
LoadILLPolarizedDiffraction::loadTwoTheta0(const NXEntry &entry, int bankId) {
  NXFloat theta0Pixels = entry.openNXFloat("D7/Detector/bank" +
                                           std::to_string(bankId) + "_offset");
  theta0Pixels.load();
  NXFloat theta0Bank =
      entry.openNXFloat("D7/2theta/actual_bank" + std::to_string(bankId));
  theta0Bank.load();

  std::vector<double> theta0(theta0Pixels.size());
  for (auto ii = 0; ii < theta0Pixels.size(); ii++) {
    theta0[ii] = theta0Pixels[ii] - theta0Bank[0];
  }
  return theta0;
}

/**
 * Rotates each pixel to its corresponding 2theta read from the file
 * @param entry : entry from which the 2theta positions will be read
 * @param workspace : workspace containing the instrument being moved
 */
void LoadILLPolarizedDiffraction::moveTwoThetaZero(
    const NXEntry &entry, API::MatrixWorkspace_sptr &workspace) {

  Instrument_const_sptr instrument = workspace->getInstrument();
  auto const numberDetectorsBank =
      static_cast<int>(D7_NUMBER_PIXELS / D7_NUMBER_BANKS);

  auto &componentInfo = workspace->mutableComponentInfo();
  for (auto ii = 0; ii < static_cast<int>(D7_NUMBER_BANKS); ii++) {
    std::vector<double> theta0 = loadTwoTheta0(entry, ii + 2);
    for (auto jj = 0; jj < numberDetectorsBank; jj++) {
      IComponent_const_sptr pixel =
          instrument->getDetector(ii * numberDetectorsBank + jj + 1);
      const auto componentIndex =
          componentInfo.indexOf(pixel->getComponentID());
      V3D position = pixel->getPos();
      double const radius =
          sqrt(position[0] * position[0] + position[2] * position[2]);
      position = V3D(radius * sin(DEG_TO_RAD * theta0[jj]), position[1],
                     radius * cos(DEG_TO_RAD * theta0[jj]));
      componentInfo.setPosition(componentIndex, position);
    }
  }
}

/**
 * Makes up the full path of the relevant IDF
 * @param instName : the name of the instrument
 * @return : the full path to the corresponding IDF
 */
std::string LoadILLPolarizedDiffraction::getInstrumentFilePath(
    const std::string &instName) const {

  Poco::Path directory(ConfigService::Instance().getInstrumentDirectory());
  Poco::Path file(instName + "_Definition.xml");
  Poco::Path fullPath(directory, file);
  return fullPath.toString();
}

} // namespace DataHandling
} // namespace Mantid
