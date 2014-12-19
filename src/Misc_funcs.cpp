/*

##############################################################################
#                                                                            #
#              FLUKE: Fields Layered Under Kohn-sham Electrons               #
#                             By: Eric G. Kratz                              #
#                                                                            #
##############################################################################

 Misc. functions that are needed by main(), but call core and wrapper
 functions. This file must be the last file imported into main().

*/

//Misc. functions
void Remove_COM(vector<QMMMAtom>& parts, QMMMSettings& QMMMOpts)
{
  //Places the system at the center of the simulation box
  Coord com = Get_COM(parts,QMMMOpts);
  //Subtract COM for atoms
  #pragma omp parallel for
  for (int i=0;i<Natoms;i++)
  {
    double x = parts[i].x-com.x-0.5*Lx;
    double y = parts[i].y-com.y-0.5*Ly;
    double z = parts[i].z-com.z-0.5*Lz;
    bool check = 1;
    while (check == 1)
    {
      check = 0;
      if (x > Lx)
      {
        x -= Lx;
        check = 1;
      }
      if (x < 0.0)
      {
        x += Lx;
        check = 1;
      }
      if (y > Ly)
      {
        y -= Ly;
        check = 1;
      }
      if (y < 0.0)
      {
        y += Ly;
        check = 1;
      }
      if (z > Lz)
      {
        z -= Lz;
        check = 1;
      }
      if (z < 0.0)
      {
        z += Lz;
        check = 1;
      }
    }
    parts[i].x = x;
    parts[i].y = y;
    parts[i].z = z;
    for (int j=0;j<QMMMOpts.Nbeads;j++)
    {
      x = parts[i].P[j].x-com.x-0.5*Lx;
      y = parts[i].P[j].y-com.y-0.5*Ly;
      z = parts[i].P[j].z-com.z-0.5*Lz;
      check = 1;
      while (check == 1)
      {
        check = 0;
        if (x > Lx)
        {
          x -= Lx;
          check = 1;
        }
        if (x < 0.0)
        {
          x += Lx;
          check = 1;
        }
        if (y > Ly)
        {
          y -= Ly;
          check = 1;
        }
        if (y < 0.0)
        {
          y += Ly;
          check = 1;
        }
        if (z > Lz)
        {
          z -= Lz;
          check = 1;
        }
        if (z < 0.0)
        {
          z += Lz;
          check = 1;
        }
      }
      parts[i].P[j].x = x;
      parts[i].P[j].y = y;
      parts[i].P[j].z = z;
    }
  }
  #pragma omp barrier
  return;
};

void Print_traj(vector<QMMMAtom>& parts, fstream& traj, QMMMSettings& QMMMOpts)
{
  if (RCOM == 1)
  {
    //Removes the center of mass and finds centroids
    Remove_COM(parts,QMMMOpts);
  }
  if (RCOM == 0)
  {
    //Finds centroids when the system is moved to the center of the box
    #pragma omp parallel for
    for (int i=0;i<Natoms;i++)
    {
      Get_Centroid(parts[i],QMMMOpts);
    }
    #pragma omp barrier
  }
  if ((QMMMOpts.PrintMode == "All") or (QMMMOpts.PrintMode == "all"))
  {
    int Ntot = QMMMOpts.Nbeads*Natoms;
    traj << Ntot << '\n' << '\n';
    for (int i=0;i<Natoms;i++)
    {
      for (int j=0;j<QMMMOpts.Nbeads;j++)
      {
        traj << parts[i].MMTyp << " ";
        traj << parts[i].P[j].x << " ";
        traj << parts[i].P[j].y << " ";
        traj << parts[i].P[j].z << '\n';
      }
    }
  }
  if ((QMMMOpts.PrintMode == "COM") or (QMMMOpts.PrintMode == "com"))
  {
    traj << Natoms << '\n' << '\n';
    for (int i=0;i<Natoms;i++)
    {
      traj << parts[i].MMTyp << " ";
      traj << parts[i].x << " ";
      traj << parts[i].y << " ";
      traj << parts[i].z << '\n';
    }
  }
  traj.flush();
  return;
};

