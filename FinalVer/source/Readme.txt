..............FinalVer...............

This program is using a radioactive source (Ra226 by default) with dimensions
of 2X2 cm^2, located 2 cm from the quartz fiber's centr of mass to induce cherenkov
radiation in a quartz fiber with radius of 0.075cm and height of 7cm. At z=5cm 
(1.5 cm from the tip of the fiber) There is a PMT which count the photons coming
out from the rod, the PMT is a circle with radius 8 mm. The user can change the PMT's
location via UI command : /detector/PMT x. x is the location of the PMT along the z axis.

Another new feature here is the Cone option, the user can use the UI command:
/detector/Cone x, x can take the values of true of false to attach or unattach the cone on
the top of the fiber.

Lastly, the user can define the name of the root file that will be created at the end of the run
by the command /filename/name x , x is the name of the file. 
** currently there is a small bug, to overcome it just make sure before any sessions of runs that you
write the command /run/beamOn 1 , without mention the file name.**

//construction.cc
This file include all the geometry and materials of the program, the information about
the sensitivity of the PMT. The user define there what materials will be sensitive and what not.
For example, one time I was intrested in counting how many photons produced in the rod
from the radioactive source, so I made the quartz fiber as a sensitive to photons.

//detector.cc
This file contains all the information that the user is intresting at about the photons,
the defualt is that the PMT is get the energy of each photons hit it but it has many more options.
There was one time I was calculated the angular distribution of the photons that coming out from the rod,
I edited the file to get the 4 momentum of each photons and than calculated the angular distribution.

//generator.cc
 Here is the particlegun information, you can define here how many particles will be generated in each run,
 The location of the particle (our radioactive source).
 
//run.cc
Here there is the information about what files to be generated, I chose to create Histograms but that can
be changed by the user.

//physics.cc
Here are all the relevant physics list.

//action.cc
Importing the function which runs the particle gun.  