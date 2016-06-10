#include "Log.h"

#include "unicode/ustream.h"
#include "unicode/unistr.h"
using namespace icu;

#include "GlobalArgs.h"


bool ascii::Log::Enabled()
{
    return ascii::GlobalArgs::Enabled("log");
}
