//
// Created by apbus_amp_k on 26.02.24.
//

#ifndef CGLAUBER_CGLAUBERFACTORY_HH
#define CGLAUBER_CGLAUBERFACTORY_HH

#include "COLA.hh"

class CGlauberFactory final: public cola::VFactory {
public:
    cola::VFilter* create(const std::map<std::string, std::string>&) final;

};

#endif //CGLAUBER_CGLAUBERFACTORY_HH
