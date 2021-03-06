// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

#include <cxxtest/TestSuite.h>

#include "MantidAPI/AlgorithmManager.h"
#include "MantidKernel/ConfigService.h"
#include "MantidKernel/FacilityInfo.h"
#include "MantidRemoteAlgorithms/UploadRemoteFile.h"

using namespace Mantid::RemoteAlgorithms;

class UploadRemoteFileTest : public CxxTest::TestSuite {
public:
  // This pair of boilerplate methods prevent the suite being created statically
  // This means the constructor isn't called when running other tests
  static UploadRemoteFileTest *createSuite() {
    return new UploadRemoteFileTest();
  }
  static void destroySuite(UploadRemoteFileTest *suite) { delete suite; }

  void test_algorithm() {
    testAlg =
        Mantid::API::AlgorithmManager::Instance().create("UploadRemoteFile", 1);
    TS_ASSERT(testAlg);
    TS_ASSERT_EQUALS(testAlg->name(), "UploadRemoteFile");
    TS_ASSERT_EQUALS(testAlg->version(), 1);
  }

  void test_castAlgorithm() {
    // can create
    std::shared_ptr<UploadRemoteFile> a;
    TS_ASSERT(a = std::make_shared<UploadRemoteFile>());

    // can cast to inherited interfaces and base classes
    TS_ASSERT(
        dynamic_cast<Mantid::RemoteAlgorithms::UploadRemoteFile *>(a.get()));
    TS_ASSERT(dynamic_cast<Mantid::API::Algorithm *>(a.get()));
    TS_ASSERT(dynamic_cast<Mantid::Kernel::PropertyManagerOwner *>(a.get()));
    TS_ASSERT(dynamic_cast<Mantid::API::IAlgorithm *>(a.get()));
    TS_ASSERT(dynamic_cast<Mantid::Kernel::IPropertyManager *>(a.get()));
  }

  void test_init() {
    if (!testAlg->isInitialized())
      TS_ASSERT_THROWS_NOTHING(testAlg->initialize());

    TS_ASSERT(testAlg->isInitialized());

    UploadRemoteFile ul;
    TS_ASSERT_THROWS_NOTHING(ul.initialize());
  }

  // TODO: when we have a RemoteJobManager capable of creating
  // algorithms for different types of compute resources (example:
  // Fermi@SNS), create different algorithms for them
  void test_propertiesMissing() {
    UploadRemoteFile alg1;
    TS_ASSERT_THROWS_NOTHING(alg1.initialize());
    // Transaction id missing
    TS_ASSERT_THROWS_NOTHING(
        alg1.setPropertyValue("RemoteFileName", "file name"));
    TS_ASSERT_THROWS_NOTHING(
        alg1.setPropertyValue("LocalFileName", "local file name"));
    TS_ASSERT_THROWS(alg1.setPropertyValue("ComputeResource", "missing!"),
                     const std::invalid_argument &);

    TS_ASSERT_THROWS(alg1.execute(), const std::runtime_error &);
    TS_ASSERT(!alg1.isExecuted());

    UploadRemoteFile alg2;
    TS_ASSERT_THROWS_NOTHING(alg2.initialize());
    // remote file name missing
    TS_ASSERT_THROWS_NOTHING(alg2.setPropertyValue("TransactionID", "id001"));
    TS_ASSERT_THROWS_NOTHING(
        alg2.setPropertyValue("LocalFileName", "local file name"));
    TS_ASSERT_THROWS(alg2.setPropertyValue("ComputeResource", "missing!"),
                     const std::invalid_argument &);

    TS_ASSERT_THROWS(alg2.execute(), const std::runtime_error &);
    TS_ASSERT(!alg2.isExecuted());

    UploadRemoteFile alg3;
    TS_ASSERT_THROWS_NOTHING(alg3.initialize());
    // local file name missing
    TS_ASSERT_THROWS_NOTHING(alg3.setPropertyValue("TransactionID", "id001"));
    TS_ASSERT_THROWS_NOTHING(
        alg3.setPropertyValue("RemoteFileName", "remote file name"));
    TS_ASSERT_THROWS(alg3.setPropertyValue("ComputeResource", "missing!"),
                     const std::invalid_argument &);

    TS_ASSERT_THROWS(alg3.execute(), const std::runtime_error &);
    TS_ASSERT(!alg3.isExecuted());

    UploadRemoteFile alg4;
    TS_ASSERT_THROWS_NOTHING(alg4.initialize());
    // compute resource missing
    TS_ASSERT_THROWS_NOTHING(
        alg4.setPropertyValue("RemoteFileName", "file name"));
    TS_ASSERT_THROWS_NOTHING(alg4.setPropertyValue("TransactionID", "id001"));

    TS_ASSERT_THROWS(alg4.execute(), const std::runtime_error &);
    TS_ASSERT(!alg4.isExecuted());
  }

  void test_wrongProperty() {
    UploadRemoteFile ul;
    TS_ASSERT_THROWS_NOTHING(ul.initialize();)
    TS_ASSERT_THROWS(ul.setPropertyValue("Compute", "anything"),
                     const std::runtime_error &);
    TS_ASSERT_THROWS(ul.setPropertyValue("TransID", "anything"),
                     const std::runtime_error &);
    TS_ASSERT_THROWS(ul.setPropertyValue("RemoteFile", "anything"),
                     const std::runtime_error &);
    TS_ASSERT_THROWS(ul.setPropertyValue("FileName", "anything"),
                     const std::runtime_error &);
    TS_ASSERT_THROWS(ul.setPropertyValue("LocalFile", "anything"),
                     const std::runtime_error &);
  }

  void test_propertiesOK() {
    testFacilities.emplace_back("SNS", "Fermi");

    const Mantid::Kernel::FacilityInfo &prevFac =
        Mantid::Kernel::ConfigService::Instance().getFacility();
    for (auto &testFacility : testFacilities) {
      const std::string facName = testFacility.first;
      const std::string compName = testFacility.second;

      Mantid::Kernel::ConfigService::Instance().setFacility(facName);

      UploadRemoteFile ul;
      TS_ASSERT_THROWS_NOTHING(ul.initialize());
      TS_ASSERT_THROWS_NOTHING(
          ul.setPropertyValue("ComputeResource", compName));
      TS_ASSERT_THROWS_NOTHING(
          ul.setPropertyValue("TransactionID", "anything001"));
      TS_ASSERT_THROWS_NOTHING(
          ul.setPropertyValue("RemoteFileName", "any name"));
      TS_ASSERT_THROWS_NOTHING(
          ul.setPropertyValue("LocalFileName", "any local path"));
      // TODO: this would run the algorithm and do a remote
      // connection. uncomment only when/if we have a mock up for this
      // TS_ASSERT_THROWS(ul.execute(), std::exception);
      TS_ASSERT(!ul.isExecuted());
    }
    Mantid::Kernel::ConfigService::Instance().setFacility(prevFac.name());
  }

  // TODO: void test_runOK() - with a mock when we can add it.
  // ideally, with different compute resources to check the remote job
  // manager factory, etc.

private:
  Mantid::API::IAlgorithm_sptr testAlg;
  std::vector<std::pair<std::string, std::string>> testFacilities;
};
