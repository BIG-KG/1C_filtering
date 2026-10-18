#include "judge_interface.hpp"

#include <iostream>
#include <stdexcept>

namespace rooms {

std::vector<PathInfo> JudgeInterface::look(std::size_t passage_count) {
  std::cout << "LOOK" << std::endl;

  std::vector<PathInfo> paths;
  paths.reserve(passage_count);

  for (std::size_t i = 0; i < passage_count; ++i) {
    std::string label;
    std::string state;
    PathInfo path{};

    if (!(std::cin >> label >> path.passage_id >> path.destination_id >>
          state)) {
      throw std::runtime_error("Failed to read LOOK response");
    }

    if (state != "OK" && state != "BROKEN") {
      throw std::runtime_error("Invalid LOOK response");
    }

    path.is_open = state == "OK";
    paths.push_back(path);
  }

  return paths;
}

bool JudgeInterface::move(int passage_id) {
  std::cout << "MOVE " << passage_id << std::endl;

  std::string reply = read_reply();

  if (reply == "OK") {
    return true;
  }

  if (reply == "FAIL") {
    return false;
  }

  throw std::runtime_error("Invalid MOVE response");
}

void JudgeInterface::clear(int passage_id) {
  std::cout << "CLEAR " << passage_id << std::endl;

  if (read_reply() != "OK") {
    throw std::runtime_error("Invalid CLEAR response");
  }
}

ExpeditionResult JudgeInterface::end() {
  std::cout << "END" << std::endl;

  std::string label;
  ExpeditionResult result{};

  if (!(std::cin >> label >> result.collected_prize >> result.days >>
        result.profit) ||
      label != "RESULT") {
    throw std::runtime_error("Invalid END response");
  }

  return result;
}

std::string JudgeInterface::read_reply() {
  std::string reply;

  if (!(std::cin >> reply)) {
    throw std::runtime_error("Failed to read judge response");
  }

  return reply;
}

}  // namespace rooms