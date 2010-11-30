#ifndef MANTIDQTCUSTOMINTERFACES_INDIRECTANALYSIS_H_
#define MANTIDQTCUSTOMINTERFACES_INDIRECTANALYSIS_H_

//----------------------
// Includes
//----------------------
#include "MantidQtCustomInterfaces/ui_IndirectDataAnalysis.h"
#include "MantidQtAPI/UserSubWindow.h"

#include "MantidQtMantidWidgets/RangeSelector.h"

#include "MantidAPI/CompositeFunction.h"
#include "MantidAPI/MatrixWorkspace.h"
#include "MantidKernel/ConfigService.h"

#include <QIntValidator>
#include <QDoubleValidator>

// Editor Factories
#include "DoubleEditorFactory.h"
#include "StringDialogEditorFactory.h"
#include <QtCheckBoxFactory>

#include <Poco/NObserver.h>

#include "qttreepropertybrowser.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

namespace MantidQt
{
  namespace CustomInterfaces
  {
    class IndirectDataAnalysis : public MantidQt::API::UserSubWindow
    {
      Q_OBJECT

    public:
      /// The name of the interface as registered into the factory
      static std::string name() { return "Indirect Data Analysis"; }
      /// Default Constructor
      IndirectDataAnalysis(QWidget *parent = 0);

    private:
      /// Initialize the layout
      virtual void initLayout();
      /// init python-dependent sections
      virtual void initLocalPython();

      void loadSettings();

      // Tab-specific setup stages (mainly miniplots)
      void setupElwin();
      void setupMsd();
      void setupFuryFit();
      void setupConFit();

      // Validation of user inputs
      bool validateElwin();
      bool validateMsd();
      bool validateFury();
      bool validateAbsorption();

      // Run processes
      void elwinRun();
      void msdRun();
      void furyRun();
      void furyfitRun();
      void confitRun();
      void absorptionRun();

      Mantid::API::CompositeFunction* createFunction(QtTreePropertyBrowser* propertyBrowser);
      Mantid::API::CompositeFunction* confitCreateFunction(bool tie=false);
      QtProperty* createLorentzian(QString);
      QtProperty* createExponential();
      QtProperty* createStretchedExp();
      void populateFunction(Mantid::API::IFunction*, Mantid::API::IFunction*, QtProperty*, int, bool tie=false);
      QwtPlotCurve* plotMiniplot(QwtPlot* plot, QwtPlotCurve* curve, std::string workspace, int index);
      
      virtual void closeEvent(QCloseEvent*);

      void handleDirectoryChange(Mantid::Kernel::ConfigValChangeNotification_ptr pNf); ///< handle POCO event

    private slots:
      // Generic
      void refreshWSlist();
      void run();

      // ElasticWindow
      void elwinPlotInput();
      void elwinTwoRanges(QtProperty*, bool);
      void elwinMinChanged(double val);
      void elwinMaxChanged(double val);
      void elwinUpdateRS(QtProperty* prop, double val);

      // MSD Fit
      void msdPlotInput();
      void msdMinChanged(double val);
      void msdMaxChanged(double val);
      void msdUpdateRS(QtProperty* prop, double val);

      // Fourier Transform
      void furyInputType(int index);
      void furyResType(const QString& type);
      void furyPlotInput();

      // Fourier Transform Fit
      void furyfitTypeSelection(int index);
      void furyfitPlotInput();
      void furyfitXMinSelected(double val);
      void furyfitXMaxSelected(double val);
      void furyfitBackgroundSelected(double val);
      void furyfitRangePropChanged(QtProperty*, double);
      void furyfitInputType(int index);
      void furyfitSequential();
      void furyfitPlotGuess(QtProperty*);

