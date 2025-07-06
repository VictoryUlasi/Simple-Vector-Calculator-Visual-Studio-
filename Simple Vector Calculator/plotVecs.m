clear,clc,close all;

vSet = dlmread("vectors.txt");

v1 = vSet(1,:);
v2 = vSet(2,:);
vDimension = vSet(3,:);

vMatrix = [v1;v2]

figure(Name="Vector Calculator", NumberTitle='off')
if vDimension(1,1) == 3
    a = v1;  %# Vector 1
    b = v2;  %# Vector 2
    c = a+b;
    
    starts = zeros(3,3);
    ends = [a;b;c];
    
    q = quiver3(starts(:,1), starts(:,2), starts(:,3), ends(:,1), ends(:,2), ends(:,3))
    axis equal
else
    a = v1;  %# Vector 1
    b = v2;  %# Vector 2
    c = a+b;
    
    starts = zeros(3,2);
    ends = [a;b;c];
    
    q = quiver(starts(:,1),starts(:,2),ends(:,1),ends(:,2))
    q.LineWidth = 3;
    axis equal
end
xlabel('X axis');
ylabel('Y axis');
zlabel('Z axis');