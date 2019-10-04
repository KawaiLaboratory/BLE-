function [hci0,hci1,hci2,flg] = importfile(filename, startRow, endRow)
%IMPORTFILE �e�L�X�g �t�@�C�����琔�l�f�[�^���x�N�g���Ƃ��ăC���|�[�g���܂��B
%   [HCI0,HCI1,HCI2,FLG] = IMPORTFILE(FILENAME) ����̑I���ɂ��Ă� �e�L�X�g �t�@�C��
%   FILENAME ����f�[�^��ǂݎ��܂��B
%
%   [HCI0,HCI1,HCI2,FLG] = IMPORTFILE(FILENAME, STARTROW, ENDROW) �e�L�X�g �t�@�C��
%   FILENAME �� STARTROW �s���� ENDROW �s�܂ł̃f�[�^��ǂݎ��܂��B
%
% Example:
%   [hci0,hci1,hci2,flg] = importfile('rssi.csv',2, 61);
%
%    TEXTSCAN ���Q�Ƃ��Ă��������B

% MATLAB �ɂ�鎩������ 2019/10/04 16:55:20

%% �ϐ������������܂��B
delimiter = ',';
if nargin<=2
    startRow = 2;
    endRow = inf;
end

%% �e�L�X�g�̊e�s�̏����ݒ�:
%   ��1: double (%f)
%	��2: double (%f)
%   ��3: double (%f)
%	��4: �e�L�X�g (%s)
% �ڍׂ� TEXTSCAN �̃h�L�������e�[�V�������Q�Ƃ��Ă��������B
formatSpec = '%f%f%f%s%[^\n\r]';

%% �e�L�X�g �t�@�C�����J���܂��B
fileID = fopen(filename,'r');

%% �f�[�^�̗�������ݒ�ɏ]���ēǂݎ��܂��B
% ���̌Ăяo���́A���̃R�[�h�̐����Ɏg�p���ꂽ�t�@�C���̍\���Ɋ�Â��Ă��܂��B�ʂ̃t�@�C���ŃG���[����������ꍇ�́A�C���|�[�g
% �c�[������R�[�h�̍Đ��������݂Ă��������B
dataArray = textscan(fileID, formatSpec, endRow(1)-startRow(1)+1, 'Delimiter', delimiter, 'TextType', 'string', 'HeaderLines', startRow(1)-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');
for block=2:length(startRow)
    frewind(fileID);
    dataArrayBlock = textscan(fileID, formatSpec, endRow(block)-startRow(block)+1, 'Delimiter', delimiter, 'TextType', 'string', 'HeaderLines', startRow(block)-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');
    for col=1:length(dataArray)
        dataArray{col} = [dataArray{col};dataArrayBlock{col}];
    end
end

%% �e�L�X�g �t�@�C������܂��B
fclose(fileID);

%% �C���|�[�g�ł��Ȃ��f�[�^�̌㏈���B
% �C���|�[�g���ɁA�C���|�[�g�ł��Ȃ��f�[�^�̋K�����K�p����Ȃ��������߁A�㏈���R�[�h���܂܂�Ă��܂���B�C���|�[�g�ł��Ȃ��f�[�^�ɓK�p�ł���R�[�h�𐶐�����ɂ́A�t�@�C�����̃C���|�[�g�ł��Ȃ��Z����I�����Ă���X�N���v�g���Đ������܂��B

%% �C���|�[�g�����z����ϐ����Ɋ��蓖�Ă܂�
hci0 = dataArray{:, 1};
hci1 = dataArray{:, 2};
hci2 = dataArray{:, 3};
flg = dataArray{:, 4};


