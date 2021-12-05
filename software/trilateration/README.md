
Trilateration is the determination of a navigation position methodology based on simultaneous ranging from three trackers.Through the use of three beacons and their associated RSSI, Trilateration is used to estimate the location of an unknown point. Distances (d_one, d_two, d_three) are measured by an RSSI signal. The location of the unknown point of interest can be found by solving the following system of quadratic equations. The script in this directory explores a baseline approach for Trilateration in python that was used to build the intuiton and direction of the project. The motivating paper for this exploration is listed below and highly recommended for anyone interested in learning about this technique and overall discipline.


R. Faragher and R. Harle, "Location Fingerprinting With Bluetooth Low Energy Beacons," in IEEE Journal on Selected Areas in Communications, vol. 33, no. 11, pp. 2418-2428, Nov. 2015, doi: 10.1109/JSAC.2015.2430281.
