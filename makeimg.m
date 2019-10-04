%% ファイル名・回数
clear;
close all;

path     = "C:\Users\Marushima Kentaro\Desktop\BLE-progs\csvs\";
% filename = "1400-1300-1200";
% filename = "900-700-600";
filename = "all30";
range    = 3;

%% 表生成
for i = 1:range
    % 移動平均読み込み(hci0, hci1, hci2)
    folderpath = strcat(path, filename, "-", num2str(i));
    RSSIs = readtable(folderpath + "\rssi.csv", 'ReadRowNames',true);
    
    % 各RSSI生データ読み込み(time, rssi)
    hci0 = readtable(folderpath + "\hci0.csv");
    hci1 = readtable(folderpath + "\hci1.csv");
    hci2 = readtable(folderpath + "\hci2.csv");
    
    figure('Name',strcat(filename, "-", int2str(i)))
    hold on;
    for j = 0:2
        % 生データ描画
        if j == 0
            mark = "o";
            line = "-";
        elseif j == 1
            mark = "*";
            line = "--";
        else
            mark = "s";
            line = "-.";
        end
        scatter(eval("hci" + int2str(j) + ".time"), eval("hci" + int2str(j) + ".rssi"), mark)

        
        % 移動平均描画
        plot(RSSIs.count, eval("RSSIs.hci" + int2str(j)), line)
    end
    
                
    % ラベル等
    xlabel('t[s]') ;
    ylabel('RSSI[dBm]');
    xlim([0, 60]);
    ylim([-90, 0]);
    % lsline;
    legend("HCI0 Received data", "HCI0 Moving average", "HCI1 Received data", "HCI1 Moving average", "HCI2 Received data", "HCI2 Moving average");

    saveas(gcf, strcat("img\", filename, "-", int2str(i)), "epsc")
end

