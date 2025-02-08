# Intro to Robotics Project
## Manipulation of KUKA LBR iiwa 7 R800 Robot: 

The repository consists of the Full details of a project - Manipulation of the KUKU LBR iiwa 7 (a 7-DOF Robot). The project demonstrates the how to program a trajectory to the Kuka robot iiwa 7 R800 to position its end effector inside a prescribed area while accounting to the potential singularities and self collision.

<br>

The further details of the project is mentioned in the respective folders as shown below. 

1. Problem Statement: 
    - The folder consists of the file [Project.pdf](Intro_to_Robotics/Problem_Statement/Project.pdf) which describes the problem statement, expectations and end goal along with other details. 
    - The [sample.txt](Intro_to_Robotics\Problem_Statement\sample.txt) is the expected format of output. The ouput file should have the configurations of all the 7 joints of the robot for the 10 secs consisting of total of 2000 rows(lines) of configurations.

2. Robot Specifications:
    - The folder consists of file [LBRiiSpecifications.pdf](Intro_to_Robotics\Robot_Specifications\LBRiiwaSpecifications.pdf) which has the specifications of the Robot Kuka LBR iiwa 7 R800. It also has some examples and instructions to set up the robot and control it.
    - The [MediaFlange.pdf](Intro_to_Robotics\Robot_Specifications\MediaFlange.pdf) consists of the specifications of the Media Flange of the robot and its end effector. It also has the instructions and other required details for setting it up and running it.

3. Code: 
    <br>
    - The folders consists of several code files which perform its functions as described below. The user just needs to run the main file and the other code files are accessed and executed as shown in the order below.
        1. [main.m](Intro_to_Robotics\Code\main.m) : This the main code file. It needs to be run first. 
        2. [t_matrix.m](Intro_to_Robotics\Code\t_matrix.m) : It computes and returns the transformation matrix for the given configuration.
        3. [myrotmat.m](Intro_to_Robotics\Code\myrotmat.m) : It computes and returns the rotational matrix for the given theta (θ) and Axis ('X', 'Y', 'Z')
        4. [inv_kin.m](Intro_to_Robotics\Code\inv_kin.m) : It computes the inverse kinematics for the given Transformation Matrix and returns the final configuration of the Robot
        5. [pose_fwd_kin.m](Intro_to_Robotics\Code\pose_fwd_kin.m) : It computes the forward kinematics for the given configuration and returns the pose matrix (position and orientation) of the config.
        6. [an_jacobian](Intro_to_Robotics\Code\an_jacobian.m) : It computes and returns the analytical jacobian. 
        7. [trajectory.m](Intro_to_Robotics\Code\trajectory.m) : It computes the trajectory for the robot given the inital and final config of the robot. It provides the trajectory configurations by solving the cubic polynomial of the joint angles. It writes the configuration of all joints in [Results.txt](Intro_to_Robotics\Results\Results.txt) file as per the requirements. 
        8. [a_values.m](Intro_to_Robotics\Code\a_values.m) : It computes the a_values for the cubic polynomials given the initial and final config. 
        9. [limit.m](Intro_to_Robotics\Code\limit.m) : It checks whether the given trajectory configurations exceed the limits of all the joint angles of the robot. It is checked to protect the robot from break/ collision. 
        10. [simulation.m](Intro_to_Robotics\Code\simulation.m) : The file reads the configurations from the [Results.txt](Intro_to_Robotics\Results\Results.txt) and runs the simulation of the Robot for every 50 samples. The robot model is created with help of Robot Toolbox used to run the simulation. 

4. Results:
    - The folder contains the [Results.txt](Intro_to_Robotics\Results\Results.txt) where the trajectory configurations are saved and stored. It matches the required format as shown in the [sample.txt](Intro_to_Robotics\Problem_Statement\sample.txt). 
    - The [Simulation.mp4](Intro_to_Robotics\Results\Simulation.mp4) is the simulation of the robot performing it's prescribed motion

5. Report:
    - The [Report.pdf](Intro_to_Robotics\Report.pdf) file consists of detailed explanation of the concepts and steps on how to achieve the

6. Simulation:
    - The Simulation displays the trajectory of the Kuka robot iiwa 7 R800 and how it position its end effector inside a prescribed area while accounting to the potential singularities and self collision.

    https://github.com/user-attachments/assets/0f859638-cefa-4a1e-8748-64eb0f034b9d

    
