#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <numeric>
#include <thread>
#include <unistd.h>
#include <random>

using namespace std;

const static char mac[18] = "D0:01:00:3E:64:4D";    // Beacon's Mac address
const static int  width   = 40;                     // filter's max width

int hci0_rssi = 0;

void BLE_hci0(){
    char s[256];                                        // Bluetooth's Data
    char c[256];                                        // Command
    int rssi = 0;                                       // RSSI
    int num = 0;                                        // SAMPLE's count
    bool find_flg = false;                              // BLE Tag FLAG
    deque<int> samples;                                 // SAMPLE
    FILE *fp;                                           // hcidump Command

    while(true){
        sleep(1);

        system("sudo hcitool -i hci0 lescan --pa --du > /dev/null &");
        fp=popen("sudo hcidump -i hci0","r");

        find_flg = false;
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
        system("sudo hcitool -i hci0 cmd 08 000c 00 03 > /dev/null");
        system("sudo kill `pidof hcitool` > /dev/null");

        rssi = atoi(&s[12]);
        if(samples.size() > width){
            samples.pop_front();
        }
        samples.push_back(rssi);

        hci0_rssi = accumulate(samples.begin(), samples.end(), 0) / samples.size();
        cout << hci0_rssi << "*" << endl;
    }
}

int main(int argc,char **argv){
    thread hci0(BLE_hci0);

    hci0.detach();
    while(true){
        sleep(1);
        cout << hci0_rssi << endl;
    }

    return 0;
}
