#pragma once

#include "shaders/NormalDerivative0.comp.spv.hpp"
#include "shaders/NormalDerivative1.comp.spv.hpp"
#include "shaders/NormalDerivative2.comp.spv.hpp"
#include "MaxvellBoltzmannDerivative0.comp.spv.hpp"
#include "MaxvellBoltzmannDerivative1.comp.spv.hpp"
#include "RayleighDerivative0.comp.spv.hpp"
#include "RayleighDerivative1.comp.spv.hpp"

#include "compute_function.h"

namespace NSComputeFunctions {

template <int64_t derivative>
struct CNormal;

template <>
struct CNormal<0> {
    static CSpirvProgrammConstRef Get() {
        return NSNormalDerivative0::SpirvRawData;
    }
};

template <>
struct CNormal<1> {
    static CSpirvProgrammConstRef Get() {
        return NSNormalDerivative1::SpirvRawData;
    }
};

template <>
struct CNormal<2> {
    static CSpirvProgrammConstRef Get() {
        return NSNormalDerivative2::SpirvRawData;
    }
};

template <int64_t derivative>
struct CMaxwellBoltzmann;

template <>
struct CMaxwellBoltzmann<0> {
    static CSpirvProgrammConstRef Get() {
        return NSMaxvellBoltzmannDerivative0::SpirvRawData;
    }
};

template <>
struct CMaxwellBoltzmann<1> {
    static CSpirvProgrammConstRef Get() {
        return NSMaxvellBoltzmannDerivative1::SpirvRawData;
    }
};

template <int64_t derivative>
struct CRayleigh;
template <>
struct CRayleigh<0> {
    static CSpirvProgrammConstRef Get() {
        return NSRayleighDerivative0::SpirvRawData;
    }
};

template <>
struct CRayleigh<1> {
    static CSpirvProgrammConstRef Get() {
        return NSRayleighDerivative1::SpirvRawData;
    }
};
}  // namespace NSComputeFunctions