void ReadArgs(int& argc, char**& argv, fstream& xyzfile,
     fstream& connectfile, fstream& regionfile, fstream& outfile)
{
  string dummy;
  //Read command line arguments
  if (argc == 1)
  {
    //Escape if there are no arguments
    cout << '\n';
    cout << "Missing arguments..." << '\n' << '\n';
    cout << "Usage: FLUKE -n Ncpus -x Input.xyz -c Connectivity.inp ";
    cout << "-r Regions.inp -o Output.xyz" << '\n';
    cout << '\n' << "  or" << '\n' << '\n';
    cout << "Usage: FLUKE -n Ncpus -x Input.pdb -c Connectivity.inp ";
    cout << "-r Regions.inp -o Output.xyz" << '\n';
    cout << '\n';
    cout << "Use -h or --help for detailed instructions.";
    cout << endl;
    exit(0);
  }
  dummy = string(argv[1]);
  if (dummy == "-convert")
  {
    //Attempt to create FLUKE input from other formats
    dummy = string(argv[2]);
    if (dummy == "-t")
    {
      Tink2FLUKE(argc,argv);
    }
    else
    {
      cout << '\n';
      cout << "Unrecognized file format.";
      cout << '\n';
      cout << endl;
    }
  }
  if (dummy == "-GauExtern")
  {
    //Escape to GauExternal
    ExternalGaussian(argc,argv);
  }
  if ((argc % 2) != 1)
  {
    dummy = string(argv[1]);
    if ((dummy != "-h") and (dummy != "--help"))
    {
      //Escape if there are missing arguments
      cout << '\n';
      cout << "Odd number of arguments..." << '\n' << '\n';
      cout << "Usage: FLUKE -n Ncpus -x Input.xyz -c Connectivity.inp ";
      cout << "-r Regions.inp -o Output.xyz" << '\n';
      cout << '\n' << "  or" << '\n' << '\n';
      cout << "Usage: FLUKE -n Ncpus -x Input.pdb -c Connectivity.inp ";
      cout << "-r Regions.inp -o Output.xyz" << '\n';
      cout << '\n';
      cout << "Use -h or --help for detailed instructions.";
      cout << endl;
      exit(0);
    }
  }
  for (int i=0;i<argc;i++)
  {
    //Read file names and CPUs
    dummy = string(argv[i]);
    if ((dummy == "-h") or (dummy == "--help"))
    {
      //Print helpful information and exit
      cout << '\n';
      cout << "Usage: FLUKE -n Ncpus -x Input.xyz -c Connectivity.inp ";
      cout << "-r Regions.inp -o Output.xyz" << '\n';
      cout << '\n' << "  or" << '\n' << '\n';
      cout << "Usage: FLUKE -n Ncpus -x Input.pdb -c Connectivity.inp ";
      cout << "-r Regions.inp -o Output.xyz" << '\n';
      cout << '\n';
      cout << "Command line arguments:" << '\n' << '\n';
      cout << "  -n    Number of CPUs used for the QM calculation." << '\n' << '\n';
      cout << "  -x    Input pdb or xyz file. The input parser will" << '\n';
      cout << "        detect the file type." << '\n' << '\n';
      cout << "  -c    List of connectivity, atom types, and charges." << '\n' << '\n';
      cout << "  -r    Information about how the system is subdivided" << '\n';
      cout << "        into QM, MM, and psuedo-atom regions." << '\n' << '\n';
      cout << "  -o    Output xyz file for the optimized structures.";
      cout << '\n' << endl;
      exit(0);
    }
    if (dummy == "-n")
    {
      Ncpus = atoi(argv[i+1]);
    }
    if (dummy == "-x")
    {
      xyzfilename = string(argv[i+1]);
      xyzfile.open(argv[i+1],ios_base::in);
    }
    if (dummy == "-c")
    {
      confilename = string(argv[i+1]);
      connectfile.open(argv[i+1],ios_base::in);
    }
    if (dummy == "-r")
    {
      regfilename = string(argv[i+1]);
      regionfile.open(argv[i+1],ios_base::in);
    }
    if (dummy == "-o")
    {
      outfile.open(argv[i+1],ios_base::out);
    }
  }
  for (int i=0;i<argc;i++)
  {
    //Detect bad arguments
    dummy = string(argv[i]);
    if (dummy[0] == '-')
    {
      bool BadArgs = 0; //Bad argument found
      if ((dummy != "-n") and (dummy != "-x") and
      (dummy != "-c") and (dummy != "-r") and
      (dummy != "-o"))
      {
        BadArgs = 1;
      }
      if (BadArgs == 1)
      {
        cout << '\n';
        cout << "Unrecognized flag..." << '\n' << '\n';
        cout << "Usage: FLUKE -n Ncpus -x Input.xyz -c Connectivity.inp ";
        cout << "-r Regions.inp -o Output.xyz" << '\n';
        cout << '\n' << "  or" << '\n' << '\n';
        cout << "Usage: FLUKE -n Ncpus -x Input.pdb -c Connectivity.inp ";
        cout << "-r Regions.inp -o Output.xyz" << '\n';
        cout << '\n';
        cout << "Use -h or --help for detailed instructions.";
        cout << endl;
        exit(0);
      }
    }
  }
  if (argc != 11)
  {
    //Escape if there are too few arguments
    cout << '\n';
    cout << "Missing arguments..." << '\n' << '\n';
    cout << "Usage: FLUKE -n Ncpus -x Input.xyz -c Connectivity.inp ";
    cout << "-r Regions.inp -o Output.xyz" << '\n';
    cout << '\n' << "  or" << '\n' << '\n';
    cout << "Usage: FLUKE -n Ncpus -x Input.pdb -c Connectivity.inp ";
    cout << "-r Regions.inp -o Output.xyz" << '\n';
    cout << '\n';
    cout << "Use -h or --help for detailed instructions.";
    cout << endl;
    exit(0);
  }
  //Make sure input files can be read
  bool DoQuit = 0;
  if (!xyzfile.good())
  {
    cout << "Error: Could not open xyz file.";
    cout << endl;
    DoQuit = 1;
  }
  if (!connectfile.good())
  {
    cout << "Error: Could not open connectivity file.";
    cout << endl;
    DoQuit = 1;
  }
  if (!regionfile.good())
  {
    cout << "Error: Could not open region file.";
    cout << endl;
    DoQuit = 1;
  }
  if (!outfile.good())
  {
    cout << "Error: Could not create output file.";
    cout << endl;
    DoQuit = 1;
  }
  if (DoQuit == 1)
  {
    exit(0);
  }
  return;
};

