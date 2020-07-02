tax = load("-ascii", "tgx.txt");
event = load("-ascii", "event.txt");
event = reshape(event ,1, 3721);
tax = reshape(tax ,1, 3720);
tax = [tax 0]
#tax = tax(1400:1600)
#event = event(1400:1600)
tax = tax(450:700)
event = event(450:700)
Fs = 1000;
Ts = 1/10;
duraT = 1;

dt = 1/Fs;
#tAxis = 140:0.1:160;
tAxis = 45:0.1:70;

y = tax;   y=y';

L = length (y); % Window Length of FFT    
nfft = 2^nextpow2(L); % Transform length

y_HannWnd = y.*hanning(L);            
Ydft_HannWnd = fft(y_HannWnd,nfft)/L;

   % at all frequencies except zero and the Nyquist
   mYdft = abs(Ydft_HannWnd);
   mYdft = mYdft (1:nfft/2+1);
   mYdft (2:end-1) = 2* mYdft(2:end-1);

f = Fs/2*linspace(0,1,nfft/2+1); 

  figure(1),
  subplot(3,1,1)
  plot(tAxis,y)
  title('Time Domain y(t)');
  xlabel('Time,s'); 
  ylabel('y');
  subplot(3,1,2)
  plot(tAxis,event)
  title('Events');
  xlabel('Time,s'); 
  ylabel('event');
  subplot(3,1,3)  
  plot(f,2*mYdft); % why need *2 ? Bcoz, Hanning Wnd Amplitude Correction Factor = 2
  axis ([0 500 0 150]); %Zoom in 
  title('Amplitude Spectrum with Hann Wnd');
  xlabel('Frequency (Hz)with hanning window'); 
  ylabel('|Y(f)|');
  