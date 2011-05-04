#ifndef BASE_H
#define BASE_H

#include "Log.h"
#include "ErrorChecking.h"
#include "PerfTimer.h"

#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <iostream>


#include <SDL/SDL.h>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <platformstl/performance/performance_counter.hpp>

#endif //BASE_H
