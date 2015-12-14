/*

###############################################################################
#                                                                             #
#                 LICHEM: Layered Interacting CHEmical Models                 #
#                              By: Eric G. Kratz                              #
#                                                                             #
#                      Symbiotic Computational Chemistry                      #
#                                                                             #
###############################################################################

 Functions for the data structures used in LICHEM.

*/

//Coord class function definitions
Coord::Coord()
{
  //Constructor
  x = 0;
  y = 0;
  z = 0;
  return;
};

Coord::~Coord()
{
  //Generic destructor
  return;
};

double Coord::VecMag()
{
  //Return the squared vector magnitude
  double R2; //Squared distance
  R2 = x*x+y*y+z*z; //Calculate using local variables
  //Return value
  return R2;
};

//Mpole class function definitions
Mpole::Mpole()
{
  //Generic constructor
  return;
};

Mpole::~Mpole()
{
  //Generic destructor
  return;
};

//RedMpole class function definitions
RedMpole::RedMpole()
{
  //Generic constructor
  return;
};

RedMpole::~RedMpole()
{
  //Generic destructor
  return;
};

//OctCharges class function definitions
OctCharges::OctCharges()
{
  //Generic constructor
  return;
};

OctCharges::~OctCharges()
{
  //Generic destructor
  return;
};

//GEMDen class function definitions
GEMDen::GEMDen()
{
  //Generic constructor
  return;
};

GEMDen::GEMDen(string Typ, string BasName)
{
  //Fancy constructor
  SetBasis(Typ,BasName);
  return;
};

GEMDen::~GEMDen()
{
  //Generic destructor
  return;
};

void GEMDen::SetBasis(string Typ, string BasName)
{
  //Set the basis set
  Dens = HermBasis(Typ,BasName);
  return;
};

void GEMDen::SetFrame(bool flip, string frame, int at1, int at2, int at3)
{
  //Set the local frame of reference
  ChiralFlip = flip;
  Type = frame;
  Atom1 = at1;
  Atom2 = at2;
  Atom3 = at3;
  return;
};

Mpole GEMDen::GEMDM()
{
  //Function to convert GEM density to distributed multipoles
  Mpole dmpole; //Blank set of multipoles
  //Save frame of reference
  dmpole.ChiralFlip = ChiralFlip;
  dmpole.Type = Type;
  dmpole.Atom1 = Atom1;
  dmpole.Atom2 = Atom2;
  dmpole.Atom3 = Atom3;
  //Initialize multipoles
  dmpole.q = 0;
  dmpole.Dx = 0;
  dmpole.Dy = 0;
  dmpole.Dz = 0;
  dmpole.IDx = 0;
  dmpole.IDy = 0;
  dmpole.IDz = 0;
  dmpole.Qxx = 0;
  dmpole.Qxy = 0;
  dmpole.Qxz = 0;
  dmpole.Qyy = 0;
  dmpole.Qxz = 0;
  dmpole.Qzz = 0;
  //Add the nuclear charge from the periodic table
  dmpole.q += PTable.RevTyping(Type);
  //Convert Hermite Gaussians to multipoles
  for (unsigned int i=0;i<Dens.size();i++)
  {
    //Check for a monopole
    if ((Dens[i].XPow() == 0) and (Dens[i].YPow() == 0) and
       (Dens[i].ZPow() == 0))
    {
      //Update monopole
      dmpole.q += Dens[i].Coeff();
    }
    //Check for a dipole
    if ((Dens[i].XPow() == 1) and (Dens[i].YPow() == 0) and
       (Dens[i].ZPow() == 0))
    {
      //Update x dipole
      dmpole.Dx += Dens[i].Coeff();
    }
    if ((Dens[i].XPow() == 0) and (Dens[i].YPow() == 1) and
       (Dens[i].ZPow() == 0))
    {
      //Update y dipole
      dmpole.Dy += Dens[i].Coeff();
    }
    if ((Dens[i].XPow() == 0) and (Dens[i].YPow() == 0) and
       (Dens[i].ZPow() == 1))
    {
      //Update z dipole
      dmpole.Dz += Dens[i].Coeff();
    }
    //Check for a quadrupole
    if ((Dens[i].XPow() == 2) and (Dens[i].YPow() == 0) and
       (Dens[i].ZPow() == 0))
    {
      //Update xx quadrupole
      dmpole.Qxx += Dens[i].Coeff();
    }
    if ((Dens[i].XPow() == 1) and (Dens[i].YPow() == 1) and
       (Dens[i].ZPow() == 0))
    {
      //Update xy quadrupole
      dmpole.Qxy += Dens[i].Coeff();
    }
    if ((Dens[i].XPow() == 1) and (Dens[i].YPow() == 0) and
       (Dens[i].ZPow() == 1))
    {
      //Update xz quadrupole
      dmpole.Qxz += Dens[i].Coeff();
    }
    if ((Dens[i].XPow() == 0) and (Dens[i].YPow() == 2) and
       (Dens[i].ZPow() == 0))
    {
      //Update xx quadrupole
      dmpole.Qyy += Dens[i].Coeff();
    }
    if ((Dens[i].XPow() == 0) and (Dens[i].YPow() == 1) and
       (Dens[i].ZPow() == 1))
    {
      //Update yz quadrupole
      dmpole.Qyz += Dens[i].Coeff();
    }
    if ((Dens[i].XPow() == 0) and (Dens[i].YPow() == 0) and
       (Dens[i].ZPow() == 2))
    {
      //Update yz quadrupole
      dmpole.Qzz += Dens[i].Coeff();
    }
  }
  return dmpole;
};

