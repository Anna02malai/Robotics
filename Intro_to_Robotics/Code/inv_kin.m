function  Q_final = inv_kin(T0e_F) %Function to return final configuration of the robot

%Desired position - PD
PD = T0e_F(1:3,4);

%Desired orientation - PHID
phi = atan2(T0e_F(2,3),T0e_F(1,3));
theta = atan2(sqrt((T0e_F(1,3))^2 + (T0e_F(2,3))^2),(T0e_F(3,3)));
psi = atan2(T0e_F(3,2),-T0e_F(3,1));
PHID = [phi; theta; psi];

steps = 100000;

%DH parameters - (alpha, a, d, q)
alpha = [-pi/2, pi/2, pi/2, -pi/2, -pi/2, pi/2, 0];
a = zeros(1,7);
d = [0.340, 0, 0.400, 0, 0.400, 0, 0.126];
q = zeros(7, steps);

%Given initial configuration
q_initial = deg2rad([58.2686; 75.3224; 11.7968; 45.9029; -22.1081; -31.2831; -42.3712]); %storing as radians
q(:,1) = q_initial;

%Initialization of Error matrix
e = zeros(6, steps);

%Finding Tphi and Ta for analytical jacobian
Tphi = [0 -sin(phi) cos(phi)*sin(theta);
        0 cos(phi) sin(phi)*sin(theta);
        1 0 cos(theta)];
I = eye(3,3);
O = zeros(3,3);
Ta = [I O; O Tphi];

%Assigning gain value
K = 100*eye(6,6);

%Iterating to find optimized Solution
for i=1:steps
    xe = pose_fwd_kin(q(:,i));             %Storing pose matrix at q
    Ja = an_jacobian(q(:,i), PD, Ta);      %Storing analytical jacobain at q
    e(:,i) = [PD; PHID] - xe;              %Computing error between desired and actual pose matrix
    pseudo_inv = pinv(Ja);                 %Finding pseudo inverse of Jacobian matrix
    qdot = pseudo_inv*K*e(:,i);            %Finding qdot with respest to gain K
    q(:,i+1) = q(:,i) + qdot*0.001;        %Incrementing q with qdot*0.001(delta)
    if (max(abs(e(:,i))) < 0.001)          %Breaking loop when error < 0.001 (less than 1 mm or radian) 
        final_config = q(:,i);             %Final configuration is last set of q values
        break;                             %Note: Exact config was found when error was <0.000001 (less than 0.001 mm or radian)
    end
    for j = 1:7
        for k = 1:steps
            if q(j,k) > pi
             	q(j,k) = q(j,k) - pi;       %constraining the q values above 180 degrees
            elseif q(j,k) < -pi       
                q(j,k) = q(j,k) + pi;       %constraining the q values below 180 degrees
            end            
        end
    end
end

%Testing the forward kinematics to give final configuration. 
% test = pose_fwd_kin(final_config);
% disp("Pose Matrix of Final Configuration (x, y, z, phi, theta, psi) (Testing fwd_kin)");
% disp(test);

%Printing outputs as required
disp("Computed Configuration (q values in deg)");
disp(rad2deg(final_config'));

disp("Desired Transformation Matrix(T07)");
disp(T0e_F);

disp("Actual Transformation Matrix");
disp(t_matrix(final_config));

Q_final = final_config';
end