void ReadFlukeInput(fstream& xyzfile, fstream& connectfile,
     fstream& regionfile, vector<QMMMAtom>& Struct, QMMMSettings& QMMMOpts)
{
  //Read input
  string dummy;
  if (GauExternal == 0)
  {
    xyzfile >> Natoms;
    for (int i=0;i<Natoms;i++)
    {
      //Save atom information
      QMMMAtom tmp;
      xyzfile >> tmp.QMTyp;
      xyzfile >> tmp.x >> tmp.y >> tmp.z;
      tmp.id = i;
      tmp.QMregion = 0;
      tmp.MMregion = 1;
      tmp.PAregion = 0;
      tmp.BAregion = 0;
      tmp.Frozen = 0;
      Struct.push_back(tmp);
    }
  }
  for (int i=0;i<Natoms;i++)
  {
    //Save connectivity information
    int tmp;
    //id MMTyp NumTyp q Nbonds [connectivity]
    connectfile >> tmp; //Atom ID
    if (tmp != Struct[i].id)
    {
      //Escape if connectivity errors are found
      cout << "Error: Atoms in the connectivity file are out of order.";
      cout << endl;
      exit(0); //Escape
    }
    connectfile >> Struct[i].MMTyp >> Struct[i].NumTyp;
    connectfile >> Struct[i].m >> Struct[i].q;
    connectfile >> tmp; //Number of bonds
    for (int j=0;j<tmp;j++)
    {
      //Save each bond to the atom's connectivity list
      int AtomID;
      connectfile >> AtomID;
      if (AtomID >= Natoms)
      {
        //Search for more connectivity errors
        cout << "Error: Atom index out of range in connectivity.";
        cout << '\n';
        cout << "Atom " << i << " bonded to non-existant atom ";
        cout << AtomID << endl;
        exit(0); //Escape
      }
      Struct[i].Bonds.push_back(AtomID); //Add bond
    }
  }
  //Collect misc. simulation options
  regionfile >> dummy >> dummy; //Potential type
  if ((dummy == "QM") or (dummy == "qm"))
  {
    //Read only QM options
    QMonly = 1;
    MMonly = 0;
    QMMM = 0;
    regionfile >> dummy >> dummy;
    if ((dummy == "psi4") or (dummy == "Psi4"))
    {
      Psi4 = 1;
    }
    if ((dummy == "gaussian") or (dummy == "Gaussian"))
    {
      Gaussian = 1;
    }
    regionfile >> dummy >> QMMMOpts.Func;
    regionfile >> dummy >> QMMMOpts.Basis;
    regionfile >> dummy >> QMMMOpts.RAM;
    regionfile >> dummy >> QMMMOpts.Charge;
    regionfile >> dummy >> QMMMOpts.Spin;
    //Place all atoms in the QM region
    for (int i=0;i<Natoms;i++)
    {
      Struct[i].QMregion = 1;
      Struct[i].MMregion = 0;
      Struct[i].PAregion = 0;
      Struct[i].BAregion = 0;
    }
  }
  if ((dummy == "QMMM") or (dummy == "qmmm"))
  {
    //Read QM and MM options
    QMMM = 1;
    QMonly = 0;
    MMonly = 0;
    regionfile >> dummy >> dummy;
    if ((dummy == "psi4") or (dummy == "Psi4"))
    {
      Psi4 = 1;
    }
    if ((dummy == "gaussian") or (dummy == "Gaussian"))
    {
      Gaussian = 1;
    }
    regionfile >> dummy >> QMMMOpts.Func;
    regionfile >> dummy >> QMMMOpts.Basis;
    regionfile >> dummy >> QMMMOpts.RAM;
    regionfile >> dummy >> QMMMOpts.Charge;
    regionfile >> dummy >> QMMMOpts.Spin;
    regionfile >> dummy >> dummy; //MM wrapper
    if ((dummy == "Tinker") or (dummy == "TINKER"))
    {
      Tinker = 1;
    }
    if ((dummy == "Amber") or (dummy == "AMBER"))
    {
      Amber = 1;
    }
  }
  if ((dummy == "MM") or (dummy == "mm"))
  {
    //Read only MM options
    MMonly = 1;
    QMonly = 0;
    QMMM = 0;
    regionfile >> dummy >> dummy; //MM wrapper
    if ((dummy == "Tinker") or (dummy == "TINKER"))
    {
      Tinker = 1;
    }
    if ((dummy == "Amber") or (dummy == "AMBER"))
    {
      Amber = 1;
    }
  }
  regionfile >> dummy >> dummy; //Calculation type
  if ((dummy == "PIMC") or (dummy == "pimc"))
  {
    //Read MC and PIMC options
    OptSim = 0;
    PIMCSim = 1;
    SteepSim = 0;
    SinglePoint = 0;
    regionfile >> dummy >> dummy; //Ensemble
    if ((dummy == "NVT") or (dummy == "nvt"))
    {
      QMMMOpts.Ensemble = "NVT";
    }
    if ((dummy == "NPT") or (dummy == "npt"))
    {
      QMMMOpts.Ensemble = "NPT";
    }
    regionfile >> dummy >> QMMMOpts.Temp;
    QMMMOpts.Beta = 1/(k*QMMMOpts.Temp);
    regionfile >> dummy >> QMMMOpts.Press;
    regionfile >> dummy >> QMMMOpts.Neq;
    regionfile >> dummy >> QMMMOpts.Nsteps;
    regionfile >> dummy >> QMMMOpts.Nbeads;
    regionfile >> dummy >> QMMMOpts.accratio;
    regionfile >> dummy >> QMMMOpts.Nprint;
    regionfile >> dummy >> QMMMOpts.PrintMode;
    for (int i=0;i<Natoms;i++)
    {
      //Create path-integral beads
      for (int j=0;j<QMMMOpts.Nbeads;j++)
      {
        //Pick random displacements
        double randx = (((double)rand())/((double)RAND_MAX));
        double randy = (((double)rand())/((double)RAND_MAX));
        double randz = (((double)rand())/((double)RAND_MAX));
        //Place the first bead at the initial position
        if (j == 0)
        {
          randx = 0.5;
          randy = 0.5;
          randz = 0.5;
        }
        Coord temp; //Bead coordinates
        //Set random bead displacements
        temp.x = Struct[i].x+(randx-0.5)*step;
        temp.y = Struct[i].y+(randy-0.5)*step;
        temp.z = Struct[i].z+(randz-0.5)*step;
        Struct[i].P.push_back(temp);
      }
    }
  }
  if ((dummy == "OPT") or (dummy == "Opt") or
  (dummy == "opt"))
  {
    //Read energy minimization options
    OptSim = 1;
    PIMCSim = 0;
    SteepSim = 0;
    SinglePoint = 0;
    QMMMOpts.Temp = 0;
    QMMMOpts.Beta = 0;
    QMMMOpts.Press = 0;
    QMMMOpts.Neq = 0;
    QMMMOpts.Nsteps = 0;
    QMMMOpts.Nbeads = 0;
    QMMMOpts.accratio = 0;
    QMMMOpts.Nprint = 0;
    QMMMOpts.PrintMode = "COM";
    QMMMOpts.Ensemble = "N/A";
    regionfile >> dummy >> QMMMOpts.QMOptTol;
    regionfile >> dummy >> QMMMOpts.MMOptTol;
    regionfile >> dummy >> QMMMOpts.MaxOptSteps;
  }
  if ((dummy == "Steep") or (dummy == "steep") or
  (dummy == "SD") or (dummy == "sd"))
  {
    //Read energy minimization options
    OptSim = 0;
    PIMCSim = 0;
    SteepSim = 1;
    SinglePoint = 0;
    QMMMOpts.Temp = 0;
    QMMMOpts.Beta = 0;
    QMMMOpts.Press = 0;
    QMMMOpts.Neq = 0;
    QMMMOpts.Nsteps = 0;
    QMMMOpts.Nbeads = 0;
    QMMMOpts.accratio = 0;
    QMMMOpts.Nprint = 0;
    QMMMOpts.PrintMode = "COM";
    QMMMOpts.Ensemble = "N/A";
    regionfile >> dummy >> QMMMOpts.QMOptTol;
    regionfile >> dummy >> QMMMOpts.MMOptTol;
    regionfile >> dummy >> QMMMOpts.MaxOptSteps;
  }
  if ((dummy == "SP") or (dummy == "sp") or
  (dummy == "energy") or (dummy == "Energy"))
  {
    //Read energy minimization options
    OptSim = 0;
    PIMCSim = 0;
    SteepSim = 0;
    SinglePoint = 1;
    QMMMOpts.Temp = 0;
    QMMMOpts.Beta = 0;
    QMMMOpts.Press = 0;
    QMMMOpts.Neq = 0;
    QMMMOpts.Nsteps = 0;
    QMMMOpts.Nbeads = 0;
    QMMMOpts.accratio = 0;
    QMMMOpts.Nprint = 0;
    QMMMOpts.PrintMode = "COM";
    QMMMOpts.Ensemble = "N/A";
  }
  regionfile >> dummy >> dummy; //PBC options
  if ((dummy == "Yes") or (dummy == "yes") or
  (dummy == "YES") or (dummy == "true") or
  (dummy == "True") or (dummy == "TRUE"))
  {
    //Read box sizes
    PBCon = 1;
    regionfile >> dummy;
    regionfile >> Lx >> Ly >> Lz;
  }
  regionfile >> dummy >> Nqm; //Number of QM atoms
  for (int i=0;i<Nqm;i++)
  {
    int AtomID;
    regionfile >> AtomID;
    Struct[AtomID].QMregion = 1;
    Struct[AtomID].MMregion = 0;
  }
  regionfile >> dummy >> Npseudo; //Number of pseudo-atoms
  for (int i=0;i<Npseudo;i++)
  {
    int AtomID;
    regionfile >> AtomID;
    Struct[AtomID].PAregion = 1;
    Struct[AtomID].MMregion = 0;
  }
  regionfile >> dummy >> Nbound; //Number of boundary-atoms
  for (int i=0;i<Nbound;i++)
  {
    int AtomID;
    regionfile >> AtomID;
    Struct[AtomID].BAregion = 1;
    Struct[AtomID].MMregion = 0;
  }
  Nmm = Natoms-Nqm-Npseudo-Nbound; //Number of MM atoms
  regionfile >> dummy >> Nfreeze; //Number of frozen atoms
  for (int i=0;i<Nfreeze;i++)
  {
    int AtomID;
    regionfile >> AtomID;
    Struct[AtomID].Frozen = 1;
    if (PIMCSim == 1)
    {
      //Frozen atoms must be purely classical
      for (int j=0;j<QMMMOpts.Nbeads;j++)
      {
        Struct[AtomID].P[j].x = Struct[AtomID].x;
        Struct[AtomID].P[j].y = Struct[AtomID].y;
        Struct[AtomID].P[j].z = Struct[AtomID].z;
      }
    }
  }
  //Collect additonal tinker input
  if (Tinker == 1)
  {
    FindTinkerClasses(Struct);
  }
  return;
};

