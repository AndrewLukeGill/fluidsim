
system partSys dim 100
system partSys particle 0 7 0 3 0 5 0 2 
system partSys particle 1 7 -1 3 0 9 0 0 
system partSys particle 2 7 -2 3 0 8 0 4 
system partSys particle 3 7 -3 3 0 7 2 0 
system partSys particle 4 7 0 3 1 6 0 2 
system partSys particle 5 7 0.1 3 0 5 0 -1 
system partSys particle 6 7 -0.1 3 0 4 0 0 
system partSys particle 8 7 3 3 3 -1 0 -1 
system partSys particle 9 7 -3 3 0 0 0 0 
system partSys particle 10 7 -2 3 0 0 0 -2 
system partSys particle 11 7 0 3 0 5 0 2 
system partSys particle 12 7 -1.1 3 0 9 0 0 
system partSys particle 13 7 -2 3 1 8 0 4 
system partSys particle 14 7 -3 3.1 0 7 2 0 
system partSys particle 15 7 0.5 3 1 6 0 2 
system partSys particle 16 7 0.1 3.3 0 5 0 -1 
system partSys particle 17 7 -0.1 3 1 4 0 0 
system partSys particle 18 7 3 3 -3 -1 0 -1 
system partSys particle 19 7 -3 3 2 0 0 0 
system partSys particle 20 7 -2 3 1 0 0 -2 
system partSys particle 21 7 2 3 0 5 0 2 
system partSys particle 22 7 -1 3 -1 9 0 0 
system partSys particle 23 7 -2 3 0.5 8 0 4 
system partSys particle 24 7 -3.4 3 0 7 2 0 
system partSys particle 25 7 2 3 1 6 0 2 
system partSys particle 26 7 1.1 3 0 5 0 -1 
system partSys particle 27 7 -2.1 3 0 4 0 0 
system partSys particle 28 7 2.5 3 3 -1 0 -1 
system partSys particle 29 7 -3 3 -2 1 0 0 
system partSys particle 30 7 -1 3 -2 0 1 0 
system partSys particle 31 7 -3 3 -1 0 0 1 
system partSys particle 32 7 -3 3 2 0 0 2 
system partSys particle 33 7 -3 2 -2 0 2 0 
system partSys particle 34 7 -2.5 3 -2 2 0 0 
system partSys particle 35 7 -3 3 0 3 0 0 
system partSys particle 36 7 -3 1 -2 0 3 0 
system partSys particle 37 7 3 3 3 0 0 3
system partSys particle 38 7 -2.1 1 -2 -1 0 0 
system partSys particle 39 7 1 3 -1 0 -1 0 
system partSys particle 40 7 -3 3 -2.41 0 0 -1 
system partSys particle 41 7 3 2.2 2 0 0 -2 
system partSys particle 42 7 2.123 3 -2 0 -2 0 
system partSys particle 43 7 1 1 1 -2 0 0 
system partSys particle 44 7 2.1 3 -2 -3 0 0 
system partSys particle 45 7 -3 3 2.56 0 -3 0 
system partSys particle 46 7 -3 2.76 -2 0 0 -3 
system partSys particle 47 7 2.645 3 2 1 1 1 
system partSys particle 48 7 3 3 1.21 1 0 1 
system partSys particle 49 7 -3 3 2.98 1 0 -1 
system partSys particle 50 7 -3 1.5437 -2 -1 0 -1 
system partSys particle 51 7 2.98 3 -2 -2 1 2 
system partSys particle 52 7 3 3 1.64 3 0 -3 
system partSys particle 53 7 -3 3 -2.9876 1 0 1 
system partSys particle 54 7 -3 3 -2.49 -1 0 2 
system partSys particle 55 7 -3 3 -2.356 1 0 1 

system partSys wall 3 0 0 -1 0 0 5000 100
system partSys wall -3 0 0 1 0 0 5000 100
system partSys wall 0 0 3 0 0 -1 5000 100
system partSys wall 0 0 -3 0 0 1 5000 100
system partSys wall 0 6 0 0 -1 0 5000 100

simulator partSim link partSys
#simulator partSim integration euler .01
#simulator partSim integration symplectic .01
simulator partSim integration verlet .01
simulator partSim ground 5000 300
simulator partSim drag 1
simulator partSim gravity -10




