% SETTINGS
f = 20000;
isDelay = true;
%
isAmplitude = true; % false = phase spectrum
%
isRemoveExcess = true;
remove_threshold = 0.125;
%
isZoom = true;
bounds = [74, 76];
%
isSave = false;


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
% calculate sampling frequency
Fs = length(S) / 10;


% get points in frequency domain and make fft
F = (0 : (length(S) - 1)) * Fs / length(S);
S_FFT = fft(S);


% get amplitude and phase spectrums
ampl = abs(S_FFT);
ampl = ampl/(length(F)/2);
phase = angle(S_FFT);
if isRemoveExcess
    ampl(ampl < remove_threshold) = 0;
    phase(ampl < remove_threshold) = 0;
end


% build amplitude spectrum
if isAmplitude
    fig = stem(F(1 : floor(length(F) / 2)), ...
               ampl(1 : floor(length(ampl) / 2)), ...
               "Marker", "_", "LineWidth", 1.5, "Color", "blue");
    title("Амплитудный спектр, f = " + f + " Гц")
    xlabel("f, Гц", "FontWeight", "bold")
    ylabel("U_m", "FontWeight", "bold")
    grid on;
    set(gca, 'GridLineStyle', '--', 'GridColor', 'black', 'GridLineWidth', 0.25, "GridAlpha", 1);
    if isZoom
        xlim([bounds(1), bounds(2)])
    end
end


% build phase spectrum
if not(isAmplitude)
    fig = stem(F(1 : floor(length(F) / 2)), ...
               phase(1 : floor(length(phase) / 2)), ...
               "Marker", "_", "LineWidth", 1.5, "Color", "blue");
    title("Фазовый спектр, f = " + f + " Гц")
    xlabel("f, Гц", "FontWeight", "bold")
    ylabel("\phi, рад", "FontWeight", "bold")
    grid on;
    set(gca, 'GridLineStyle', '--', 'GridColor', 'black', 'GridLineWidth', 0.25, "GridAlpha", 1);
    if isZoom
        xlim([bounds(1), bounds(2)])
    end
end


% get output path
output_path = "../results/";
if isAmplitude
    output_path = output_path + "amplitude_spectrum";
else
    output_path = output_path + "phase_spectrum";
end
if isDelay
    output_path = output_path + "_delay/putty" + f;
else
    output_path = output_path + "/putty" + f;
end
if isZoom
    output_path = output_path + "_zoom";
end
if isRemoveExcess
    output_path = output_path + "_clear";
end


% save
if isSave
    saveas(fig, output_path + ".jpg", "jpg")
end