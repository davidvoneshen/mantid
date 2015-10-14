import unittest
from mantid.simpleapi import DeleteWorkspace, CreateSampleWorkspace, AddSampleLog, EditInstrumentGeometry,\
    CloneWorkspace, CheckWorkspacesMatch
from testhelpers import run_algorithm
from mantid.api import AnalysisDataService
from scipy.constants import N_A, hbar, k
import numpy as np


class ComputeCalibrationCoefVanTest(unittest.TestCase):
    def setUp(self):
        input_ws = CreateSampleWorkspace(Function="User Defined",
                                         UserDefinedFunction="name=LinearBackground, A0=0.3;name=Gaussian, \
                                         PeakCentre=5, Height=10, Sigma=0.3", NumBanks=2, BankPixelWidth=1,
                                         XMin=0, XMax=10, BinWidth=0.1, BankDistanceFromSample=4.0)
        self._input_ws = input_ws
        AddSampleLog(self._input_ws, LogName='wavelength', LogText='4.0', LogType='Number', LogUnit='Angstrom')

    def test_output(self):
        outputWorkspaceName = "output_ws"
        alg_test = run_algorithm("ComputeCalibrationCoefVan", VanadiumWorkspace=self._input_ws,
                                 OutputWorkspace=outputWorkspaceName)
        self.assertTrue(alg_test.isExecuted())
        wsoutput = AnalysisDataService.retrieve(outputWorkspaceName)

        # Output = Vanadium ws
        self.assertEqual(wsoutput.getRun().getLogData('run_title').value,
                         self._input_ws.getRun().getLogData('run_title').value)

        # Structure of output workspace
        for i in range(wsoutput.getNumberHistograms()):
            self.assertIsNotNone(wsoutput.readY(i)[0])
            for j in range(1, wsoutput.blocksize()):
                self.assertEqual(wsoutput.readY(i)[j], wsoutput.readY(i)[0])

        # Size of output workspace
        self.assertEqual(wsoutput.getNumberHistograms(), self._input_ws.getNumberHistograms())

        DeleteWorkspace(wsoutput)
        return

    def test_sum(self):
        outputWorkspaceName = "output_ws"
        alg_test = run_algorithm("ComputeCalibrationCoefVan", VanadiumWorkspace=self._input_ws,
                                 OutputWorkspace=outputWorkspaceName)
        self.assertTrue(alg_test.isExecuted())
        wsoutput = AnalysisDataService.retrieve(outputWorkspaceName)

        # check whether sum is calculated correctly, for theta=0, dwf=1
        y_sum = sum(self._input_ws.readY(0)[27:75])
        self.assertAlmostEqual(y_sum, wsoutput.readY(0)[0])

        DeleteWorkspace(wsoutput)

    def test_dwf(self):
        outputWorkspaceName = "output_ws"

        # change theta to make dwf != 1
        EditInstrumentGeometry(self._input_ws, L2="4,8", Polar="0,15", Azimuthal="0,0", DetectorIDs="1,2")
        alg_test = run_algorithm("ComputeCalibrationCoefVan", VanadiumWorkspace=self._input_ws,
                                 OutputWorkspace=outputWorkspaceName)
        self.assertTrue(alg_test.isExecuted())
        wsoutput = AnalysisDataService.retrieve(outputWorkspaceName)

        # check dwf calculation
        y_sum = sum(self._input_ws.readY(1)[27:75])
        mvan = 0.001*50.942/N_A
        Bcoef = 4.736767162094296*1e+20*hbar*hbar/(2.0*mvan*k*389.0)
        dwf = np.exp(-1.0*Bcoef*(4.0*np.pi*np.sin(0.5*np.radians(15.0))/4.0)**2)
        self.assertAlmostEqual(y_sum*dwf, wsoutput.readY(1)[0])

        DeleteWorkspace(wsoutput)

    def test_input_not_modified(self):
        backup = CloneWorkspace(self._input_ws)
        outputWorkspaceName = "output_ws"
        alg_test = run_algorithm("ComputeCalibrationCoefVan", VanadiumWorkspace=self._input_ws,
                                 OutputWorkspace=outputWorkspaceName)
        self.assertTrue(alg_test.isExecuted())
        self.assertEqual("Success!", CheckWorkspacesMatch(backup, self._input_ws))
        DeleteWorkspace(backup)

    def tearDown(self):
        if AnalysisDataService.doesExist(self._input_ws.getName()):
            DeleteWorkspace(self._input_ws)

if __name__ == "__main__":
    unittest.main()
