...............optimalSource...............

This program demonstrates a very simple application where an cherenkov
 photons created in a quartz fiber as a result from radioactive beta decay.
 The user can choose what kind of radioactive source to be simulated by running 
 the relevant macro. The options are: 
 - Sr90.mac
 - Cs137.mac
 - Ra226.mac
 
 The user can create/modify his own macros for his relevant sources.
 
 Each macro performs 4 runs, each of 10^3, 10^4, 10^5, 10^6 decays respectively.
 Each macro creates 4 roots files named "output_(#run number)" which shows the cherenkov energy distribution 
 as a function of energy, (output_0.root is for the first run,
 output_1.root is for the second and so on..). This means that if the user want to 
 save the root files, he has to change their names before running the other macros,
 otherwise the files will be overwritten.
 
 //GEOMETRY DEFINITION
 
 The geometry is constructed in the construction.cc file.
   The setup consists a world of box shape filled with air. Two
   volumes is placed inside it: a cylindrical fiber filled with quartz and a PMT with circular shape.
   
 The quartz fiber dimensions is 7 cm for height and a radius of 0.075 cm.
 The PMT has radius of 4 mm with negligible thickness and it is sensitive to photons with energy
 of 1 to 4.5 eV.
 The radioactive source has a dimensions of 2*2 cm^2, placed
 parallel to the fiber's axis of symmetry at a distance of 2 cm from it's
 center of mass. The PMT is placed 1.5 cm from the top of the fiber.
 
 //PMT
 
 The user can edit the information he intrested to get from the PMT in the detector.cc file
 under the ProcessHits function. To change the energy sensitivity of the PMT, check the file 
 construction.cc , under the ConstructSDandField function.
   
