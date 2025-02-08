% Defined the robot links using DH parameters
L(1) = Link([0, 0.340, 0, -pi/2]);
L(2) = Link([0, 0, 0, pi/2]);
L(3) = Link([0, 0.400, 0, pi/2]);
L(4) = Link([0, 0, 0, -pi/2]);
L(5) = Link([0, 0.400, 0, -pi/2]);
L(6) = Link([0, 0, 0, pi/2]);
L(7) = Link([0, 0.126, 0, 0]);

KR = SerialLink(L, 'name', "Kuka Robot iiwa");

figure;
% KR.plot(zeros(1, 7)); % Plot the robot with all joint angles set to zero
% KR.plot([0.82361577, 1.59232724, -2.08919968, 1.24093020, 1.73108660, 1.06782190, -0.58696495]);

% Reading the joint angles from the file
fileID = fopen('Muthupalaniappan_Annamalai.txt','r');
formatSpec = '%f %f %f %f %f %f %f';
sizeA = [7 Inf];
A = fscanf(fileID, formatSpec, sizeA);
fclose(fileID);

% Plotting the robot in each configuration from the file
for i = 1:2000/50
    KR.plot(A(:, i*50)');
    fprintf("percent done: %.2f\n", i/2000);
end
