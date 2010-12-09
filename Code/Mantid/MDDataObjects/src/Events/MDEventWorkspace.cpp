#include "MDDataObjects/Events/MDEventWorkspace.h"
#include "MDDataObjects/Events/MDEvent.h"

namespace Mantid
{
namespace MDDataObjects
{

  //-----------------------------------------------------------------------------------------------
  /** Returns the number of dimensions in this workspace */
template <size_t nd, size_t nv, typename TE>
  int MDEventWorkspace<nd,nv,TE>::getNumDims() const
  {
    return nd;
  }

  /** Returns the total number of points (events) in this workspace */
  template <size_t nd, size_t nv, typename TE>
  size_t MDEventWorkspace<nd,nv,TE>::getNPoints() const
  {
    //return data.size();
    return 0;
  }

  /** Returns the number of bytes of memory
   * used by the workspace. */
  template <size_t nd, size_t nv, typename TE>
  size_t MDEventWorkspace<nd,nv,TE>::getMemoryUsed() const
  {
    return this->getNPoints() * sizeof(MDEvent<nd>);
  }



  //-----------------------------------------------------------------------------------------------


  // We export a bunch of version of MDEventWorkspace with various dimension sizes.
  template DLLExport class MDEventWorkspace<1>;
  template DLLExport class MDEventWorkspace<2>;
  template DLLExport class MDEventWorkspace<3>;
  template DLLExport class MDEventWorkspace<4>;




}//namespace MDDataObjects

}//namespace Mantid

