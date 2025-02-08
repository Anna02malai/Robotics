function T_matrix = t_matrix(q) %Function to return transformation matrix

%Storing DH Parameters
alpha = [-pi/2, pi/2, pi/2, -pi/2, -pi/2, pi/2, 0];
a = zeros(1,7);
d = [0.340, 0, 0.400, 0, 0.400, 0, 0.126];

%Extracting Transformation Matrices
for k = 1:7
    T{k} = [cos(q(k)) -sin(q(k))*cos(alpha(k)) sin(q(k))*sin(alpha(k)) a(k)*cos(q(k));
            sin(q(k)) cos(q(k))*cos(alpha(k)) -cos(q(k))*sin(alpha(k)) a(k)*sin(q(k));
            0 sin(alpha(k)) cos(alpha(k)) d(k);
            0 0 0 1]; 
end

%Computing Respective Transformation Matrices
T02 = T{1}*T{2};
T03 = T02*T{3};
T04 = T03*T{4};
T05 = T04*T{5};
T06 = T05*T{6};
T07 = T06*T{7};

T_matrix = T07;
end