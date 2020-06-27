%% sinusoid

Fs = 44100; % Hz
T = 5; % Length of track, s

f = linspace(2000, 6000, Fs*T); % Start and end of sweep, Hz
t = linspace(0,T,Fs*T);
y = 5000*sin(2*pi*f.*t);

y = int16(y);

ys = [y' y']; % Stereo
audiowrite('001_sweep.wav', ys, Fs);


%% white noise

Fs = 44100; % Hz
T = 5; % Length of track, s

y = 8000*randn(1,T*Fs);

y = int16(y);

ys = [y' y']; % Stereo
audiowrite('002_whitenoise.wav', ys, Fs);