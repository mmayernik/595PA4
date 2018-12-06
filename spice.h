
//Specify SPICE Data Structs and constants
#define SPICE_HEADER "* inverter characterization\n.options temp=75\n.tran 0.001n 2.2n 0.0n 0.001n\n.include tuned_45nm_HP.pm\n.include clkinv0.subckt\nvdd vdd 0 1.000"





//Spice Primary Functions
double * simulate_netlist();

int create_spice_files(char * filename);


//Spice Helper Functions


