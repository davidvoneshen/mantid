#ifndef MANTID_SLICEVIEWER_PEAKSPRESENTER_H_
#define MANTID_SLICEVIEWER_PEAKSPRESENTER_H_

#include "MantidKernel/System.h"
#include <boost/shared_ptr.hpp>
#include "MantidKernel/System.h"

namespace Mantid
{
  namespace API
  {
    // Forward dec.
    class IPeaksWorkspace;
  }
}

namespace MantidQt
{
namespace SliceViewer
{
  // Forward dec.
  class PeakOverlayViewFactory;
  class PeakOverlayView;

  /*---------------------------------------------------------
  Abstract PeaksPresenter.

  This is abstract to allow usage of the NULL object pattern. This allows the ConcreteViewPresenter to be conctructed in an atomic sense after the constrution of the owning object,
  whithout having to perform fragile null checks.

  ----------------------------------------------------------*/
  class DLLExport PeaksPresenter
  {
  public:
    virtual void update() = 0;
    virtual void updateWithSlicePoint(const double&) = 0;
  };


  typedef boost::shared_ptr<PeaksPresenter> PeaksPresenter_sptr;
  typedef boost::shared_ptr<const PeaksPresenter> PeaksPresenter_const_sptr;
}
}

#endif /* MANTID_SLICEVIEWER_PEAKSPRESENTER_H_ */