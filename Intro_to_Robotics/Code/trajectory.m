function  [q_tra_pos, q_dot_vel] = trajectory(q_initial,q_final) %Function for designing the trajectory of the robot

%Trajectory Design
%Trajectory motion = Total of 10secs (8 + 2). 
t = 0:0.005:9.995;
t_m  = t(1:1600);

%Initializing space for joint angles 1 to 7
q1 = zeros(length(t),1);
q2 = zeros(length(t),1);
q3 = zeros(length(t),1);
q4 = zeros(length(t),1);
q5 = zeros(length(t),1);
q6 = zeros(length(t),1);
q7 = zeros(length(t),1);

%Initializing space for joint velocities 1 to 7
q1_dot = zeros(length(t),1);
q2_dot = zeros(length(t),1);
q3_dot = zeros(length(t),1);
q4_dot = zeros(length(t),1);
q5_dot = zeros(length(t),1);
q6_dot = zeros(length(t),1);
q7_dot = zeros(length(t),1);

%Getting a3 and a2 values from a_values function
a = a_values(q_initial,q_final);

%Solving cubic polynomial for joint angles
q1(1:1600) = a(1,1)*t_m.^3 + a(1,2)*t_m.^2 + q_initial(1,1);
q2(1:1600) = a(2,1)*t_m.^3 + a(2,2)*t_m.^2 + q_initial(2,1);
q3(1:1600) = a(3,1)*t_m.^3 + a(3,2)*t_m.^2 + q_initial(3,1);
q4(1:1600) = a(4,1)*t_m.^3 + a(4,2)*t_m.^2 + q_initial(4,1);
q5(1:1600) = a(5,1)*t_m.^3 + a(5,2)*t_m.^2 + q_initial(5,1);
q6(1:1600) = a(6,1)*t_m.^3 + a(6,2)*t_m.^2 + q_initial(6,1);
q7(1:1600) = a(7,1)*t_m.^3 + a(7,2)*t_m.^2 + q_initial(7,1);

q1(1601:2000) = q1(1600);
q2(1601:2000) = q2(1600);
q3(1601:2000) = q3(1600);
q4(1601:2000) = q4(1600);
q5(1601:2000) = q5(1600);
q6(1601:2000) = q6(1600);
q7(1601:2000) = q7(1600);

%Storing joint angles in radians
q_tra = [q1 q2 q3 q4 q5 q6 q7];
q_tra_pos = q_tra';

%Storing joint angles in radians in text file
fileID = fopen('Muthupalaniappan_Annamalai.txt','w');
for i=1:2000
    if i<2000
        fprintf(fileID,'%.8f %.8f %.8f %.8f %.8f %.8f %.8f\n',q_tra_pos(1:7,i));
    else 
        fprintf(fileID,'%.8f %.8f %.8f %.8f %.8f %.8f %.8f',q_tra_pos(1:7,i));
    end
end
fclose(fileID);
%solving derivative of cubic polynomial for joint velocities
q1_dot(1:1600) = 3*a(1,1)*t_m.^2 + 2*a(1,2)*t_m;
q2_dot(1:1600) = 3*a(2,1)*t_m.^2 + 2*a(2,2)*t_m;
q3_dot(1:1600) = 3*a(3,1)*t_m.^2 + 2*a(3,2)*t_m;

q4_dot(1:1600) = 3*a(4,1)*t_m.^2 + 2*a(4,2)*t_m;
q5_dot(1:1600) = 3*a(5,1)*t_m.^2 + 2*a(5,2)*t_m;
q6_dot(1:1600) = 3*a(6,1)*t_m.^2 + 2*a(6,2)*t_m;
q7_dot(1:1600) = 3*a(7,1)*t_m.^2 + 2*a(7,2)*t_m;

q1_dot(1601:2000) = 0;
q2_dot(1601:2000) = 0;
q3_dot(1601:2000) = 0;
q4_dot(1601:2000) = 0;
q5_dot(1601:2000) = 0;
q6_dot(1601:2000) = 0;
q7_dot(1601:2000) = 0;

% storing joint velocities in radians
q_dot_vel = [q1_dot q2_dot q3_dot q4_dot q5_dot q6_dot q7_dot];
% q_tra_vel = q_dot_vel';

% %Storing joint angles in radians in text file
% fileID = fopen('Velocity.txt','w');
% fprintf(fileID,'%.8f %.8f %.8f %.8f %.8f %.8f %.8f\n',q_tra_vel);
% fclose(fileID);

end