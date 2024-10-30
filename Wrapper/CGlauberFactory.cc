//
// Created by apbus_amp_k on 26.02.24.
//

#include "CGlauberFactory.hh"

#include <memory>

#include "CGlauberGenerator.hh"

cola::VFilter *CGlauberFactory::create(const std::map<std::string, std::string>& paramMap) {
    auto fMomStr = paramMap.at("fermiMom");
    std::unique_ptr<FermiMomentum> fermiMomentum;

    if (fMomStr == "Goldhaber" or fMomStr == "1")
        fermiMomentum = std::make_unique<GoldhaberMomentum>();
    else if (fMomStr == "Morrisey" or fMomStr == "2")
        fermiMomentum = std::make_unique<MorriseyMomentum>();
    else if (fMomStr == "VanBiber" or fMomStr == "3")
        fermiMomentum = std::make_unique<VanBiberMomentum>();
    else
        throw std::runtime_error("This Fermi momentum option is unrecognized! Available options are 1-3, seek documentation for more information");

    return new CGlauberGenerator(paramMap.at("name_a"), paramMap.at("name_b"), std::stod(paramMap.at("energy")),
                                 std::stoi(paramMap.at("is_collider")), std::move(fermiMomentum));
}