      // Convolution Fit
      void confitTypeSelection(int index);
      void confitInputType(int index);
      void confitPlotInput();
      void confitPlotGuess(QtProperty*);
      void confitSequential();
      void confitMinChanged(double);
      void confitMaxChanged(double);
      void confitBackgLevel(double);
      void confitUpdateRS(QtProperty*, double);
      void confitCheckBoxUpdate(QtProperty*, bool);
      void confitHwhmChanged(double);
      void confitHwhmUpdateRS(double);

      // Absorption (Basic)
      void absorptionShape(int index);
      
      // Common Elements
      void help();
      void openDirectoryDialog();
      
    private:
      Ui::IndirectDataAnalysis m_uiForm;
      int m_nDec;
      QIntValidator *m_valInt;
      QDoubleValidator *m_valDbl;

      bool m_furyResFileType;

      // Editor Factories
      DoubleEditorFactory *m_dblEdFac;
      StringDialogEditorFactory *m_strEdFac;
      QtCheckBoxFactory *m_blnEdFac;
      
      // ELASTICWINDOW MINIPLOT (prefix: 'm_elw')
      QwtPlot* m_elwPlot;
      MantidWidgets::RangeSelector* m_elwR1;
      MantidWidgets::RangeSelector* m_elwR2;
      QwtPlotCurve* m_elwDataCurve;
      QtTreePropertyBrowser* m_elwTree;
      QMap<QString, QtProperty*> m_elwProp;
      QtDoublePropertyManager* m_elwDblMng;
      QtBoolPropertyManager* m_elwBlnMng;
      QtGroupPropertyManager* m_elwGrpMng;

      // MSD Fit MiniPlot (prefix: 'm_msd')
      QwtPlot* m_msdPlot;
      MantidWidgets::RangeSelector* m_msdRange;
      QwtPlotCurve* m_msdDataCurve;
      QtTreePropertyBrowser* m_msdTree;
      QMap<QString, QtProperty*> m_msdProp;
      QtDoublePropertyManager* m_msdDblMng;

      // Fury Fit Member Variables (prefix 'm_ff')
      QtTreePropertyBrowser* m_ffTree; ///< FuryFit Property Browser
      QtGroupPropertyManager* m_groupManager;
      QtDoublePropertyManager* m_doubleManager;
      QtDoublePropertyManager* m_ffRangeManager; ///< StartX and EndX for FuryFit
      QMap<QString, QtProperty*> m_ffProp;
      QwtPlot* m_ffPlot;
      QwtPlotCurve* m_ffDataCurve;
      QwtPlotCurve* m_ffFitCurve;
      MantidQt::MantidWidgets::RangeSelector* m_ffRangeS;
      MantidQt::MantidWidgets::RangeSelector* m_ffBackRangeS;
      Mantid::API::MatrixWorkspace_const_sptr m_ffInputWS;
      Mantid::API::MatrixWorkspace_const_sptr m_ffOutputWS;
      std::string m_ffInputWSName;
      QString m_furyfitTies;

      // Confit (prefix: 'm_cf')
      QtTreePropertyBrowser* m_cfTree;
      QwtPlot* m_cfPlot;
      QMap<QString, QtProperty*> m_cfProp;
      MantidWidgets::RangeSelector* m_cfRangeS;
      MantidWidgets::RangeSelector* m_cfBackgS;
      MantidWidgets::RangeSelector* m_cfHwhmRange;
      QtGroupPropertyManager* m_cfGrpMng;
      QtDoublePropertyManager* m_cfDblMng;
      QtBoolPropertyManager* m_cfBlnMng;
      QtStringPropertyManager* m_cfStrMng;
      QwtPlotCurve* m_cfDataCurve;
      QwtPlotCurve* m_cfCalcCurve;
      Mantid::API::MatrixWorkspace_const_sptr m_cfInputWS;
      std::string m_cfInputWSName;

      /// Change Observer for ConfigService (monitors user directories)
      Poco::NObserver<IndirectDataAnalysis, Mantid::Kernel::ConfigValChangeNotification> m_changeObserver;

    };
  }
}
#endif //MANTIDQTCUSTOMINTERFACES_INDIRECTANALYSIS_H_