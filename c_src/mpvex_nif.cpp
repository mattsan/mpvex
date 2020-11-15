#include "mpvex.h"
#include "nif_term_cast.h"

namespace mpvex {

ErlNifResourceType* MpvexType;

} // namespace mpvex

namespace {

class format_not_supported : public std::exception {
public:
    const char* what() const noexcept {
        return "format_not_supported";
    }
};

ERL_NIF_TERM make_error_tuple(ErlNifEnv* env, int error) {
    return enif_make_tuple2(
        env,
        enif_make_atom(env, "error"),
        enif_make_int(env, error)
    );
}

ERL_NIF_TERM make_error_tuple(ErlNifEnv* env, const std::string& error) {
    return enif_make_tuple2(
        env,
        enif_make_atom(env, "error"),
        enif_make_atom(env, error.c_str())
    );
}

ERL_NIF_TERM make_error_tuple(ErlNifEnv* env, const std::exception& exception) {
    return make_error_tuple(env, exception.what());
}

void destruct_mpvex(ErlNifEnv* env, void* obj) {
    mpvex::Mpvex* mpvex = static_cast<mpvex::Mpvex*>(obj);

    mpvex->~Mpvex();
}

int load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info) {
    mpvex::MpvexType = enif_open_resource_type(env, "Mpvex", "Mpvex", destruct_mpvex, ERL_NIF_RT_CREATE, NULL);

    return 0;
}

ERL_NIF_TERM command(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        std::vector<ERL_NIF_TERM> terms = mpvex::nif_term_cast<std::vector<ERL_NIF_TERM>>(env, argv[1]);
        std::vector<std::string> commands(terms.size());
        auto term_to_string = [env](const ERL_NIF_TERM& term) { return mpvex::nif_term_cast<std::string>(env, term); };
        std::transform(terms.begin(), terms.end(), commands.begin(), term_to_string);

        mpvex::Mpvex* mpvex = mpvex::nif_term_cast<mpvex::Mpvex*>(env, argv[0]);
        int status = mpvex->command(commands);

        if (status >= 0) {
            return enif_make_atom(env, "ok");
        } else {
            return make_error_tuple(env, status);
        }
    } catch (const std::exception& e) {
        return make_error_tuple(env, e);
    }
}

ERL_NIF_TERM create(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    void* resource = enif_alloc_resource(mpvex::MpvexType, sizeof(mpvex::Mpvex));
    mpvex::Mpvex* mpvex = new(resource) mpvex::Mpvex;
    ERL_NIF_TERM term = enif_make_resource(env, mpvex);
    enif_release_resource(resource);

    try {
        mpvex->create();

        return enif_make_tuple2(
            env,
            enif_make_atom(env, "ok"),
            term
        );
    } catch (const std::exception& e) {
        return make_error_tuple(env, e);
    }
}

ERL_NIF_TERM error_string(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        int error = mpvex::nif_term_cast<int>(env, argv[0]);
        std::string s = mpvex::Mpvex::error_string(error);
        ErlNifBinary error_string = mpvex::nif_term_cast<ErlNifBinary>(env, s);

        return enif_make_tuple2(
            env,
            enif_make_atom(env, "ok"),
            enif_make_binary(env, &error_string)
        );
    } catch (const std::exception& e) {
        return make_error_tuple(env, e);
    }
}

ERL_NIF_TERM event_name(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        int event_id = mpvex::nif_term_cast<int>(env, argv[0]);
        std::string s = mpvex::Mpvex::event_name(static_cast<mpv_event_id>(event_id));
        ErlNifBinary event_name = mpvex::nif_term_cast<ErlNifBinary>(env, s);

        return enif_make_tuple2(
            env,
            enif_make_atom(env, "ok"),
            enif_make_binary(env, &event_name)
        );
    } catch (const std::exception& e) {
        return make_error_tuple(env, e);
    }
}

ERL_NIF_TERM initialize(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        mpvex::Mpvex* mpvex = mpvex::nif_term_cast<mpvex::Mpvex*>(env, argv[0]);
        int status = mpvex->initialize();

        if (status >= 0) {
            return enif_make_atom(env, "ok");
        } else {
            return make_error_tuple(env, status);
        }
    } catch (const std::exception& e) {
        return make_error_tuple(env, e);
    }
}

ERL_NIF_TERM set_option(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        std::string name = mpvex::nif_term_cast<std::string>(env, argv[1]);
        int format = mpvex::nif_term_cast<int>(env, argv[2]);

        if (format != MPV_FORMAT_FLAG) {
            throw format_not_supported();
        }

        int data = mpvex::nif_term_cast<int>(env, argv[3]);

        mpvex::Mpvex* mpvex = mpvex::nif_term_cast<mpvex::Mpvex*>(env, argv[0]);
        int status = mpvex->set_option(name, static_cast<mpv_format>(format), data);

        if (status >= 0) {
            return enif_make_atom(env, "ok");
        } else {
            return make_error_tuple(env, status);
        }
    } catch (const std::exception& e) {
        return make_error_tuple(env, e);
    }
}

ERL_NIF_TERM set_option_string(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        std::string name = mpvex::nif_term_cast<std::string>(env, argv[1]);
        std::string data = mpvex::nif_term_cast<std::string>(env, argv[2]);

        mpvex::Mpvex* mpvex = mpvex::nif_term_cast<mpvex::Mpvex*>(env, argv[0]);
        int status = mpvex->set_option_string(name, data);

        if (status >= 0) {
            return enif_make_atom(env, "ok");
        } else {
            return make_error_tuple(env, status);
        }
    } catch (const std::exception& e) {
        return make_error_tuple(env, e);
    }
}

ERL_NIF_TERM terminate_destroy(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        mpvex::Mpvex* mpvex = mpvex::nif_term_cast<mpvex::Mpvex*>(env, argv[0]);
        mpvex->terminate_destroy();

        return enif_make_atom(env, "ok");
    } catch (const std::exception& e) {
        return make_error_tuple(env, e);
    }
}

ErlNifFunc nif_funcs[] = {
    {"command", 2, command},
    {"create", 0, create},
    {"error_string", 1, error_string},
    {"event_name", 1, event_name},
    {"initialize", 1, initialize},
    {"set_option", 4, set_option},
    {"set_option_string", 3, set_option_string},
    {"terminate_destroy", 1, terminate_destroy}
};

} // namespace

ERL_NIF_INIT(Elixir.Mpvex.NIF, nif_funcs, load, NULL, NULL, NULL);
