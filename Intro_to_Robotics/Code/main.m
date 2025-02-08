clc
clear all
close all

%configuration of the robot at the instance the picture is captured(given)
Qc_deg = [97.31 -4.70 162.57 103.53 -1.96 -62.33 113.31]; 
Qc_rad = deg2rad(Qc_deg);  % converting deg to rad

% T07/T0e - Transformation matrix at the instance the picture is captured
T0e = t_matrix(Qc_rad);

%Computing TeC (end-effector to camera)
%Given position and orientation from end effector to camera
xe_C = 0;
ye_C = -0.0662;
ze_C = 0.0431;
yawZe_c = -pi/2;
RotZe_c = myrotmat(yawZe_c, 'z');
TeC = [RotZe_c; 0 0 0];
TeC = [TeC, [xe_C; ye_C; ze_C; 1]];  %Constant matrix does not change

%Computing TCA (Camera to Aruco Marker)
%Given position and orientation of aruco marker w.r.to Camera
xc_A =  -0.205780720039398;
yc_A =  -0.109793029482687;
zc_A = 0.561252115509121;
roll_x = 174.1750404305652 * pi/180;  %Converting to rad
pitch_y = -17.3967534123935 * pi/180; %Converting to rad
yaw_z = -1.9587388578232 * pi/180; %Converting to rad

%TCA - Transformation matrix(Camera to Aruco Marker)
eul_ang_CA = [yaw_z pitch_y roll_x];
RotC_A = eul2rotm(eul_ang_CA,'ZYX');
TCA = [RotC_A; 0 0 0];
TCA = [TCA, [xc_A; yc_A; zc_A; 1]];

%Computing TAt (Aruco Marker to target)
%Given position and orientation from Aruco Marker to target
xA_t = 0.103975;
yA_t = -0.103975;
zA_t = 0;
yawZA_t = -pi/2;
RotZA_t = myrotmat(yawZA_t, 'z');
rollXA_t = pi;
RotXA_t = myrotmat(rollXA_t, 'x');
ArRot = RotZA_t * RotXA_t;
TAt = [ArRot; 0 0 0];
TAt = [TAt, [xA_t; yA_t; zA_t; 1]];

%T0t at Qc configuration (0th frame to Target)
T0t = T0e * TeC * TCA * TAt;

%TeS at Qf configuration(Final orientation of robot at target t)
xe_S = 0;
ye_S = 0.0455;
ze_S = 0.06;
TeS = [1 0 0; 0 1 0; 0 0 1; 0 0 0];
TeS = [TeS, [xe_S; ye_S; ze_S; 1]];

%Desired Transfomation matrix (or) Final Transformation matrix  
T0e_F = T0t * inv(TeS);

%Computing the Final configuration for trajectory design.
q_initial = deg2rad([58.2686; 75.3224; 11.7968; 45.9029; -22.1081; -31.2831; -42.3712]); %storing as radians
q_final = inv_kin(T0e_F); % in radians

%Trajectory Design
%Trajectory motion = Total of 10secs (8 + 2). 
t = 0:0.005:9.995;
[q_traj, q_vel] = trajectory(q_initial,q_final);

%Checking for joint limit or velocity limit violations !!!! 
q_tra = rad2deg(q_traj');
q_dot_tra = rad2deg(q_vel);
limit_check = limit(q_tra, q_dot_tra);

%Plotting Angles, Velocities and Accelerations
subplot(2,1,1);
plot(t,q_tra)
title('Joint Angles(Deg)')
xlabel('Time (s)');
ylabel('Angle (Deg)');
yline(120, '--', 'LineWidth', 1.5); %lowest limit of all
yline(-120, '--', 'LineWidth', 1.5); %lowest limit of all

subplot(2,1,2); 
plot(t,q_dot_tra) %q_dot_tra 
title('Joint Velocities(Deg/5ms)')
xlabel('Time (s)');
ylabel('velocity (Deg/5ms)');
yline(98, '--', 'LineWidth', 1.5); %lowest limit of all
yline(-98, '--', 'LineWidth', 1.5); %lowest limit of all