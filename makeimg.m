%% ファイル名・回数
clear;
close all;

dirname = 'csvs\';
folder  = dir(dirname);
marks   = ["o", "*", "s"];
lines   = ["-", "--", "-."];

for i=1:length(folder)
    if folder(i).name ~= "." && folder(i).name ~= ".."
        figure('Name', strcat(folder(i).name));
        hold on;
        
        csvs = dir(strcat(dirname, folder(i).name, '\*.csv'));
        for j=1:length(csvs)
            eval(strcat(strtok(csvs(j).name, "."), "=readtable(strcat(dirname, folder(i).name, '\', csvs(j).name));"));
        end
        
        for j=0:2
            scatter(eval("hci" + int2str(j) + ".time"), eval("hci" + int2str(j) + ".rssi"), marks(j+1));
            plot(rssi.count, eval("rssi.hci" + int2str(j)), lines(j+1));
        end
        xlabel('t[s]') ;
        ylabel('RSSI[dBm]');
        xlim([0, 60]);
        ylim([-90, 0]);
        legend("HCI0 Received data", "HCI0 Moving average", "HCI1 Received data", "HCI1 Moving average", "HCI2 Received data", "HCI2 Moving average");
        
        % saveas(gcf, strcat("img\", filename, "-", int2str(i)), "epsc")
    end
end
