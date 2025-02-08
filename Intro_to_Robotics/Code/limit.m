function Limits = limit(q_tra_pos, q_dot_vel) %Function to return

lmt_ct = 0;
for m = 1:length(q_tra_pos)
    for n = 1:7
        if n==1
            if abs(q_tra_pos(m,n)) >=170 || abs(q_dot_vel(m,n)) >= 98                        
                lmt_ct = lmt_ct + 1;
            end
        elseif n==2
            if abs(q_tra_pos(m,n)) >=120 || abs(q_dot_vel(m,n)) >= 98 
                lmt_ct = lmt_ct + 1;
            end
        elseif n==3
            if abs(q_tra_pos(m,n)) >=170 || abs(q_dot_vel(m,n)) >= 100
                lmt_ct = lmt_ct + 1;
            end
        elseif n==4
            if abs(q_tra_pos(m,n)) >=120 || abs(q_dot_vel(m,n)) >= 130
                lmt_ct = lmt_ct + 1;
            end
        elseif n==5
            if abs(q_tra_pos(m,n)) >=170 || abs(q_dot_vel(m,n)) >= 140
                lmt_ct = lmt_ct + 1;
            end
        elseif n==6
            if abs(q_tra_pos(m,n)) >=120 || abs(q_dot_vel(m,n)) >= 180
                lmt_ct = lmt_ct + 1;
            end
        elseif n==7
            if abs(q_tra_pos(m,n)) >=175 || abs(q_dot_vel(m,n)) >= 180
                lmt_ct = lmt_ct + 1;
            end
        end
    end
end

if lmt_ct > 0
    disp('Exceeded Joints or Velocity limits');
    else
    disp('Did not exceed Joints or Velocity limits');
end

Limits = lmt_ct;
end