//QMMMAtom class function definitions
QMMMAtom::QMMMAtom()
{
  //Generic constructor
  return;
};

QMMMAtom::~QMMMAtom()
{
  //Generic destructor
  return;
};

//QMMMElec class function definitions
QMMMElec::QMMMElec()
{
  //Generic constructor
  return;
};

QMMMElec::~QMMMElec()
{
  //Generic destructor
  return;
};

//QMMMSettings class function definitions
QMMMSettings::QMMMSettings()
{
  //Generic constructor
  return;
};

QMMMSettings::~QMMMSettings()
{
  //Generic destructor
  return;
};

//PeriodicTable class function definitions
PeriodicTable::PeriodicTable()
{
  //Set atomic properties
  //NB: The widths were calculated for the all-electron density
  Typs.push_back("H"); //Hydrogen, 1
  GauWids.push_back(0.33046422838582712); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("He"); //Helium, 2
  GauWids.push_back(0.44937774826454929); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Li"); //Lithium, 3
  GauWids.push_back(0.24891493951151511); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Be"); //Berylium, 4
  GauWids.push_back(0.28393617417828898); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("B"); //Boron, 5
  GauWids.push_back(0.31688766703304322); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("C"); //Carbon, 6
  GauWids.push_back(0.34748552120700499); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("N"); //Nitrogen, 7
  GauWids.push_back(0.38230385698895192); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("O"); //Oxygen, 8
  GauWids.push_back(0.40455289847810627); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("F"); //Fluorine, 9
  GauWids.push_back(0.43227871563688025); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ne"); //Neon, 10
  GauWids.push_back(0.45940792416326015); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Na"); //Sodium, 11
  GauWids.push_back(0.28603607723272795); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Mg"); //Magnesium, 12
  GauWids.push_back(0.29359655766949572); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Al"); //Aluminum, 13
  GauWids.push_back(0.30283227512360922); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Si"); //Silicon, 14
  GauWids.push_back(0.31462038964458622); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("P"); //Phosphorus, 15
  GauWids.push_back(0.33301416291877084); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("S"); //Sulfur, 16
  GauWids.push_back(0.35056486830985728); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Cl"); //Chlorine, 17
  GauWids.push_back(0.37229368179032268); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ar"); //Argon, 18
  GauWids.push_back(0.39498740756144213); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("K"); //Potassium, 19
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ca"); //Calcium, 20
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Sc"); //Scandium, 21
  GauWids.push_back(0.29098058180517811); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ti"); //Titanium, 22
  GauWids.push_back(0.29703979129006947); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("V"); //Vanadium, 23
  GauWids.push_back(0.30365511638085596); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Cr"); //Chromium, 24
  GauWids.push_back(0.30996489546441913); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Mn"); //Manganese, 25
  GauWids.push_back(0.32461806746013033); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Fe"); //Iron, 26
  GauWids.push_back(0.33074715374383107); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Co"); //Cobalt, 27
  GauWids.push_back(0.33482470199473979); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ni"); //Nickel, 28
  GauWids.push_back(0.33983030212559556); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Cu"); //Copper, 29
  GauWids.push_back(0.34432982070592233); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Zn"); //Zinc, 30
  GauWids.push_back(0.34411081636918234); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ga"); //Gallium, 31
  GauWids.push_back(0.33702898576385676); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ge"); //Germanium, 32
  GauWids.push_back(0.33935669473845503); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("As"); //Arsenic, 33
  GauWids.push_back(0.34818738929821641); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Se"); //Selenium, 34
  GauWids.push_back(0.35888462095130624); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Br"); //Bromine, 35
  GauWids.push_back(0.37243540751493021); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Kr"); //Krypton, 36
  GauWids.push_back(0.3880309254592651); //Method: PBE0/aug-cc-pVQZ
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Rb"); //Rubidium, 37
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Sr"); //Strontium, 38
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Y"); //Yttrium, 39
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Zr"); //Zirconium, 40
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Nb"); //Niobium, 41
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Mo"); //Molybdenum, 42
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Tc"); //Technetium, 43
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ru"); //Ruthenium, 44
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Rh"); //Rhodium, 45
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Pd"); //Palladium, 46
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ag"); //Silver, 47
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Cd"); //Cadmium, 48
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("In"); //Indium, 49
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Sn"); //Tin, 50
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Sb"); //Antimony, 51
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Te"); //Tellurium, 52
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("I"); //Iodine, 53
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Xe"); //Xenon, 54
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Cs"); //Caesium, 55
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ba"); //Barium, 56
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("La"); //Lanthanum, 57
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ce"); //Cerium, 58
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Pr"); //Praseodymium, 59
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Nd"); //Neodymium, 60
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Pm"); //Promethium, 61
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Sm"); //Samarium, 62
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Eu"); //Europium, 63
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Gd"); //Gadolinium, 64
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Tb"); //Terbium, 65
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Dy"); //Dysprosium, 66
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ho"); //Holmium, 67
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Er"); //Erbium, 68
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Tm"); //Thulium, 69
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Yb"); //Ytterbium, 70
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Lu"); //Lutetium, 71
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Hf"); //Hafnium, 72
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ta"); //Tantalum, 73
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("W"); //Tungsten, 74
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Re"); //Rhenium, 75
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Os"); //Osmium, 76
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ir"); //Iridium, 77
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Pt"); //Platinum, 78
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Au"); //Gold, 79
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Hg"); //Mercury, 80
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Tl"); //Thallium, 81
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Pb"); //Lead, 82
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Bi"); //Bismuth, 83
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Po"); //Polonium, 84
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("At"); //Astatine, 85
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Rn"); //Radon, 86
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Fr"); //Francium, 87
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ra"); //Radium, 88
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ac"); //Actinium, 89
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Th"); //Thorium, 90
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Pa"); //Protactium, 91
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("U"); //Uranium, 92
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Np"); //Neptunium, 93
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Pu"); //Plutonium, 94
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Am"); //Americium, 95
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Cm"); //Curium, 96
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Bk"); //Berkelium, 97
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Cf"); //Californium, 98
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Es"); //Einsteinium, 99
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Fm"); //Fermium, 100
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Md"); //Mendelevium, 101
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("No"); //Nobelium, 102
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Lr"); //Lawrencium, 103
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Rf"); //Rutherfordium, 104
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Db"); //Dubnium, 105
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Sg"); //Seaborgium, 106
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Bh"); //Bohrium, 107
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Hs"); //Hasium, 108
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Mt"); //Meitnerium, 109
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Ds"); //Darmstadtium, 110
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Rg"); //Roentgenium, 111
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Cn"); //Copernicium, 112
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back(""); //Num. 113
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Fl"); //Flerovium, 114
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back(""); //Num. 115
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back("Lv"); //Livermorium, 116
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back(""); //Num. 117
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  Typs.push_back(""); //Num. 118
  GauWids.push_back(0.3); //Default value
  CovRadii.push_back(1.0); //Default value
  return;
};

