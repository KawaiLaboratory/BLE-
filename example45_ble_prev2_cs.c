/***************************************************************************************
BLEタグを使った盗難防止システム(徐々に遠ざかるケースに対応)

ご注意：必ずしも盗難を防止できるシステムではありません。
出版社および当方はいかなる損害に対しても、一切の責任を負いません。

                                                        Copyright (c) 2016 Wataru KUNINO
***************************************************************************************/
#include <stdio.h>                                              // 標準入出力ライブラリ
#include <stdlib.h>                                             // system命令に使用
#include <string.h>                                             // strncmp命令に使用
#include <time.h>                                               // time命令time_tに使用
#include "../libs/checkMac.c"                                   // MACアドレスの書式確認
#include "../libs/kbhit.c"
#define FORCE_INTERVAL        1                                // データ取得間隔(秒)
#define ALART_RSSI_OFFSET     10                                // 警告1:RSSI低下レベル
#define ALART_RSSI_TIMES       4                                // 警告2:RSSI低下回数

int main(int argc,char **argv){
    char  s0[256];
    char  s1[256];
    char  s2[256];
    FILE  *fp0;
    FILE  *fp1;
    FILE  *fp2;
    time_t timer;                                                       // タイマー用
    time_t trig=0;                                                      // 取得時刻保持
    int rssi1=-999;                                                      // 受信レベル
    int rssi2=-999;                                                      // 受信レベル
    int rssi3=-999;                                                      // 受信レベル
    int rssi_times=0;                                                   // 受信低下回数
    int mac_f1=0;                                                        // MAC一致フラグ
    int mac_f2=0;                                                        // MAC一致フラグ
    int mac_f3=0;                                                        // MAC一致フラグ
    int i;
    int j;
    int k;
    char c;
    
    if(argc != 2 || checkMac(argv[1]) ){                                // 書式の確認
        fprintf(stderr,"usage: %s MAC_Address\n",argv[0]);              // 入力誤り表示
        return -1;                                                      // 終了
    }

    printf("Example 45 BLE Prev 2 (0:off 1:LED 2:Alert Q:Quit)\n");     // 起動表示
    while(1){
        if( kbhit() ){                                                  // キー入力時
            c=getchar();                                                // 入力キー取得
            if(c=='q' || c=='Q') break;                                 // 「Q」で終了
            if(c!='1' && c!='2') c='0';                                 // 1,2以外で0に
            sprintf(s0,"sudo gatttool -b %s -t random --char-write -a 13 -n 0%c",argv[1],c);         // コマンド作成
            system(s0);                                                  // コマンド実行
            sprintf(s1,"sudo gatttool -b %s -t random --char-write -a 13 -n 0%c",argv[1],c);         // コマンド作成
            system(s1);                                                  // コマンド実行
            sprintf(s2,"sudo gatttool -b %s -t random --char-write -a 13 -n 0%c",argv[1],c);         // コマンド作成
            system(s2);                                                  // コマンド実行
        }
        system("sudo hcitool -i hci0 lescan --pa --du > /dev/null &");          // LEスキャン
        fp0=popen("sudo hcidump -i hci0","r");                                   // hcidump実行
        printf("fp0)\n");     // 起動表示 
        system("sudo hcitool -i hci1 lescan --pa --du > /dev/null &");          // LEスキャン
        fp1=popen("sudo hcidump -i hci1","r");                                   // hcidump実行
        printf("fp1)\n");     // 起動表示
        system("sudo hcitool -i hci2 lescan --pa --du > /dev/null &");          // LEスキャン
        fp2=popen("sudo hcidump -i hci2","r");                                   // hcidump実行
        printf("fp2)\n");     // 起動表示

        
        while(fp0){
            fgets(s0,256,fp0);                                            // hcidumpから
            printf("check1  \n");     // 起動表示
            if( strncmp(&s0[4],"LE Advertising Report",21)==0 ){         // ビーコン判
                for(i=0;i<6;i++){
                    fgets(s0,256,fp0);                                    // データの取得
                    if(i==1){                                           // 2行目の時
                        if(strncmp(&s0[13],argv[1],17)==0) mac_f1=1;     // MAC一致を確認
                        else mac_f1=0;                                   // mac_f1に代入
                    }
                }
                if(mac_f1) break;                                        // whileを抜ける(得られるまで抜けない) 
            }
        }
        pclose(fp0);                                                     // popenを閉じる
        printf("check2 \n");     // 起動表示

        while(fp1){
            fgets(s1,256,fp1);                                            // hcidumpから
            if( strncmp(&s1[4],"LE Advertising Report",21)==0 ){         // ビーコン判定
                for(j=0;j<6;j++){
                    fgets(s1,256,fp1);                                    // データの取得
                    if(j==1){                                           // 2行目の時
                        if(strncmp(&s1[13],argv[1],17)==0) mac_f2=1;      // MAC一致を確認
                        else mac_f2=0;                                   // mac_f2に代入
                    }
                }
                if(mac_f2) break;                                        // whileを抜ける
            }                                                           // (得られるまで
        }                                                               //     抜けない)
        pclose(fp1);                                                     // popenを閉じる        
        printf("check3 \n");     // 起動表示

        while(fp2){
            fgets(s2,256,fp2);                                            // hcidumpから
            if( strncmp(&s2[4],"LE Advertising Report",21)==0 ){         // ビーコン判定
                for(k=0;k<6;k++){
                    fgets(s2,256,fp2);                                    // データの取得
                    if(k==1){                                           // 2行目の時
                        if(strncmp(&s2[13],argv[1],17)==0) mac_f3=1;      // MAC一致を確認
                        else mac_f3=0;                                   // mac_f3に代入
                    }
                }
                if(mac_f3) break;                                        // whileを抜ける
            }                                                           // (得られるまで
        }                                                               //     抜けない)
        pclose(fp2);                                                     // popenを閉じる
        printf("check4 \n");     // 起動表示
        system("sudo hcitool -i hci0 cmd 08 000c 00 01 > /dev/null");           // LEスキャン止
        system("sudo kill `pidof hcitool` > /dev/null");                // プロセス停止
        system("sudo hcitool -i hci1 cmd 08 000c 00 01 > /dev/null");           // LEスキャン止
        system("sudo kill `pidof hcitool` > /dev/null");                // プロセス停止
        system("sudo hcitool -i hci2 cmd 08 000c 00 01 > /dev/null");           // LEスキャン止
        system("sudo kill `pidof hcitool` > /dev/null");                // プロセス停止
        printf("check5 \n");     // 起動表示

        i=atoi(&s0[12]);                                                 // RSSI値をiへ
        j=atoi(&s1[12]);                                                 // RSSI値をjへ
        k=atoi(&s2[12]);                                                 // RSSI値をkへ
        printf("check5 \n");     // 起動表示

        if( i < rssi1 ) rssi_times++; else rssi_times = 0;               // 劣化検出
        if( j < rssi2 ) rssi_times++; else rssi_times = 0;               // 劣化検出
        if( k < rssi3 ) rssi_times++; else rssi_times = 0;               // 劣化検出
        printf("check6 \n");     // 起動表示

        if( i+ALART_RSSI_OFFSET < rssi1 ) rssi_times=99;                 // 著しい劣化
        if( j+ALART_RSSI_OFFSET < rssi2 ) rssi_times=99;                 // 著しい劣化
        if( k+ALART_RSSI_OFFSET < rssi3 ) rssi_times=99;                 // 著しい劣化
        printf("check7 \n");     // 起動表示

        rssi1=i;
        rssi2=j;
        rssi3=k;
        printf("check8 \n");     // 起動表示
        printf("Recieved BLE Beacon, RSSI 1=%d\n , ",rssi1);                  // 受信,RSSI 1表示
        printf("Recieved BLE Beacon, RSSI 2=%d\n , ",rssi2);                  // 受信,RSSI 2表示
        printf("Recieved BLE Beacon, RSSI 3=%d\n , ",rssi3);                  // 受信,RSSI 3表示
        printf("check9 \n");     // 起動表示

        printf("Times Degraded=%d\n",rssi_times);                       // 劣化回数表示
        if( rssi_times >= ALART_RSSI_TIMES){                            // 連続劣化回数
            printf("Send Alart!\n");                                    // 警告を送信
            sprintf(s0,"sudo gatttool -b %s --char-write -a 13 -n 02",argv[1]);
            system(s0);                                                  // コマンド実行
            sprintf(s1,"sudo gatttool -b %s --char-write -a 13 -n 02",argv[1]);
            system(s1);                                                  // コマンド実行
            sprintf(s2,"sudo gatttool -b %s --char-write -a 13 -n 02",argv[1]);
            system(s2);                                                  // コマンド実行
        }
        printf("check10 \n");     // 起動表示
        time(&timer);                                                   // 現在時刻取得
        if( timer >= trig && rssi_times < ALART_RSSI_TIMES){            // 定期時刻時
            sprintf(s0,"sudo gatttool -b %s -t random --char-read -a 0x000f > /dev/null",argv[1]);
            system(s0);                                                  // コマンド実行
            sprintf(s1,"sudo gatttool -b %s -t random --char-read -a 0x000f > /dev/null",argv[1]);
            system(s1);                                                  // コマンド実行
            sprintf(s2,"sudo gatttool -b %s -t random --char-read -a 0x000f > /dev/null",argv[1]);
            system(s2);                                                  // コマンド実行
            trig = timer + FORCE_INTERVAL;                              // 次trigを設定
        }
    }
    return 0;
}
