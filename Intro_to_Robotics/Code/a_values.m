%Function for computing a values
function a = a_values(q_i,q_f)

a = zeros(7,2);

for i = 1:7
    %a0 = initial q_i, a1 = 0, Since inital and final velocities are 0
    a2 = (-3*(q_i(i) - q_f(i)))/64; 
    a3 = (2*(q_i(i) - q_f(i)))/512;
    a(i,1) = a3;
    a(i,2) = a2;
end