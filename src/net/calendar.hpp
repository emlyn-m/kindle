#pragma once

#include <ctime>
#include <functional>
#include <string>
#include <vector>

namespace ui {

struct CalEvent {
  std::string id;
  std::string title;
  time_t start_time;
  time_t end_time;
};

struct Calendar {
  std::vector<CalEvent> events;
  std::vector<CalEvent> todos;
  time_t last_update = 0;
};

void calendar_start(std::function<void()> on_update);
const Calendar &calendar_state();

} // namespace ui
