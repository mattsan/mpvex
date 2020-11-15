#ifndef MPVEX_MPVEX_NIF_H_
#define MPVEX_MPVEX_NIF_H_

#include <string>
#include <vector>
#include <stdexcept>

#include <erl_nif.h>

namespace mpvex {

extern ErlNifResourceType* MpvexType;

class badarg : public std::exception {
public:
    const char* what() const noexcept {
        return "badarg";
    }
};

template<class T, class U> T nif_term_cast(ErlNifEnv* env, const U& u);

template<> int nif_term_cast(ErlNifEnv* env, const ERL_NIF_TERM& term) {
    int i;
    if (!enif_get_int(env, term, &i)) {
        throw badarg();
    }
    return i;
}

template<> ErlNifBinary nif_term_cast(ErlNifEnv* env, const ERL_NIF_TERM& term) {
    ErlNifBinary binary;
    if (!enif_inspect_binary(env, term, &binary)) {
        throw badarg();
    }
    return binary;
}

template<> ErlNifBinary nif_term_cast(ErlNifEnv* env, const std::string& s) {
    ErlNifBinary binary;
    if (!enif_alloc_binary(s.size(), &binary)) {
        throw badarg();
    }
    strncpy(reinterpret_cast<char*>(binary.data), s.c_str(), s.size());
    return binary;
}

template<> std::string nif_term_cast(ErlNifEnv* env, const ERL_NIF_TERM& term) {
    ErlNifBinary binary = nif_term_cast<ErlNifBinary>(env, term);
    return std::string(reinterpret_cast<char*>(binary.data), binary.size);
}

template<> std::vector<ERL_NIF_TERM> nif_term_cast(ErlNifEnv* env, const ERL_NIF_TERM& term) {
    unsigned int length;
    if (!enif_get_list_length(env, term, &length)) {
        throw badarg();
    }

    std::vector<ERL_NIF_TERM> terms(length);

    ERL_NIF_TERM list = term;
    for (int i = 0; i < length; ++i) {
        ERL_NIF_TERM head;
        ERL_NIF_TERM tail;
        if (!enif_get_list_cell(env, list, &head, &tail)) {
            throw badarg();
        }

        terms[i] = head;
        list = tail;
    }

    return terms;
}

template<> Mpvex* nif_term_cast(ErlNifEnv* env, const ERL_NIF_TERM& term) {
    Mpvex* mpvex;
    if (!enif_get_resource(env, term, MpvexType, reinterpret_cast<void**>(&mpvex))) {
        throw badarg();
    }
    return mpvex;
}

} // namespace mpvex

#endif//MPVEX_MPVEX_NIF_H_
