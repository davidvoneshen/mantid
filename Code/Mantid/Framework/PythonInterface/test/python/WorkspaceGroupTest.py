import unittest
from testhelpers import run_algorithm
from mantid.api import mtd, WorkspaceGroup, MatrixWorkspace

class WorkspaceGroupTest(unittest.TestCase):
  
    def test_group_interface(self):
        run_algorithm('CreateWorkspace', OutputWorkspace='First',DataX=[1.,2.,3.], DataY=[2.,3.], DataE=[2.,3.],UnitX='TOF')
        run_algorithm('CreateWorkspace', OutputWorkspace='Second',DataX=[1.,2.,3.], DataY=[2.,3.], DataE=[2.,3.],UnitX='TOF')
        run_algorithm('GroupWorkspaces',InputWorkspaces='First,Second',OutputWorkspace='grouped')
        grouped = mtd['grouped']
        self.assertEquals(type(grouped), WorkspaceGroup)
        self.assertEquals(2, grouped.size())
        self.assertEquals(2, grouped.getNumberOfEntries())
        # Matches operator
        self.assertEquals(len(grouped), grouped.getNumberOfEntries())
        # Matches length of name list
        names = grouped.getNames()
        self.assertEquals(str(names), "['First','Second']")
        self.assertEquals(len(grouped), len(names))
        expected = ['First', 'Second']
        for i in range(len(names)):
            self.assertEquals(expected[i], names[i])
            
        # Clearing the data should leave the handle unusable
        mtd.clear()
        try:
            grouped.getNames()
            self.fail("WorkspaceGroup handle is still usable after ADS has been cleared, it should be a weak reference and raise an error.")
        except RuntimeError, exc:
            self.assertEquals(str(exc), 'Variable invalidated, data has been deleted.')
            
    def test_group_index_access_returns_correct_workspace(self):
        run_algorithm('CreateWorkspace', OutputWorkspace='First',DataX=[1.,2.,3.], DataY=[2.,3.], DataE=[2.,3.],UnitX='TOF')
        run_algorithm('CreateWorkspace', OutputWorkspace='Second',DataX=[4.,5.,6.], DataY=[4.,5.], DataE=[2.,3.],UnitX='TOF')
        run_algorithm('CreateWorkspace', OutputWorkspace='Third',DataX=[7.,8.,9.], DataY=[6.,7.], DataE=[2.,3.],UnitX='TOF')
        run_algorithm('GroupWorkspaces',InputWorkspaces='First,Second,Third',OutputWorkspace='grouped')
        group = mtd['grouped']
        
        self.assertRaises(IndexError, group.__getitem__, 3) # Index out of bounds
        for i in range(3):
            member = group[i]
            self.assertTrue(isinstance(member, MatrixWorkspace))

    def test_SimpleAlgorithm_Accepts_Group_Handle(self):
        from mantid.simpleapi import Load, Scale
        group = Load("CSP79590.raw")
        self.assertEquals(group.getName(), "group")
        self.assertEquals(type(group), WorkspaceGroup)
        try:
            w = Scale(group, 1.5)
            mtd.remove(str(w))
        except Exception, exc:
            self.fail("Algorithm raised an exception with input as WorkspaceGroup: '" + str(exc) + "'")
        mtd.remove(str(group))
