#ifndef MANTID_ICAT_CSEARCHBYRUNNUMBER_H_
#define MANTID_ICAT_CSEARCHBYRUNNUMBER_H_

#include "MantidAPI/Algorithm.h"
#include "MantidAPI/ITableWorkspace.h"
#include"MantidICat/SearchHelper.h"

/** CSearchByRunNumber is a class responsible for SearchByRunNumber algorithm.
  * This algorithm does the basic search and returns the investigations record

    Required Properties:
    <UL>
    <LI> StartRun - The the start run number for search </LI>
    <LI> EndRun - The end run number for search </LI>
	<LI> Instruments - The list of instruments used for search </LI>
	<LI> StartDate - The start date used for search </LI>
	<LI> EndDate - The end date used for search </LI>
	<LI> Keywords - The keyswords used for search </LI>
	<LI> Case Sensitive - The case ensitive boolean flag used for search </LI>
    </UL>
   
    @author Sofia Antony, ISIS Rutherford Appleton Laboratory 
    @date 07/07/2010
    Copyright &copy; 2010 ISIS Rutherford Appleton Laboratory & NScD Oak Ridge National Laboratory

    This file is part of Mantid.

    Mantid is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Mantid is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    File change history is stored at: <https://svn.mantidproject.org/mantid/trunk/Code/Mantid>.
    Code Documentation is available at: <http://doxygen.mantidproject.org>
    
	*/	


namespace Mantid
{
	namespace ICat
	{
		class DLLExport CSearchByRunNumber: public API::Algorithm
		{
		public:
			///constructor
			CSearchByRunNumber():API::Algorithm(){}
			///destructor
			~CSearchByRunNumber()
			{
			}
			/// Algorithm's name for identification overriding a virtual method
			virtual const std::string name() const { return "SearchByRunNumber"; }
			/// Algorithm's version for identification overriding a virtual method
			virtual int version() const { return 1; }
			/// Algorithm's category for identification overriding a virtual method
			virtual const std::string category() const { return "ICat"; }
		private:
			/// Overwrites Algorithm init method.
			void init();
			/// Overwrites Algorithm exec method
			void exec();
			
			/// Search method 
			void  doSearchByRunNumber(API::ITableWorkspace_sptr &outputws);
		
             /// get all inputs for the algorithm
			 void getInputProperties(CSearchHelper& helper,CSearchInput& inputs);

											
		};
	}
}

#endif
