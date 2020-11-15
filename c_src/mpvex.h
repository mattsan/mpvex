#ifndef MPVEX_MPVEX_H_
#define MPVEX_MPVEX_H_

#include <vector>
#include <string>

#include <mpv/client.h>

namespace mpvex {

class Mpvex {
public:
    static std::string error_string(int status);
    static std::string event_name(mpv_event_id event);

    Mpvex();
    ~Mpvex();

    void create();
    int command(const std::vector<std::string>& commands);
    int initialize();
    int set_option(const std::string& name, mpv_format format, int data);
    int set_option_string(const std::string& name, const std::string& data);
    void terminate_destroy();

private:
    bool destroyed_;
    mpv_handle* ctx_;
};

} // namespace mpvex

#endif//MPVEX_MPVEX_H_
