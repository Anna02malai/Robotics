function An_jac = an_jacobian(q, pe, Ta) %function to find Analytical Jacobian

%Storing the DH parameters
alpha = [-pi/2, pi/2, pi/2, -pi/2, -pi/2, pi/2, 0];
a = zeros(1,7);
d = [0.340, 0, 0.400, 0, 0.400, 0, 0.126];

%Extracting transformation matrices
for k = 1:7
    T{k} = [cos(q(k)) -sin(q(k))*cos(alpha(k)) sin(q(k))*sin(alpha(k)) a(k)*cos(q(k));
            sin(q(k)) cos(q(k))*cos(alpha(k)) -cos(q(k))*sin(alpha(k)) a(k)*sin(q(k));
            0 sin(alpha(k)) cos(alpha(k)) d(k);
            0 0 0 1]; 
end

%Computing Respective Transformation Matrices
T01 = T{1};
T02 = T01*T{2};
T03 = T02*T{3};
T04 = T03*T{4};
T05 = T04*T{5};
T06 = T05*T{6};

%Extracting position and Z matrices respectively
z0 = [0 0 1]'; p0 = [0,0,0]';
z1 = T{1}(1:3,3); p1 = T{1}(1:3,4);
z2 = T02(1:3,3); p2 = T02(1:3,4);
z3 = T03(1:3,3); p3 = T03(1:3,4);
z4 = T04(1:3,3); p4 = T04(1:3,4);
z5 = T05(1:3,3); p5 = T05(1:3,4);
z6 = T06(1:3,3); p6 = T06(1:3,4);

%Computing Geometric Jacobian
Jg = [ cross(z0, pe-p0) cross(z1, pe-p1) cross(z2, pe-p2) cross(z3, pe-p3) cross(z4, pe-p4) cross(z5, pe-p5) cross(z6, pe-p6);
    z0 z1 z2 z3 z4 z5 z6];

%Finding Analytical Jacobian
An_jac = inv(Ta)*Jg;
end

