#pragma once

#include <vector>
#include <functional>
#include <memory>

#include "compute_function.h"

namespace NComputeFunctions {

template <int64_t derivative>
struct TNormal;

template <>
struct TNormal<0> {
    static TSpirvProgrammConstRef Get() {
        const static TSpirvProgramm programm = {};
        return programm;
    }
};

template <>
struct TNormal<1> {
    static TSpirvProgrammConstRef Get() {
        const static TSpirvProgramm programm = {};
        return programm;
    }
};

template <>
struct TNormal<2> {
    static TSpirvProgrammConstRef Get() {
        const static TSpirvProgramm programm = {};
        return programm;
    }
};

template <int64_t derivative>
struct TMaxwellBoltzmann;

template <>
struct TMaxwellBoltzmann<0> {
    static TSpirvProgrammConstRef Get() {
        const static TSpirvProgramm programm = {};
        return programm;
    }
};

template <>
struct TMaxwellBoltzmann<1> {
    static TSpirvProgrammConstRef Get() {
        const static TSpirvProgramm programm = {};
        return programm;
    }
};

template <int64_t derivative>
struct TRayleigh;
template <>
struct TRayleigh<0> {
    static TSpirvProgrammConstRef Get() {
        const static TSpirvProgramm programm = {};
        return programm;
    }
};

template <>
struct TRayleigh<1> {
    static TSpirvProgrammConstRef Get() {
        const static TSpirvProgramm programm = {};
        return programm;
    }
};
}  // namespace NComputeFunctions