void FlukeErrorChecker(QMMMSettings& QMMMOpts)
{
  //Checks for basic errors and conflicts
  bool DoQuit = 0; //Bool, quit with error
  if (((Tinker+Amber) != 1) and (QMonly != 1))
  {
    //Check the MM wrappers
    cout << " Error: No valid MM wrapper selected.";
    cout << '\n';
    cout << "  Select a wrapper if you want to run this type ";
    cout << "of calculation.";
    cout << '\n';
    DoQuit = 1;
  }
  if (((Psi4+Gaussian) != 1) and (MMonly != 1))
  {
    //Check the QM wrappers
    cout << " Error: No valid QM wrapper selected.";
    cout << '\n';
    cout << "  Select a wrapper if you want to run this type ";
    cout << "of calculation.";
    cout << '\n';
    DoQuit = 1;
  }
  if ((QMMMOpts.Ensemble == "NPT") and (PBCon == 0))
  {
    //Check the PBC options
    cout << " Error: NPT simulation without PBC.";
    cout << '\n';
    cout << "  Turn PBC on if you want to run this type ";
    cout << "of calculation.";
    cout << '\n';
    DoQuit = 1;
  }
  if (Ncpus < 1)
  {
    //Checks the number of threads and continue
    cout << " Warning: Calculations cannot run with ";
    cout << Ncpus << " CPUs.";
    cout << '\n';
    if (Jokes == 1)
    {
      cout << " Do you know how computers work?";
    }
    cout << " Ncpus set to 1";
    cout << '\n';
    Ncpus = 1;
  }
  if ((Psi4 == 1) and (QMMM == 1))
  {
    if (OptSim == 1)
    {
      cout << " Error: Psi4 can only be used in pure QM calculations";
      cout << '\n';
      cout << " or QMMM PIMC calculations.";
      cout << '\n';
      DoQuit = 1;
    }
    if ((Npseudo != 0) or (Nbound != 0))
    {
      cout << " Error: The psi4 wrapper can only use QM and MM atoms.";
      cout << '\n';
      cout << " Remove the pseudo-atoms and boundary-atoms.";
      cout << '\n';
      DoQuit = 1;
    }
  }
  if (DoQuit == 1)
  {
    //Quits
    cout << endl;
    exit(0);
  }
  if (DoQuit == 0)
  {
    //Sarcastically continues
    cout << "No fatal errors detected.";
    cout << '\n';
    if (Jokes == 1)
    {
      cout << " And there was much rejoicing. Yay...";
      cout << '\n';
      cout << endl;
    }
  }
  return;
};

