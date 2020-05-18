BandFilt = designfilt('bandpassiir','FilterOrder',20, ...          
'PassbandFrequency1', 20, 'PassbandFrequency2', 20e3, ...          
'SampleRate',200e3); 
%fvtool(BandFilt)

Fs = 5000;                    % sample rate in Hz 
Ts = 1/Fs;  % Periodo de la señal
N = 5;                     % number of signal samples rng default;
t = 0:1/1e6:7*Ts;
signal = N*sin(2*pi*(Fs*t));
Fn_High = 50000;
noise_High = 5*sin(2*pi*(Fn_High*t)+2*pi*rand/36);% noisy waveform 
% time vector  % Design a 70th order lowpass FIR filter with cutoff frequency of 75 Hz.  Fnorm = 75/(Fs/2);           % Normalized frequency df = designfilt('lowpassfir','FilterOrder',70,'CutoffFrequency',Fnorm); 
x = signal + noise_High;
% Design a 7th order lowpass IIR elliptic filter with cutoff frequency % of 75 Hz.  

df = designfilt('highpassiir',...
    'FilterOrder',7, ...
    'HalfPowerFrequency',Fs/2/pi*1.1,...        
    'SampleRate',200e3);
%df = designfilt('lowpassiir', 'NumeratorOrder', 1, 'DenominatorOrder', ...
%                2, 'HalfPowerFrequency', 0.5, 'SampleRate', 200000);

fvtool(df)

y1 = filter(df,x);    % non-linear phase filter - no delay compensation 
%y2 = filtfilt(df,x);  % zero-phase implementation - delay compensation  

figure(2)
plot(t,signal,'b');hold on; plot(t,noise_High,'g'); legend('Signal','Noise')
figure(3)
plot(t,x,'r');
hold on
plot(t,y1,'k'); legend('Noisy signal', 'Filtered signal');

%plot(t,x); hold on plot(t,y1,'r','linewidth',1.5); %plot(t,y2,'g','linewidth',1.5); title('Filtered Waveforms'); xlabel('Time (s)') 
%legend('Original Signal','Non-linear phase IIR output',...   
%'Zero-phase IIR output'); 
%ax = axis; axis([0.25 0.55 ax(3:4)])
grid on 

