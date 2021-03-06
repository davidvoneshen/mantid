# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
#   NScD Oak Ridge National Laboratory, European Spallation Source,
#   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
# SPDX - License - Identifier: GPL - 3.0 +
import numpy as np


# inner loop Z=sum_L(S2(K,L)*beta_L)
# outer loop W=sum(beta(k)*Z(k))
def DIST(SPACE_beta, SPACE_s2):
    return -np.dot(SPACE_beta, np.dot(SPACE_s2, SPACE_beta))
