# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2019 ISIS Rutherford Appleton Laboratory UKRI,
#     NScD Oak Ridge National Laboratory, European Spallation Source
#     & Institut Laue - Langevin
# SPDX - License - Identifier: GPL - 3.0 +
#  This file is part of the mantid workbench.

from __future__ import (absolute_import, unicode_literals)

from qtpy.QtCore import Qt
from qtpy.QtWidgets import QWidget

from mantidqt.widgets.plotconfigdialog.colorselector import ColorSelector
from mantidqt.widgets.plotconfigdialog.curvestabwidget import CurveProperties
from mantidqt.utils.qt import load_ui


class CurvesTabWidgetView(QWidget):

    def __init__(self, parent=None, hide_errorbars_tab=True):
        super(CurvesTabWidgetView, self).__init__(parent=parent)

        self.ui = load_ui(__file__,
                          'curves_tab.ui',
                          baseinstance=self)
        self.setAttribute(Qt.WA_DeleteOnClose, True)

        self.line = LineTabWidget(self)
        self.tab_widget.addTab(self.line, "Line")
        self.marker = MarkerTabWidget(self)
        self.tab_widget.addTab(self.marker, "Marker")
        if not hide_errorbars_tab:
            self.errorbars = ErrorbarsTabWidget(self)
            self.tab_widget.addTab(self.errorbars, "Errorbars")

    def populate_select_axes_combo_box(self, axes_names):
        self.select_axes_combo_box.addItems(axes_names)

    def populate_select_curve_combo_box(self, curve_names):
        self.select_curve_combo_box.addItems(curve_names)

    def set_current_curve_selector_text(self, new_text):
        current_index = self.select_curve_combo_box.currentIndex()
        self.select_curve_combo_box.setItemText(current_index, new_text)

    def remove_select_axes_combo_box_current_item(self):
        current_index = self.select_axes_combo_box.currentIndex()
        self.select_axes_combo_box.removeItem(current_index)

    def remove_select_curve_combo_box_current_item(self):
        current_index = self.select_curve_combo_box.currentIndex()
        self.select_curve_combo_box.removeItem(current_index)

    # Top level entries
    def get_selected_ax_name(self):
        return self.select_axes_combo_box.currentText()

    def get_selected_curve_name(self):
        return self.select_curve_combo_box.currentText()

    def set_curve_label(self, label):
        self.curve_label_line_edit.setText(label)

    def get_curve_label(self):
        return self.curve_label_line_edit.text()

    def get_hide_curve(self):
        return self.hide_curve_check_box.checkState()

    def set_hide_curve(self, state):
        self.hide_curve_check_box.setCheckState(state)

    # Property object getters and setters
    def get_properties(self):
        return CurveProperties.from_view(self)

    def set_properties(self, curve_props):
        self.set_curve_label(curve_props.label)
        self.set_hide_curve(curve_props.hide_curve)
        # Line properties
        self.line.set_style(curve_props.line_style)
        self.line.set_draw_style(curve_props.draw_style)
        self.line.set_width(curve_props.line_width)
        self.line.set_color(curve_props.line_color)
        # Marker properties
        self.marker.set_style(curve_props.marker_style)
        self.marker.set_size(curve_props.marker_size)
        self.marker.set_face_color(curve_props.marker_face_color)
        self.marker.set_edge_color(curve_props.marker_edge_color)


class LineTabWidget(QWidget):

    def __init__(self, parent=None):
        super(LineTabWidget, self).__init__(parent=parent)

        self.ui = load_ui(__file__,
                          'curves_tab_line_tab.ui',
                          baseinstance=self)
        self.color_selector_widget = ColorSelector(parent=self)
        self.grid_layout.replaceWidget(self.color_selector_dummy_widget,
                                       self.color_selector_widget)
        self.setAttribute(Qt.WA_DeleteOnClose, True)

    def get_style(self):
        return self.line_style_combo_box.currentText()

    def set_style(self, line_style):
        self.line_style_combo_box.setCurrentText(line_style)

    def get_draw_style(self):
        return self.draw_style_combo_box.currentText()

    def set_draw_style(self, draw_style):
        self.draw_style_combo_box.setCurrentText(draw_style)

    def get_width(self):
        return self.line_width_spin_box.value()

    def set_width(self, width):
        self.line_width_spin_box.setValue(width)

    def get_color(self):
        return self.color_selector_widget.current_color

    def set_color(self, color_hex):
        self.color_selector_widget.set_color(color_hex)


class MarkerTabWidget(QWidget):

    def __init__(self, parent=None):
        super(MarkerTabWidget, self).__init__(parent=parent)

        self.ui = load_ui(__file__,
                          'curves_tab_marker_tab.ui',
                          baseinstance=self)
        self.setAttribute(Qt.WA_DeleteOnClose, True)

    def get_style(self):
        return self.marker_style_combo_box.currentText()

    def set_style(self, style):
        self.marker_style_combo_box.setCurrentText(style)

    def get_size(self):
        return self.marker_size_spin_box.value()

    def set_size(self, size):
        self.marker_size_spin_box.setValue(size)

    def get_face_color(self):
        return self.marker_face_color_line_edit.text()

    def set_face_color(self, color):
        self.marker_face_color_line_edit.setText(color)

    def get_edge_color(self):
        return self.marker_edge_color_line_edit.text()

    def set_edge_color(self, color):
        self.marker_edge_color_line_edit.setText(color)


class ErrorbarsTabWidget(QWidget):

    def __init__(self, parent=None):
        super(ErrorbarsTabWidget, self).__init__(parent=parent)

        self.ui = load_ui(__file__,
                          'curves_tab_errobars_tab.ui',
                          baseinstance=self)
        self.setAttribute(Qt.WA_DeleteOnClose, True)

    def get_show_errorbars(self):
        return self.show_errorbars_tickbox.checkState()

    def set_show_errorbars(self, state):
        self.show_errorbars_tickbox.setState(state)

    def get_width(self):
        return self.width_spin_box.value()

    def set_width(self, width):
        self.width_spin_box.setValue(width)

    def get_capsize(self):
        return self.capsize_spin_box.value()

    def set_capsize(self, size):
        self.capsize_spin_box.setValue(size)

    def get_cap_thickness(self):
        return self.cap_thickness_spin_box.value()

    def set_cap_thickness(self, thickness):
        self.cap_thickness_spin_box.setValue(thickness)

    def get_error_every(self):
        return self.error_every_spin_box.value()

    def set_error_every(self, error_every):
        self.error_every_spin_box.setValue(error_every)

    def get_color(self):
        return self.color_line_edit.text()

    def set_color(self, color):
        self.color_line_edit.setText(color)
