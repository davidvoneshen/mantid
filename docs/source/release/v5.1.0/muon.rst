============
MuSR Changes
============

.. contents:: Table of Contents
   :local:

.. warning:: **Developers:** Sort changes under appropriate heading
    putting new features at the top of the section, followed by
    improvements, followed by bug fixes.

Muon Analysis 2
################

New Features
------------
- The plotting logic within the Muon Analysis 2 GUI has updated to prevent all the workspaces from a
  fit being plotted at the same time. Instead, the choice of which fit workspace to plot can be made in
  fitting and sequential fitting tabs.
- When the user switches to the fitting tab, the workspace present in the fit display box is plotted.
  To switch back to a view of all the data, the user can switch to the home, grouping or phase table tabs.
- The sequential fitting table now allows multiple selections to be made.
- Addition of an external plotting button to the Muon Analysis 2 GUI.
  This allows the user to create a standalone Workbench (or MantidPlot) plot of the displayed data.
  The user may then perform standard operations on the plot, e.g drag and drop workspaces onto the figure.
- The loading in the Muon and Frequency domain interfaces has been sped up by reducing the number of calls made to algorithms.
  On average, this should result in a 50% decrease in load times. This reduction in the number of algorithms also improves
  the clarity of the workspace history, as the number of algorithms present in the history is now reduced.
- On the fitting tab, only one fit object (fit output and input workspaces) will be shown at a time.
- Addition of background correction algorithm (PSIBackgroundCorrection) to remove the background present in
  PSI bin data loaded using LoadPSIMuonBin.
- Addition of a LoadMuonNexusV2 algorithm to load the new Muon V2 files, see :ref:`LoadMuonNexusV2 <algm-LoadMuonNexusV2>`.
- Updated rounding for time zero and first good data to be 3 decimal places.
- Added double pulse analysis, see :ref:`Muon home tab <muon_home_tab-ref>`.
- Added multi-period support to the LoadMuonNexusV2 algorithm.
- Added two buttons to the Muon analysis and Frequency domain analysis plot toolbar to allow users to show major and minor gridlines.


Improvements
-------------
- Improved the plotting code the Muon Analysis interface, with a significant amount of redundant code removed.
  This code improvement will lead to more maintainable code, which will be more stable and
  easier to add new functionality in the future.
- Improved the maintainability of the fitting code in the Muon Analysis interface by removing redundant and duplicated code.
  These changes will make the addition of new functionality in the future easier.
- The plot guess option within the fitting tab will now update when a parameter is changed.
- Have updated the FDA GUI so that it functions correctly for frquency transforms and single fits.
- Updated :ref:`DoublePulseFit <algm-DoublePulseFit>` to allow composite function input.
- Updated :ref:`CalculateMuonAsymmetry <algm-CalculateMuonAsymmetry>` to allow double pulse fits.
- Tf asymmetry mode can now be performed on double pulse fits from the muon analysis GUI.
- Updated :ref:`LoadMuonNexusV2 <algm-LoadMuonNexusV2>` and  :ref:`LoadPSIMuonBin <algm-LoadPSIMuonBin>` to load a list of time zeros into a new property TimZeroList.
- Updated :ref:`LoadMuonNexusV2 <algm-LoadMuonNexusV2>` and  :ref:`LoadPSIMuonBin <algm-LoadPSIMuonBin>` to add an option to not auto-correct the time by loaded timezero.
- Fitting tab in Muon analysis and Frequency domain analysis GUI's are now disabled when no valid fitting data is present.
- Updated plotting to make line colours more consistent.
- The ALC interface in workbench will now show errors by default. The error bars can also be turned on/off using the right-click plot menu.
- Have updated the FDA GUI so that it functions correctly for frequency transforms and single fits.

Bug fixes
---------
- Fixed an issue where ties set in Muon Analysis were not being respected.
- The plot will always match the state of the `Plot raw` checkbox.
- Fixed a bug that caused rebinned data to override non-binned data.
- Fixed an issue where switching to simultaneous fit mode was occasionally throwing an exception.
- Fixed an issue where loading additional data in simultaneous fit mode was throwing an exception.
- Fixed an issue where mantid crashed when the muon analysis plotting window crashed was resized to be too small.
- Fixed an issue with setting the current workspace before adding a function.
- Fixed an issue with the results tab not updating correctly after multiple fits with different functions.
- Fixed an issue where Muon Analysis and Frequency Domain Analysis gui was not properly disabling during calculations.


Elemental Analysis 
##################

New Features
------------
- Added a deselect all elements button.
- Fixed an issue where groups were all being plotted on the same tiled plot.


:ref:`Release 5.1.0 <v5.1.0>`
