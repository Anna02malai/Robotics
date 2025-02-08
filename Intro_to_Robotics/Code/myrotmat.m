function R = myrotmat(theta, axis)

switch axis
    case {'x','X'}
        R = [1 0 0 ; 0 cos(theta) -sin(theta); 0 sin(theta) cos(theta)];
    case {'y','Y'}
        R = [cos(theta) 0 sin(theta); 0 1 0; -sin(theta) 0 cos(theta)];
    case {'z','Z'}
        R = [cos(theta) -sin(theta) 0; sin(theta) cos(theta) 0; 0 0 1];
    otherwise
        disp('Unknown axis. Please use x, y or z');
        R = [];
end


        