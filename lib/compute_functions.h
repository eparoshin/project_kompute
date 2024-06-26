#pragma once

#include "MaxvellBoltzmannDerivative0.comp.spv.h"
#include "MaxvellBoltzmannDerivative1.comp.spv.h"
#include "RayleighDerivative0.comp.spv.h"
#include "RayleighDerivative1.comp.spv.h"
#include "shaders/NormalDerivative0.comp.spv.h"
#include "shaders/NormalDerivative1.comp.spv.h"
#include "shaders/NormalDerivative2.comp.spv.h"

#include "compute_function.h"

namespace NSApplication {
namespace NSCompute {

template <int64_t derivative>
struct CNormal;

template <>
struct CNormal<0> {
    static const CPlotComputeFunction::CShaderCode& Get() {
        return NSNormalDerivative0::SpirvRawData;
    }
};

template <>
struct CNormal<1> {
    static const CPlotComputeFunction::CShaderCode& Get() {
        return NSNormalDerivative1::SpirvRawData;
    }
};

template <>
struct CNormal<2> {
    static const CPlotComputeFunction::CShaderCode& Get() {
        return NSNormalDerivative2::SpirvRawData;
    }
};

template <int64_t derivative>
struct CMaxwellBoltzmann;

template <>
struct CMaxwellBoltzmann<0> {
    static const CPlotComputeFunction::CShaderCode& Get() {
        return NSMaxvellBoltzmannDerivative0::SpirvRawData;
    }
};

template <>
struct CMaxwellBoltzmann<1> {
    static const CPlotComputeFunction::CShaderCode& Get() {
        return NSMaxvellBoltzmannDerivative1::SpirvRawData;
    }
};

template <int64_t derivative>
struct CRayleigh;
template <>
struct CRayleigh<0> {
    static const CPlotComputeFunction::CShaderCode& Get() {
        return NSRayleighDerivative0::SpirvRawData;
    }
};

template <>
struct CRayleigh<1> {
    static const CPlotComputeFunction::CShaderCode& Get() {
        return NSRayleighDerivative1::SpirvRawData;
    }
};
}  // namespace NSCompute
}  // namespace NSApplication