void FlukePrintSettings(QMMMSettings& QMMMOpts)
{
  //Prints out the simulation details
  cout << "Setting up simulation..." << '\n';
  cout << '\n';
  cout << "Atoms: " << Natoms << '\n';
  if (QMMM == 1)
  {
    cout << " QM atoms: " << Nqm << '\n';
    cout << " MM atoms: " << Nmm << '\n';
    cout << " Pseudo-atoms: " << Npseudo << '\n';
    cout << " Boundary-atoms: " << Nbound << '\n';
  }
  if (Nfreeze > 0)
  {
    cout << " Frozen atoms: " << Nfreeze << '\n';
  }
  if (PIMCSim == 1)
  {
    //Print input for error checking
    if (QMMMOpts.Nbeads > 1)
    {
      cout << " PI Beads: " << QMMMOpts.Nbeads << '\n';
    }
    cout << '\n';
    cout << "Equilibration steps: " << QMMMOpts.Neq << '\n';
    cout << "Steps for production run: " << QMMMOpts.Nsteps << '\n';
    cout << "Simulation mode: ";
    if (QMMM == 1)
    {
      cout << "QMMM";
    }
    if (QMonly == 1)
    {
      cout << "Pure QM";
    }
    if (MMonly == 1)
    {
      cout << "Pure MM";
    }
    cout << " " << QMMMOpts.Ensemble;
    if (QMMMOpts.Nbeads > 1)
    {
      cout << " path-integral";
    }
    cout << " Monte Carlo" << '\n' << '\n';
  }
  if (OptSim == 1)
  {
    cout << '\n';
    cout << "Simulation mode: ";
    if (QMMM == 1)
    {
      cout << "QMMM";
    }
    if (QMonly == 1)
    {
      cout << "Pure QM";
    }
    if (MMonly == 1)
    {
      cout << "Pure MM";
    }
    cout << " energy minimization" << '\n';
    cout << '\n';
  }
  if (SinglePoint == 1)
  {
    cout << '\n';
    cout << "Simulation mode: ";
    if (QMMM == 1)
    {
      cout << "QMMM";
    }
    if (QMonly == 1)
    {
      cout << "Pure QM";
    }
    if (MMonly == 1)
    {
      cout << "Pure MM";
    }
    cout << " single-point energy" << '\n';
    cout << '\n';
  }
  cout.flush(); //Flush for output being redirected to a file
  return;
};

void GetQuotes(vector<string>& Quotes)
{
  //Generate random quotes
  string dummy;
  dummy = "\'It is difficult to prove that this quote is not random.\'";
  dummy += '\n';
  dummy += "                                           -Eric G. Kratz";
  dummy += '\n';
  for (int i=0;i<1000;i++)
  {
    //Add quotes to the list
    Quotes.push_back(dummy);
  }
  return;
};
