# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2019 ISIS Rutherford Appleton Laboratory UKRI,
#     NScD Oak Ridge National Laboratory, European Spallation Source
#     & Institut Laue - Langevin
# SPDX - License - Identifier: GPL - 3.0 +
from __future__ import (absolute_import, division, print_function)

import unittest
import numpy as np

from MsdTestHelper import (is_registered, check_output, do_a_fit)


class SCgapAnisoSwaveTest(unittest.TestCase):

    def test_function_has_been_registered(self):
        status, msg = is_registered("SCgapAnisoSwave")
        if not status:
            self.fail(msg)

    def test_function_output(self):
        input = [0.1, 3.1, 6.1, 9.1]
        expected = [1.0, 0.785047, 0.527729, 0.0]
        tolerance = 1.0e-05
        status, output = check_output("SCgapAnisoSwave", input, expected, tolerance, Delta=1.4, Tcritical=9.0, A=0.81)
        if not status:
            msg = 'Computed output {} from input {} unequal to expected: {}'
            self.fail(msg.format(*[str(i) for i in (output, input, expected)]))

    def test_do_fit(self):
        do_a_fit(np.arange(0.1, 16, 0.2), 'SCgapAnisoSwave', guess=dict(Delta=1.45, Tcritical=9.1, A=0.81),
                 target=dict(Delta=1.4, Tcritical=9.0, A=0.81), atol=0.01)


if __name__ == '__main__':
    unittest.main()
