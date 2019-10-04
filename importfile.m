function [hci0,hci1,hci2,flg] = importfile(filename, startRow, endRow)
%IMPORTFILE テキスト ファイルから数値データを列ベクトルとしてインポートします。
%   [HCI0,HCI1,HCI2,FLG] = IMPORTFILE(FILENAME) 既定の選択については テキスト ファイル
%   FILENAME からデータを読み取ります。
%
%   [HCI0,HCI1,HCI2,FLG] = IMPORTFILE(FILENAME, STARTROW, ENDROW) テキスト ファイル
%   FILENAME の STARTROW 行から ENDROW 行までのデータを読み取ります。
%
% Example:
%   [hci0,hci1,hci2,flg] = importfile('rssi.csv',2, 61);
%
%    TEXTSCAN も参照してください。

% MATLAB による自動生成 2019/10/04 16:55:20

%% 変数を初期化します。
delimiter = ',';
if nargin<=2
    startRow = 2;
    endRow = inf;
end

%% テキストの各行の書式設定:
%   列1: double (%f)
%	列2: double (%f)
%   列3: double (%f)
%	列4: テキスト (%s)
% 詳細は TEXTSCAN のドキュメンテーションを参照してください。
formatSpec = '%f%f%f%s%[^\n\r]';

%% テキスト ファイルを開きます。
fileID = fopen(filename,'r');

%% データの列を書式設定に従って読み取ります。
% この呼び出しは、このコードの生成に使用されたファイルの構造に基づいています。別のファイルでエラーが発生する場合は、インポート
% ツールからコードの再生成を試みてください。
dataArray = textscan(fileID, formatSpec, endRow(1)-startRow(1)+1, 'Delimiter', delimiter, 'TextType', 'string', 'HeaderLines', startRow(1)-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');
for block=2:length(startRow)
    frewind(fileID);
    dataArrayBlock = textscan(fileID, formatSpec, endRow(block)-startRow(block)+1, 'Delimiter', delimiter, 'TextType', 'string', 'HeaderLines', startRow(block)-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');
    for col=1:length(dataArray)
        dataArray{col} = [dataArray{col};dataArrayBlock{col}];
    end
end

%% テキスト ファイルを閉じます。
fclose(fileID);

%% インポートできないデータの後処理。
% インポート中に、インポートできないデータの規則が適用されなかったため、後処理コードが含まれていません。インポートできないデータに適用できるコードを生成するには、ファイル内のインポートできないセルを選択してからスクリプトを再生成します。

%% インポートした配列を列変数名に割り当てます
hci0 = dataArray{:, 1};
hci1 = dataArray{:, 2};
hci2 = dataArray{:, 3};
flg = dataArray{:, 4};