PeriodicTable::~PeriodicTable()
{
  //Generic destructor
  return;
};

string PeriodicTable::Typing(int Z)
{
  //Function to convert nuclear charges to atom types
  return Typs[Z-1];
};

int PeriodicTable::RevTyping(string AtName)
{
  //Function to convert atom types to nuclear charges
  int Z = 0;
  #pragma omp parallel for schedule(dynamic) num_threads(Ncpus)
  for (unsigned int i=0;i<Typs.size();i++)
  {
    if (AtName == Typs[i])
    {
      Z = i+1;
    }
  }
  return Z;
};

double PeriodicTable::GetGauWid(string AtName)
{
  //Function to find the 1s Gaussian width of an atom
  double gauwid = 0;
  #pragma omp parallel for schedule(dynamic) num_threads(Ncpus)
  for (unsigned int i=0;i<Typs.size();i++)
  {
    if (AtName == Typs[i])
    {
      //Save value
      gauwid = GauWids[i];
    }
  }
  return gauwid;
};

double PeriodicTable::GetCovRadius(string AtName)
{
  //Function to find the 1s Gaussian width of an atom
  double radius = 0;
  #pragma omp parallel for schedule(dynamic) num_threads(Ncpus)
  for (unsigned int i=0;i<Typs.size();i++)
  {
    if (AtName == Typs[i])
    {
      //Save value
      radius = CovRadii[i];
    }
  }
  return radius;
};
