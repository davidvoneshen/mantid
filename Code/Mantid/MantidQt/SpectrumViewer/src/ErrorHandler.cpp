#include <iostream>

#include "MantidKernel/Logger.h"

#include "MantidQtSpectrumViewer/ErrorHandler.h"

namespace MantidQt
{
namespace ImageView
{

using namespace Mantid;

Kernel::Logger& g_log = Kernel::Logger::get("ImageView");

/**
 * Display the specified string in an error message.
 * 
 * @param text   The string containing the text of the error message 
 */
void ErrorHandler::Error( std::string  text )
{
  g_log.error( "ERROR: " + text );
}


/**
 * Display the specified string in a warning message.
 * 
 * @param text   The string containing the text of the warning message 
 */
void ErrorHandler::Warning( std::string  text )
{
  g_log.warning( "WARNING: " + text );
}


/**
 * Display the specified string in a warning message.
 * 
 * @param text   The string containing the text of the warning message 
 */
void ErrorHandler::Notice( std::string  text )
{
  g_log.notice( "Notice: " + text );
}


} // namespace MantidQt 
} // namespace ImageView 
