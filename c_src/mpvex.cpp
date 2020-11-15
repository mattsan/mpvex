#include <algorithm>
#include <stdexcept>

#include "mpvex.h"

namespace mpvex {

std::string Mpvex::error_string(int status) {
    return mpv_error_string(status);
}

std::string Mpvex::event_name(mpv_event_id event) {
    return mpv_event_name(event);
}

Mpvex::Mpvex() : destroyed_(false), ctx_() {
}

Mpvex::~Mpvex() {
    terminate_destroy();
}

void Mpvex::create() {
    ctx_ = mpv_create();

    if (!ctx_) {
        throw std::runtime_error("failed_creating_context");
    }
}

void Mpvex::terminate_destroy() {
    if (!destroyed_) {
        mpv_terminate_destroy(ctx_);
        destroyed_ = true;
    }
}

int Mpvex::command(const std::vector<std::string>& commands) {
    std::vector<const char*> cmds(commands.size() + 1);

    std::transform(
        commands.begin(),
        commands.end(),
        cmds.begin(),
        [](const std::string& s) { return s.c_str(); }
    );

    cmds.back() = 0;

    return mpv_command(ctx_, &cmds.front());
}

int Mpvex::initialize() {
    return mpv_initialize(ctx_);
}

int Mpvex::set_option(const std::string& name, mpv_format format, int data) {
    return mpv_set_option(ctx_, name.c_str(), format, &data);
}

int Mpvex::set_option_string(const std::string& name, const std::string& data) {
    return mpv_set_option_string(ctx_, name.c_str(), data.c_str());
}

} // namespace mpvex
