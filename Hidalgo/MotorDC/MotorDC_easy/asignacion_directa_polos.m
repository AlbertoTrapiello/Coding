function [Kp,Ti,Td]=asignacion_directa_polos(K,T1,T2,sita,wn,alfa,key)

switch key
case 1, Td = 0; Kp = (2*sita*wn*T1-1)/K; Ti = (2*sita*wn*T1-1)/(wn^2*T1);
case 2, Ti = (T1*T2*wn^2*(1+2*alfa*sita)-1)/T1/T2/alfa/wn^3; Kp = (T1*T2*wn^2*(1+2*alfa*sita)-1)/K; Td  = (T1*T2*wn^2*(1+2*alfa*sita)-T1-T2)/(T1*T2*wn^2*(1+2*alfa*sita)-1);
end 
