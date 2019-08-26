#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <numeric>
#include <thread>

using namespace std;

int main(int argc,char **argv){
    const static char mac[18] = "D0:01:00:3E:64:4D";    // Beacon's Mac address
    const static int  width   = 40;                     // filter's max width

    char s[256];                                        // Bluetooth's Data
    char c[256];                                        // Command
    int rssi = 0;                                       // RSSI
    int num = 0;                                        // SAMPLE's count
    double rssi_ave = 0;                                // MOVING AVERAGE
    bool find_flg = false;                              // BLE Tag FLAG
    deque<int> samples;                                 // SAMPLE
    FILE *fp;                                           // hcidump Command

    while(true){
        find_flg = false;
        system("sudo hcitool -i hci0 lescan --pa --du > /dev/null &");
        fp=popen("sudo hcidump -i hci0","r");

        if(fp == NULL){
            cout << "ERROR" << endl;
            return 0;
        }

        while(fp){
            fgets(s, 256, fp);
            if(strncmp(&s[4], "LE Advertising Report", 21) == 0){
                for(int i = 0; i < 6; i++){
                    fgets(s, 256, fp);
                    if(i == 1){
                        if(strncmp(&s[13], mac, 17) == 0){
                            find_flg = true;
                        }
                    }
                }
                if(find_flg){
                    break;
                }
            }
        }

        pclose(fp);
        system("sudo hcitool cmd 08 000c 00 03 > /dev/null");
        system("sudo kill `pidof hcitool` > /dev/null");

        rssi = atoi(&s[12]);

        if(samples.size() > width){
            samples.pop_front();
        }
        samples.push_back(rssi);

        rssi_ave = accumulate(samples.begin(), samples.end(), 0) / double(samples.size());
        cout << "rssi:" << rssi << ", ave:" << rssi_ave << endl;
        // sprintf(c, "sudo gatttool -b %s -t random --char-read -a 0x0022", mac);
        // system(c);
    }

    return 0;
}
