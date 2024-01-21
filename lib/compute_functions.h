#pragma once

#include <functional>
#include <memory>
#include <vector>
#include "shaders/NormalDerivative0.comp.spv.hpp"
#include "shaders/NormalDerivative1.comp.spv.hpp"
#include "shaders/NormalDerivative2.comp.spv.hpp"
#include "MaxvellBoltzmannDerivative0.comp.spv.hpp"
#include "MaxvellBoltzmannDerivative1.comp.spv.hpp"
#include "RayleighDerivative0.comp.spv.hpp"
#include "RayleighDerivative1.comp.spv.hpp"

#include "compute_function.h"

namespace NComputeFunctions {

template <int64_t derivative>
struct TNormal;

template <>
struct TNormal<0> {
    static TSpirvProgrammConstRef Get() {
        return NNormalDerivative0::SpirvRawData;
    }
};

template <>
struct TNormal<1> {
    static TSpirvProgrammConstRef Get() {
        return NNormalDerivative1::SpirvRawData;
    }
};

template <>
struct TNormal<2> {
    static TSpirvProgrammConstRef Get() {
        return NNormalDerivative2::SpirvRawData;
    }
};

template <int64_t derivative>
struct TMaxwellBoltzmann;

template <>
struct TMaxwellBoltzmann<0> {
    static TSpirvProgrammConstRef Get() {
        return NMaxvellBoltzmannDerivative0::SpirvRawData;
    }
};

template <>
struct TMaxwellBoltzmann<1> {
    static TSpirvProgrammConstRef Get() {
        return NMaxvellBoltzmannDerivative1::SpirvRawData;
    }
};

template <int64_t derivative>
struct TRayleigh;
template <>
struct TRayleigh<0> {
    static TSpirvProgrammConstRef Get() {
        return NRayleighDerivative0::SpirvRawData;
    }
};

template <>
struct TRayleigh<1> {
    static TSpirvProgrammConstRef Get() {
        return NRayleighDerivative1::SpirvRawData;
    }
};
}  // namespace NComputeFunctions
