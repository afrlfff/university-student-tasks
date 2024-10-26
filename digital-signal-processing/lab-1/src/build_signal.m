% SETTINGS
f = 250;
isDelay = true;
sine_offset = 0.00118;
t_start = 0.0;
t_end = 0.02;


% read Signal points
if isDelay
    lines = readlines("../data/putty" + f + " del.txt");
else
    lines = readlines("../data/putty" + f + ".txt");
end
lines = lines(2 : length(lines) - 1);
S = str2double(lines); % Signal


% get true voltage values
maxA = 512;
S = S / (maxA / 2.5);
% calculate sampling frequency (number of points per second) and dt (step)
Fs = length(S) / 10;
dt = 1 / Fs;


% build sinusoid
num_of_sine_samples = 1000;
w = 2 * pi * f;
t = t_start : (t_end - t_start) / (num_of_sine_samples - 1) : t_start + (t_end - t_start);
y = sin(w*(t + sine_offset)) + 1.5;
plot(t, y, "LineWidth", 1.5, "Color", "red");
hold on


% build Signal
num_of_samples = floor((t_end - t_start) / dt) + 1;
S = S(1 : num_of_samples);
t = t_start : dt : t_start + dt * (num_of_samples - 1);
graphic = plot(t, S, "LineWidth", 2, "Marker","*", "Color", "blue");
hold off


% set title and labels
if isDelay
    title("Сигнал при частоте " + int2str(f) + " Гц. delay(0.9)");
else
    title("Сигнал при частоте " + int2str(f) + " Гц");
end
xlabel('t, с')
ylabel('U(t), В')
legend("y = sin(w(t + " + num2str(sine_offset) + ")) + 1.5", "Signal")


% save graphic
% if isDelay
%     saveas(graphic, "../results/signal_delay/putty" + f + " del.jpg", "jpg")
% else
%     saveas(graphic, "../results/signal/putty" + f + ".jpg", "jpg")
% end