# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
#     NScD Oak Ridge National Laboratory, European Spallation Source
#     & Institut Laue - Langevin
# SPDX - License - Identifier: GPL - 3.0 +

from qtpy.QtCore import QObject, QRunnable, Signal

import mantid.simpleapi as sapi

from .DrillAlgorithmObserver import DrillAlgorithmObserver

class DrillTaskSignals(QObject):
    """
    Signals that the DrillTask could send.
    """
    started = Signal(int)         # task reference
    finished = Signal(int, int)   # task reference, return value (0: success)
    progress = Signal(int, float) # task reference and progress between 0.0 and 1.0


class DrillTask(QRunnable):
    """
    Class that defines a processing task in the drill interface.
    """
    def __init__(self, ref, alg, **kwargs):
        super(DrillTask, self).__init__()
        self.ref = ref
        self.signals = DrillTaskSignals()
        # setup the algo
        self.alg = sapi.AlgorithmManager.create(alg)
        # check every properties before throwing an exception
        errors = list()
        for (k, v) in kwargs.items():
            try:
                self.alg.setProperty(k, v)
            except Exception as e:
                errors.append(str(e))
        if errors:
            raise Exception("* " + "\n* ".join(errors))
        # setup the observer
        self.observer = DrillAlgorithmObserver()
        self.observer.observeFinish(self.alg)
        self.observer.observeError(self.alg)
        self.observer.signals.finished.connect(
                lambda ret : self.signals.finished.emit(self.ref, ret)
                )
        self.observer.observeProgress(self.alg)
        self.observer.signals.progress.connect(
                lambda p : self.signals.progress.emit(self.ref, p)
                )

    def run(self):
        """
        Override QRunnable::run. Provide the running part of the task that will
        start in an other thread.
        """
        self.signals.started.emit(self.ref)
        try:
            ret = self.alg.execute()
            if not ret:
                self.signals.finished.emit(self.ref, 1)
        except:
            self.signals.finished.emit(self.ref, 1)

    def cancel(self):
        """
        Cancel the algorithm. This function first tests if the algorithm is
        currently running. It avoids emitting error signal for cancelling non
        running ones.
        """
        if self.alg.isRunning():
            self.alg.cancel()
            self.signals.finished.emit(self.ref, 1)

