#pragma once

#include <core/event.h>
#include <SDL.h>

namespace kairos {

/** Tag class used to indicate a SDL implmentation of event handling. */
class sdl_event_tag {
};

/** Peek all events from SDL since last call. 
* SDL_Event are converted to kairos::system_event before being emplaced in events parameter.
*/
void peek_events(const sdl_event_tag& sdl, array<system_event>& events);

system_event convert(const SDL_Event& e);

} // namespace kairos