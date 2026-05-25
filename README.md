# CGlauber
## COLA module wrap around TGlauberMC model

Configurable parameters:
* `is_collider` - 1 or 0 for either collider or fixed target geometry. Default: collider.
* `fermiMom` - Fermi momentum 
* `energy` - collision energy, either sqrt(s_nn) for collider geometry or A GeV for fixed target. Defaults to 5.02e3 
* `name_a` and `name_b` - Names of projectile and target nuclei. Both default to Pb, consult `TGlauNucleus.cc` to see all available options (Compared to TGlauberMC, additional O, Ne and Pb configurations from AAMCC are available) 
* `fermiMom` - Intranuclear momentum model. To simulate momenta after collision, additional model is needed. Available options: Goldhaber, Morissey or VanBiber. Options are also coded to 1,2 and 3, respectively. Default: Goldhaber.