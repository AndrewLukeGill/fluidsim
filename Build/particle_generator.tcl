system partSys gen -2.5 -3 20 15 1 -1 1000 1

#system partSys particle 599 10 1.5 5 0 0 0 0  

system partSys wall 2 0 0 -1 0 0 5000 100
system partSys wall -3 0 0 1 0 0 5000 100
system partSys wall 0 0 1 0 0 -1 5000 100
system partSys wall 0 0 -1 0 0 1 5000 100
#system partSys wall 0 20 0 0 -1 0 5000 100

simulator partSim link partSys

#simulator partSim fix 599

#simulator partSim integration euler .01
simulator partSim integration symplectic .01
#simulator partSim integration verlet .01
simulator partSim ground 5000 300
simulator partSim drag 1
simulator partSim gravity -10

#system partSys new 1.5 5 0 0 0 0 50 0.1 1 1