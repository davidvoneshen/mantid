from __future__ import print_function

from qtpy.QtCore import QPoint

from mantid.simpleapi import *
from mantidqt.utils.qt.test.gui_window_test import *

from workbench.plotting.functions import plot
from workbench.plotting.globalfiguremanager import GlobalFigureManager
from workbench.test.workbenchtests import runTests


class TestFitPropertyBrowser(WorkbenchGuiTest):

    def start(self):
        if 'ws' not in mtd:
            ws = Load(r'irs26176_graphite002_conv_1LFixF_s0_to_9_Result.nxs', OutputWorkspace='ws')
        else:
            ws = mtd['ws']
        plot([ws], [1])
        manager = GlobalFigureManager.get_active()
        self.w = manager.window
        trigger_action(find_action_with_text(self.w, 'Fit'))
        yield 0.1
        self.fit_browser = manager.fit_browser

    def test_fit_range(self):
        yield self.start()
        start_x = self.fit_browser.startX()
        start_x_pxl = self.fit_browser.tool.fit_start_x.get_x_in_pixels()
        end_x = self.fit_browser.endX()
        end_x_pxl = self.fit_browser.tool.fit_end_x.get_x_in_pixels()
        self.assertGreater(end_x, start_x)
        self.assertGreater(start_x, 0.3)
        self.assertGreater(2.0, end_x)
        pos = self.w._canvas.geometry().center()
        canvas = self.w.childAt(pos)
        pos.setX(self.fit_browser.tool.fit_start_x.get_x_in_pixels())
        new_pos = pos + QPoint(100, 0)
        yield drag_mouse(canvas, pos, new_pos)
        self.assertAlmostEqual(self.fit_browser.startX(), start_x + 100.0 / (end_x_pxl - start_x_pxl) * (end_x - start_x), 2)
        pos.setX(self.fit_browser.tool.fit_end_x.get_x_in_pixels())
        new_pos = pos - QPoint(30, 0)
        yield drag_mouse(canvas, pos, new_pos)
        self.assertAlmostEqual(self.fit_browser.endX(), end_x - 30.0 / (end_x_pxl - start_x_pxl) * (end_x - start_x), 2)

    def test_fit_range_start_moved_too_far(self):
        yield self.start()
        start_x = self.fit_browser.startX()
        end_x = self.fit_browser.endX()
        start_x_pxl = self.fit_browser.tool.fit_start_x.get_x_in_pixels()
        end_x_pxl = self.fit_browser.tool.fit_end_x.get_x_in_pixels()
        self.assertGreater(end_x, start_x)
        self.assertGreater(start_x, 0.3)
        self.assertGreater(2.0, end_x)

        pos = self.w._canvas.geometry().center()
        canvas = self.w.childAt(pos)
        pos.setX(self.fit_browser.tool.fit_end_x.get_x_in_pixels())
        new_pos = pos - QPoint(100, 0)
        yield drag_mouse(canvas, pos, new_pos)
        new_end_x = self.fit_browser.endX()
        self.assertAlmostEqual(new_end_x, end_x - 100.0 / (end_x_pxl - start_x_pxl) * (end_x - start_x), 2)

        pos.setX(self.fit_browser.tool.fit_start_x.get_x_in_pixels())
        new_pos = pos + QPoint()
        new_pos.setX(end_x_pxl)
        yield drag_mouse(canvas, pos, new_pos)
        self.assertAlmostEqual(self.fit_browser.startX(), new_end_x)

    def test_fit_range_end_moved_too_far(self):
        yield self.start()
        start_x = self.fit_browser.startX()
        start_x_pxl = self.fit_browser.tool.fit_start_x.get_x_in_pixels()
        end_x = self.fit_browser.endX()
        end_x_pxl = self.fit_browser.tool.fit_end_x.get_x_in_pixels()
        self.assertGreater(end_x, start_x)
        self.assertGreater(start_x, 0.3)
        self.assertGreater(2.0, end_x)
        pos = self.w._canvas.geometry().center()
        canvas = self.w.childAt(pos)
        pos.setX(self.fit_browser.tool.fit_start_x.get_x_in_pixels())
        new_pos = pos + QPoint(100, 0)
        yield drag_mouse(canvas, pos, new_pos)
        new_start_x = self.fit_browser.startX()
        self.assertAlmostEqual(new_start_x, start_x + 100.0 / (end_x_pxl - start_x_pxl) * (end_x - start_x), 2)

        pos.setX(self.fit_browser.tool.fit_end_x.get_x_in_pixels())
        new_pos = pos + QPoint()
        new_pos.setX(start_x_pxl)
        yield drag_mouse(canvas, pos, new_pos)
        self.assertAlmostEqual(self.fit_browser.endX(), new_start_x)

    def test_fit_range_moved_start_outside(self):
        yield self.start()
        start_x_pxl = self.fit_browser.tool.fit_start_x.get_x_in_pixels()
        pos = self.w._canvas.geometry().center()
        canvas = self.w.childAt(pos)
        pos.setX(self.fit_browser.tool.fit_start_x.get_x_in_pixels())
        new_pos = pos - QPoint(pos.x(), 0)
        yield drag_mouse(canvas, pos, new_pos)
        self.assertAlmostEqual(start_x_pxl, self.fit_browser.tool.fit_start_x.get_x_in_pixels())

    def test_fit_range_moved_end_outside(self):
        yield self.start()
        end_x_pxl = self.fit_browser.tool.fit_end_x.get_x_in_pixels()
        pos = self.w._canvas.geometry().center()
        canvas = self.w.childAt(pos)
        pos.setX(self.fit_browser.tool.fit_end_x.get_x_in_pixels())
        new_pos = pos + QPoint(0, canvas.width())
        yield drag_mouse(canvas, pos, new_pos)
        self.assertAlmostEqual(end_x_pxl, self.fit_browser.tool.fit_end_x.get_x_in_pixels())

    def test_fit_range_set_start(self):
        yield self.start()
        self.fit_browser.setStartX(0.7)
        self.assertAlmostEqual(self.fit_browser.tool.fit_start_x.x, 0.7)

    def test_fit_range_set_start_outside(self):
        yield self.start()
        self.fit_browser.setStartX(0.1)
        self.assertAlmostEqual(self.fit_browser.tool.fit_start_x.x, 0.1)

    def test_fit_range_set_start_outside_right(self):
        yield self.start()
        self.fit_browser.setStartX(2.0)
        self.assertAlmostEqual(self.fit_browser.tool.fit_start_x.x, self.fit_browser.endX())

    def test_fit_range_set_end(self):
        yield self.start()
        self.fit_browser.setEndX(1.0)
        self.assertAlmostEqual(self.fit_browser.tool.fit_end_x.x, 1.0)

    def test_fit_range_set_end_outside(self):
        yield self.start()
        self.fit_browser.setEndX(2.0)
        self.assertAlmostEqual(self.fit_browser.tool.fit_end_x.x, 2.0)

    def test_fit_range_set_end_outside_left(self):
        yield self.start()
        self.fit_browser.setEndX(0.3)
        self.assertAlmostEqual(self.fit_browser.tool.fit_end_x.x, self.fit_browser.startX())

    def test_fit(self):
        yield self.start()
        self.fit_browser.loadFunction('name=LinearBackground')
        self.fit_browser.fit()
        yield self.wait_for_true(lambda: 'ws_Workspace' in mtd)
        # self.assertEqual(self.fit_browser.getFittingFunction(), "name=LinearBackground,A0=4.74354,A1=-0.442138")
        self.assertEqual(len(self.fit_browser.fit_result_lines), 2)
        del mtd['ws_Workspace']
        self.fit_browser.fit()
        yield self.wait_for_true(lambda: 'ws_Workspace' in mtd)
        self.assertEqual(len(self.fit_browser.fit_result_lines), 2)


runTests(TestFitPropertyBrowser)