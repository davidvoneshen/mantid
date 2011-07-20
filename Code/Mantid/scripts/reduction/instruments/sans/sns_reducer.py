"""
    Reducer class for EQSANS reduction
"""
from sans_reducer import SANSReducer
from reduction import ReductionStep
import sns_reduction_steps
import sans_reduction_steps

class EqSansReducer(SANSReducer):

    def __init__(self):
        super(EqSansReducer, self).__init__()
        ## Default beam finder
        self._beam_finder = sans_reduction_steps.BaseBeamFinder(96.29, 126.15)
        ## Default data loader
        self._data_loader = sns_reduction_steps.LoadRun()
        ## Normalization
        self._normalizer = sns_reduction_steps.Normalize()
        ## Transmission calculator
        self._transmission_calculator = sns_reduction_steps.BeamStopTransmission(True)
        
        # Default dark current subtracter class
        self._dark_current_subtracter_class = sns_reduction_steps.SubtractDarkCurrent
        
        # No resolution calculation for now
        self._resolution_calculator = None

    def set_instrument(self, configuration):
        super(SANSReducer, self).set_instrument(configuration)
