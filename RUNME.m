close all
clear
% Your simulations will use this structure
[m,v,s,search]=aStar('map_8.txt',[2,2],[1,18]);
%Here   'm' is the map file returned as a matrix
%       'v' is a matrix that shows which cells have been visited, '0' means
%       visited, '1' means not visited
%       's' is the vector of steps to reach the target,
%       [startloc] is a vector i.e. [2,2]
%       [targetloc] is also a vector ie [4,18]

% To view the path determined
figure
plotmap(m,s);

% To view the searched paths
figure
plotmap(m,search);