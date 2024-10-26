Fs = 10;
T = 1/Fs;
L = 50;
t = (0 : L - 1)*T;
w = (0 : L - 1) * Fs / L;
function y = f(t)
    y = 2 * sin(2 * pi * t / 180);
end
function y = g(t)
    y = 3 * sin(4 * pi * t / 180);
end
a = 20;
b = -10;
left_FFT = fft(a * f(t) + b * g(t));
right_FFT = a * fft(f(t)) + b * fft(g(t));


figure;
subplot(2, 2, 1);
stem(w, abs(left_FFT), "Marker", "_", "LineWidth", 1.5, "Color", "blue");
title("Амплитудный спектр (л.ч.)");
subplot(2, 2, 3);
stem(w, angle(left_FFT), "Marker", "_", "LineWidth", 1.5, "Color", "blue");
title("Фазовый спектр (л.ч.)");
subplot(2, 2, 2);
stem(w, abs(right_FFT), "Marker", "_", "LineWidth", 1.5, "Color", "red");
title("Амплитудный спектр (п.ч.)");
subplot(2, 2, 4);
stem(w, angle(right_FFT), "Marker", "_", "LineWidth", 1.5, "Color", "red");
title("Фазовый спектр (п.ч.)");